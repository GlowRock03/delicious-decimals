#include "DeliciousPopup.hpp"

DeliciousPopup* DeliciousPopup::create() {

    auto popup = new DeliciousPopup;

    if (popup->init(0)) {

        popup->autorelease();
        return popup;
    }

    delete popup;
    return nullptr;
}

bool DeliciousPopup::init(int value) {

    if (!Popup::init(320.f, 140.f)) return false;

    auto buttonMenu = CCMenu::create();
    buttonMenu->setPosition({0, 0});

    deliciousDecimals = Mod::get()->getSettingValue<int64_t>("delicious-decimals");
    deliciousRate = Mod::get()->getSettingValue<float>("delicious-deviation-degree");
    std::string dialTarget = Mod::get()->getSettingValue<std::string>("delicious-dial");

    for (int i = 0; i <= dialOptions.size(); i++) {
        if (dialOptions[i] == dialTarget) {
            dialSettingIndex = i;
        }
    }

    // Title Sprite
    auto titleSprite = CCLabelBMFont::create("Title Sprite", "bigFont.fnt");
    titleSprite->setPosition({ this->getContentWidth() / 2, (this->getContentHeight() / 2) + (this->getContentHeight() / 8) });
    titleSprite->setID("title-sprite");
    this->addChild(titleSprite);

    // Decimal Title
    auto decimalTextTitle = CCLabelBMFont::create("Delicious Decimals (DD):", "bigFont.fnt");
    decimalTextTitle->setScale(0.3f);
    decimalTextTitle->setPosition({ this->getContentWidth() / 2 - this->getContentWidth() / 8, (this->getContentHeight() / 2) });
    decimalTextTitle->setID("decimal-text");
    this->addChild(decimalTextTitle);

    // Decimal Input
    auto decimalInput = TextInput::create(50.f, std::to_string(deliciousDecimals), "chatFont.fnt");
    decimalInput->setFilter("0123456789");
    decimalInput->setCallback([this](std::string const& text) {

        auto parsed = numFromString<int64_t>(text);
        if (!parsed) return;

        int64_t maxDecimals = 0;
        if (auto pl = static_cast<MyPlayLayer*>(PlayLayer::get())) maxDecimals = pl->getMaxDecimals();
        deliciousDecimals = std::clamp(*parsed, int64_t(0), maxDecimals);

        log::info("Result from input: {}", deliciousDecimals);
        Mod::get()->setSettingValue<int64_t>("delicious-decimals", deliciousDecimals);
    });
    decimalInput->setString(std::to_string(deliciousDecimals));
    decimalInput->setScale(0.8f);
    decimalInput->setPosition({ this->getContentWidth() / 2 - this->getContentWidth() / 8, (this->getContentHeight() / 2) - (this->getContentHeight() / 12)});
    decimalInput->setID("decimal-input");
    this->addChild(decimalInput);

    // Dial Left Button
    auto leftDialButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(DeliciousPopup::onLeftDialButton));
    leftDialButton->setID("left-dial-button");
    leftDialButton->setPosition({300, 100});
    buttonMenu->addChild(leftDialButton);

    // Dial Title
    dialInput = TextInput::create(50.f, dialOptions[dialSettingIndex], "bigFont.fnt");
    dialInput->setString(dialOptions[dialSettingIndex]);
    dialInput->setScale(0.8f);
    dialInput->setEnabled(false);
    dialInput->hideBG();
    dialInput->setPosition({350, 100});
    buttonMenu->addChild(dialInput);

    // Dial Right Button
    auto rightSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    rightSprite->setFlipX(true);
    auto rightDialButton = CCMenuItemSpriteExtra::create(rightSprite, this, menu_selector(DeliciousPopup::onRightDialButton));
    rightDialButton->setID("right-dial-button");
    rightDialButton->setPosition({400, 100});
    buttonMenu->addChild(rightDialButton);

    // Rate Title
    auto rateTextTitle = CCLabelBMFont::create("Delicious Deviation Degree (DDD):", "bigFont.fnt");
    rateTextTitle->setScale(0.3f);
    rateTextTitle->setPosition({ this->getContentWidth() / 2 + this->getContentWidth() / 8, (this->getContentHeight() / 2) });
    rateTextTitle->setID("decimal-text");
    this->addChild(rateTextTitle);

    // Rate Input
    auto rateInput = TextInput::create(50.f, fmt::format("{:.1f}", deliciousRate), "chatFont.fnt");
    rateInput->setFilter("0123456789.");
    rateInput->setCallback([this](std::string const& text) {

        auto parsed = numFromString<float>(text);
        if (!parsed) return;

        deliciousRate = std::clamp(*parsed, 0.1f, 100.0f);

        log::info("Result from input: {}", deliciousRate);
        Mod::get()->setSettingValue<float>("delicious-deviation-degree", deliciousRate);
    });
    rateInput->setString(fmt::format("{:.1f}", deliciousRate));
    rateInput->setScale(0.8f);
    rateInput->setPosition({ this->getContentWidth() / 2 + this->getContentWidth() / 8, (this->getContentHeight() / 2) - (this->getContentHeight() / 12)});
    rateInput->setID("rate-input");
    this->addChild(rateInput);

    this->addChild(buttonMenu);

    return true;
}

void DeliciousPopup::onExit() {

    Popup::onExit();

    if (auto pl = static_cast<PlayLayer*>(PlayLayer::get())) {
        pl->updateProgressbar();
    }
}

void DeliciousPopup::onLeftDialButton(CCObject* sender) {

    dialSettingIndex = dialSettingIndex == 0 ? 2 : dialSettingIndex - 1;
    dialInput->setString(dialOptions[dialSettingIndex]);
    Mod::get()->setSettingValue<std::string>("delicious-dial", dialOptions[dialSettingIndex]);

    log::info("Left dial clicked");
}

void DeliciousPopup::onRightDialButton(CCObject* sender) {

    dialSettingIndex = dialSettingIndex == 2 ? 0 : dialSettingIndex + 1;
    dialInput->setString(dialOptions[dialSettingIndex]);
    Mod::get()->setSettingValue<std::string>("delicious-dial", dialOptions[dialSettingIndex]);
    log::info("Right dial clicked");
}