#include "MyPlayLayer.hpp"
#include "../Utility/Utility.hpp"

void MyPlayLayer::onModify(auto& self) {

    if (!self.setHookPriorityPost("PlayLayer::updateProgressbar", Priority::Last)) {

        geode::log::warn("Failed to set hook priority");
    }
}

bool MyPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {

    if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

    m_fields->extraLabelContainer = CCNode::create();                   // container needed early for online levels, Node IDs requirement
    m_fields->extraLabelContainer->setID("extra-percent-labels"_spr);
    this->addChild(m_fields->extraLabelContainer);

    return true;
}

void MyPlayLayer::updateProgressbar() {

    PlayLayer::updateProgressbar();

    if (!m_percentageLabel || m_level->isPlatformer()) return;

    size_t decimalLength = static_cast<size_t>(Mod::get()->getSettingValue<int64_t>("delicious-decimals"));

    if (!m_fields->wrappingInitialized) {

        m_percentageLabel->setAnchorPoint({0.0f, 0.5f});
        m_percentageLabel->setAlignment(kCCTextAlignmentLeft);
        m_fields->wrappingInitialized = true;
    }

    double percent = getActualProgress();

    if (m_fields->cachedPercentValue != percent) {

        std::string base = Utility::formatWithDecimals(percent, decimalLength);
        std::string fullPercentText;

        if (percent == 0.0 || percent == 100.0) {
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

    float lineHeight = 15.f;
    float y = m_percentageLabel->getPositionY() - lineHeight;

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

            y -= lineHeight;
        }
    }
}

float MyPlayLayer::getScreenWidth() {

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    return winSize.width;
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

    std::sort(sprites.begin(), sprites.end(), [](CCNode* a, CCNode* b) {
        
        return a->getPositionX() < b->getPositionX();
    });

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