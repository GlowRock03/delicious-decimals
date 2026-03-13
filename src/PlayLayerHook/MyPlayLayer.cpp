#include "MyPlayLayer.hpp"
#include "../Utility/Utility.hpp"

void MyPlayLayer::onModify(auto& self) {

    if (!self.setHookPriorityPost("PlayLayer::updateProgressbar", Priority::Last)) {

        geode::log::warn("Failed to set hook priority");
    }
}

bool MyPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {

    if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

    if (!m_fields->extraLabelContainer) {
        m_fields->extraLabelContainer = CCNode::create();                   // container needed early for online levels, Node IDs requirement
        m_fields->extraLabelContainer->setID("extra-percent-labels"_spr);
        this->addChild(m_fields->extraLabelContainer);
    }

    m_fields->currentDeviationDecimals = 0;

    return true;
}

void MyPlayLayer::updateProgressbar() {

    PlayLayer::updateProgressbar();

    if (!m_percentageLabel || m_level->isPlatformer()) return;

    std::string dialSetting = Mod::get()->getSettingValue<std::string>("delicious-dial");
    size_t decimalLength;
    if (dialSetting == "Definitive") {
        decimalLength = std::min(getMaxDecimals(), static_cast<size_t>(Mod::get()->getSettingValue<int64_t>("delicious-decimals")));
        Mod::get()->setSettingValue<int64_t>("delicious-decimals", static_cast<int64_t>(decimalLength));
    } else if (dialSetting == "Dynamix") {
        decimalLength = getDynamicDecimals(static_cast<size_t>(Mod::get()->getSettingValue<int64_t>("delicious-decimals")));
    } else if (dialSetting == "Deviation") {

        float deviationRate = Mod::get()->getSettingValue<float>("delicious-deviation-degree");
        size_t maxDecimals = std::min(getMaxDecimals(), static_cast<size_t>(Mod::get()->getSettingValue<int64_t>("delicious-decimals")));

        float growthPerFrame = static_cast<float>(deviationRate) / 100.f;
        m_fields->deviationAccumulator += growthPerFrame;

        size_t addDecimals = static_cast<size_t>(m_fields->deviationAccumulator);
        m_fields->deviationAccumulator -= addDecimals;
        m_fields->currentDeviationDecimals = std::min(m_fields->currentDeviationDecimals + addDecimals, maxDecimals);

        decimalLength = m_fields->currentDeviationDecimals;
    }

    if (!m_fields->wrappingInitialized) {

        m_percentageLabel->setAnchorPoint({0.0f, 0.5f});
        m_percentageLabel->setAlignment(kCCTextAlignmentLeft);
        m_fields->wrappingInitialized = true;
    }

    double percent = getActualProgress();

    if (m_fields->cachedPercentValue != percent) {

        std::string base = Utility::formatWithDecimals(percent, decimalLength);
        std::string fullPercentText;

        if (decimalLength == 0) {
            fullPercentText = std::to_string((int)percent);
        } else if (std::remainder(percent, 1.f) == 0) {
            fullPercentText = Utility::padWithZeros(base, decimalLength);
        } else {
            fullPercentText = Utility::addFakeDecimals(base, decimalLength, percent);
        }

        fullPercentText += "%";

        m_fields->cachedPercentText = fullPercentText;
        m_fields->cachedPercentValue = percent;
    }

    auto fullText = m_fields->cachedPercentText;

    m_percentageLabel->setString(fullText.c_str());
    auto advances = getCharacterAdvances(m_percentageLabel);

    float firstLineWidth = getAvailableWidth() * 2;
    float otherLineWidth = getScreenWidth() * 2;

    auto lines = Utility::wrapTextByAdvances(fullText, advances, firstLineWidth, otherLineWidth);

    if (!m_fields->extraLabelContainer) {

        m_fields->extraLabelContainer = CCNode::create();
        m_fields->extraLabelContainer->setID("extra-percent-labels"_spr);
        this->addChild(m_fields->extraLabelContainer);
    }

    for (auto lbl : m_fields->extraLabels) lbl->removeFromParentAndCleanup(true);

    m_fields->extraLabels.clear();
    m_percentageLabel->setString(lines[0].c_str());

    float y = m_percentageLabel->getPositionY() - m_fields->lineHeight;

    if (lines.size() > 1) {

        for (size_t i = 1; i < lines.size(); i++) {

            auto lbl = CCLabelBMFont::create(lines[i].c_str(), m_percentageLabel->getFntFile());

            lbl->setAnchorPoint({0.f, 0.5f});
            lbl->setScale(m_percentageLabel->getScale());
            lbl->setAlignment(kCCTextAlignmentLeft);
            lbl->setPosition({0.f, y});
            lbl->setID(fmt::format("percent-label-{}", i));

            m_fields->extraLabelContainer->addChild(lbl);
            m_fields->extraLabels.push_back(lbl);

            y -= m_fields->lineHeight;
        }
    }
}

float MyPlayLayer::getScreenWidth() {

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    return winSize.width;
}

float MyPlayLayer::getScreenHeight() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    return winSize.height;
}

float MyPlayLayer::getAvailableWidth() {

    float screenRight = getScreenWidth();

    float labelX = m_percentageLabel->getPositionX();
    auto children = m_percentageLabel->getChildren();

    if (!children || children->count() == 0) return screenRight - labelX;

    auto firstGlyph = static_cast<CCNode*>(CCArrayExt(children)[0]);
    float firstGlyphLeft = firstGlyph->getPositionX() - firstGlyph->getContentSize().width * 0.5f * firstGlyph->getScaleX();

    float labelLeftEdge = labelX + firstGlyphLeft;
    float available = screenRight - labelLeftEdge;

    return std::max(available, 0.f);
}

double MyPlayLayer::getActualProgress() {

    double percent;

    if (m_level->m_timestamp > 0) {
        percent = (static_cast<double>(m_gameState.m_levelTime) * 240.0) / static_cast<double>(m_level->m_timestamp) * 100.0;
    } else {
        percent = static_cast<double>(m_player1->getPositionX()) / static_cast<double>(m_levelLength) * 100.0;
    }

    return std::clamp(percent, 0.0, 100.0);
}

std::vector<float> MyPlayLayer::getCharacterAdvances(CCLabelBMFont* label) {

    std::vector<float> advances;

    auto children = label->getChildren();
    if (!children) return advances;

    std::vector<CCNode*> sprites;
    for (auto obj : CCArrayExt(children)) {
        sprites.push_back(static_cast<CCNode*>(obj));
    }

    for (size_t i = 0; i < sprites.size(); i++) {

        float pos = sprites[i]->getPositionX();
        float width = sprites[i]->getContentSize().width * sprites[i]->getScaleX();
        float left = pos - width * 0.5f;

        if (i + 1 < sprites.size()) {
            float nextPos = sprites[i + 1]->getPositionX();
            float nextWidth = sprites[i + 1]->getContentSize().width * sprites[i + 1]->getScaleX();
            float nextLeft = nextPos - nextWidth * 0.5f;
            advances.push_back(nextLeft - left);
        } else {
            float labelWidth = label->getContentSize().width;
            advances.push_back(labelWidth - left);
        }
    }

    return advances;
}

size_t MyPlayLayer::getMaxDecimals() {

    if (m_fields->maxDecimalsComputed)
        return m_fields->cachedMaxDecimals;

    if (!m_percentageLabel) return 0;

    float firstLineWidth = getAvailableWidth() * 2;
    float otherLineWidth = getScreenWidth() * 2;

    size_t maxLines = static_cast<size_t>(getScreenHeight() / m_fields->lineHeight);
    if (maxLines == 0) return 0;

    std::string mock = "100.";
    mock.append(m_fields->MAX_DECIMALS, '0');
    mock += "%";

    auto tempLabel = CCLabelBMFont::create(mock.c_str(), m_percentageLabel->getFntFile());
    tempLabel->setScale(m_percentageLabel->getScale());

    auto advances = getCharacterAdvances(tempLabel);
    auto lines = Utility::wrapTextByAdvances(mock, advances, firstLineWidth, otherLineWidth);

    size_t charAmount = 0;
    for (size_t i = 0; i < std::min(lines.size(), maxLines); i++) {
        charAmount += lines[i].size();
    }

    size_t baseChars = 5;
    if (charAmount <= baseChars) {
        m_fields->cachedMaxDecimals = 0;
    } else {
        m_fields->cachedMaxDecimals = charAmount - baseChars;
    }

    m_fields->maxDecimalsComputed = true;
    log::info("Max Deciamls Found: {}", m_fields->cachedMaxDecimals);
    return m_fields->cachedMaxDecimals;
}

size_t MyPlayLayer::getDynamicDecimals(size_t decimalLength) {

    if (!m_percentageLabel) return 0;

    double percent = getActualProgress();
    double rounded = std::floor(percent * 1000);

    if (m_fields->cachedPercentRounded == rounded) return m_fields->cachedDecimalLength;

    float firstLineWidth = getAvailableWidth() * 2;
    float otherLineWidth = getScreenWidth() * 2;

    size_t maxLines = static_cast<size_t>(getScreenHeight() / m_fields->lineHeight);
    if (maxLines == 0) return 0;

    if (decimalLength == getMaxDecimals()) {
        decimalLength += 100;
    }

    std::string fullText = Utility::addFakeDecimals(
        Utility::formatWithDecimals(percent, decimalLength),
        decimalLength,
        percent
    ) + "%";

    auto dynamicTempLabel = CCLabelBMFont::create(fullText.c_str(), m_percentageLabel->getFntFile());
    dynamicTempLabel->setScale(m_percentageLabel->getScale());

    auto advances = getCharacterAdvances(dynamicTempLabel);

    static std::vector<float> cumAdvances;
    cumAdvances.resize(advances.size());

    if (!advances.empty()) {
        cumAdvances[0] = advances[0];
        for (size_t i = 1; i < advances.size(); i++) {
            cumAdvances[i] = cumAdvances[i - 1] + advances[i];
        }
    }

    size_t left = 0;
    size_t right = decimalLength;
    size_t best = 0;

    while (left <= right) {

        size_t mid = left + (right - left) / 2;
        size_t testLen = 4 + mid + 1; // "100." + decimals + "%"
        size_t lineStart = 0;
        size_t lineCount = 1;

        float limit = firstLineWidth;

        for (size_t i = 0; i < testLen && i < cumAdvances.size(); i++) {

            float cursor = cumAdvances[i] - (lineStart > 0 ? cumAdvances[lineStart - 1] : 0.f);

            if (cursor > limit && i > lineStart) {

                lineCount++;
                lineStart = i;
                limit = otherLineWidth;

                if (lineCount > maxLines) break;
            }
        }

        if (lineCount > maxLines) {
            right = mid - 1;
        } else {
            best = mid;
            left = mid + 1;
        }
    }

    m_fields->cachedPercentRounded = rounded;
    m_fields->cachedDecimalLength = best;

    return best;
}