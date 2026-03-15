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

    if (!Popup::init(300.f, 210.f)) return false;

    deliciousDecimals = Mod::get()->getSettingValue<int64_t>("delicious-decimals");
    deliciousRate = Mod::get()->getSettingValue<float>("delicious-deviation-degree");
    std::string dialTarget = Mod::get()->getSettingValue<std::string>("delicious-dial");

    for (int i = 0; i < dialOptions.size(); i++) {
        if (dialOptions[i] == dialTarget) {
            dialSettingIndex = i;
        }
    }

    auto mainContainer = CCNode::create();
    mainContainer->setLayout(AnchorLayout::create());
    mainContainer->setContentSize(m_buttonMenu->getContentSize());
    mainContainer->setAnchorPoint({0.5f, 0.5f});
    mainContainer->setPosition(this->getContentSize() / 2);
    mainContainer->setID("main-content-node");
    this->addChild(mainContainer);


    // Title Sprite Row
    auto titleSprite = CCLabelBMFont::create("Title Sprite", "bigFont.fnt");
    titleSprite->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Top)->setOffset({0.f, -30.f}));
    titleSprite->setID("title-sprite");
    

    // Delicious Decimal Row
    auto decimalRow = CCNode::create();
    decimalRow->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center)->setOffset({0.f, 20.f}));
    decimalRow->setLayout(AnchorLayout::create());
    decimalRow->setAnchorPoint({0.5f, 0.5f});
    decimalRow->setContentSize({m_buttonMenu->getContentSize().height, 30.f});
    decimalRow->setID("delicious-decimal-node");

    // Decimal Title
    auto decimalTextTitle = CCLabelBMFont::create("Delicious Decimals (DD):", "bigFont.fnt");
    decimalTextTitle->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Top)->setOffset({0.f, 10.f}));
    decimalTextTitle->setScale(0.3f);
    decimalTextTitle->setID("delicious-decimal-label");

    // Decimal Input
    auto decimalInput = TextInput::create(50.f, std::to_string(deliciousDecimals), "chatFont.fnt");
    decimalInput->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));
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
    decimalInput->setID("delicious-decimal-input");

    decimalRow->addChild(decimalTextTitle);
    decimalRow->addChild(decimalInput);
    decimalRow->updateLayout();


    // Dial Input Row
    auto dialRow = CCMenu::create();
    dialRow->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center)->setOffset({0.f, -20.f}));
    dialRow->setLayout(AnchorLayout::create());
    dialRow->setAnchorPoint({0.5f, 0.5f});
    dialRow->setContentSize({m_buttonMenu->getContentSize().height, 30.f});
    dialRow->setID("dial-node");


    // Dial Left Button
    auto leftSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    leftSprite->setScale(0.8f);
    auto leftDialButton = CCMenuItemSpriteExtra::create(leftSprite, this, menu_selector(DeliciousPopup::onLeftDialButton));
    leftDialButton->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Left)->setOffset({50.f, 0.f}));
    leftDialButton->setID("left-dial-button");

    // Dial Title
    dialInput = TextInput::create(50.f, dialOptions[dialSettingIndex], "bigFont.fnt");
    dialInput->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));
    dialInput->setString(dialOptions[dialSettingIndex]);
    dialInput->setScale(2.f);
    dialInput->setEnabled(false);
    dialInput->hideBG();
    dialInput->setID("dial-label");

    // Dial Right Button
    auto rightSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    rightSprite->setFlipX(true);
    rightSprite->setScale(0.8f);
    auto rightDialButton = CCMenuItemSpriteExtra::create(rightSprite, this, menu_selector(DeliciousPopup::onRightDialButton));
    rightDialButton->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Right)->setOffset({-50.f, 0.f}));
    rightDialButton->setID("right-dial-button");

    dialRow->addChild(leftDialButton);
    dialRow->addChild(dialInput);
    dialRow->addChild(rightDialButton);
    dialRow->updateLayout();
    

    // Growth Rate Row
    auto growthRateRow = CCNode::create();
    growthRateRow->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Bottom)->setOffset({0.f, 50.f}));
    growthRateRow->setLayout(AnchorLayout::create());
    growthRateRow->setAnchorPoint({0.5f, 0.5f});
    growthRateRow->setContentSize({m_buttonMenu->getContentSize().height, 30.f});
    growthRateRow->setID("delicious-deviation-degree-node");

    // Rate Title
    auto rateTextTitle = CCLabelBMFont::create("Delicious Deviation Degree (DDD):", "bigFont.fnt");
    rateTextTitle->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));
    rateTextTitle->setScale(0.3f);
    rateTextTitle->setID("delicious-deviation-degree-label");

    // Rate Input
    auto rateInput = TextInput::create(50.f, fmt::format("{:.1f}", deliciousRate), "chatFont.fnt");
    rateInput->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Bottom)->setOffset({0.f, -10.f}));
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
    rateInput->setID("delicious-deviation-degree-input");

    growthRateRow->addChild(rateTextTitle);
    growthRateRow->addChild(rateInput);
    growthRateRow->updateLayout();


    mainContainer->addChild(growthRateRow);
    mainContainer->addChild(dialRow);
    mainContainer->addChild(decimalRow);
    mainContainer->addChild(titleSprite);
    mainContainer->updateLayout();
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