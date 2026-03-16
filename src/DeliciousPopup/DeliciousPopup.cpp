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

    if (!Popup::init(300.f, 210.f, "GJ_square04.png")) return false;

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

    auto decimalButtonMenu = CCMenu::create();
    decimalButtonMenu->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));
    decimalButtonMenu->setLayout(AnchorLayout::create());
    decimalButtonMenu->setAnchorPoint({0.5f, 0.5f});
    decimalButtonMenu->setContentSize({m_buttonMenu->getContentSize().height, 30.f});
    decimalButtonMenu->setID("delicious-decimal-menu");


    // Decimal Title
    auto decimalTextTitle = CCLabelBMFont::create("Delicious Decimals (DD):", "bigFont.fnt");
    decimalTextTitle->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Top)->setOffset({0.f, 10.f}));
    decimalTextTitle->setScale(0.3f);
    decimalTextTitle->setID("delicious-decimal-label");

    // Decimal Input
    auto decimalInput = TextInput::create(100.f, std::to_string(deliciousDecimals), "chatFont.fnt");
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

    auto decimalInfoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    decimalInfoSprite->setScale(0.6f);
    auto decimalInfoButton = CCMenuItemSpriteExtra::create(decimalInfoSprite, this, menu_selector(DeliciousPopup::onDecimalInfoButton));
    decimalInfoButton->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Top)->setOffset({75.f, 10.f}));
    decimalInfoButton->setID("decimal-info-button");


    decimalButtonMenu->addChild(decimalInfoButton);
    decimalRow->addChild(decimalButtonMenu);

    decimalRow->addChild(decimalTextTitle);
    decimalRow->addChild(decimalInput);
    decimalRow->addChild(decimalInfoButton);
    decimalButtonMenu->updateLayout();
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
    leftSprite->setScale(0.6f);
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
    rightSprite->setScale(0.6f);
    auto rightDialButton = CCMenuItemSpriteExtra::create(rightSprite, this, menu_selector(DeliciousPopup::onRightDialButton));
    rightDialButton->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Right)->setOffset({-50.f, 0.f}));
    rightDialButton->setID("right-dial-button");

    auto dialInfoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    dialInfoSprite->setScale(0.6f);
    auto dialInfoButton = CCMenuItemSpriteExtra::create(dialInfoSprite, this, menu_selector(DeliciousPopup::onDialInfoButton));
    dialInfoButton->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Top)->setOffset({75.f, -5.f}));
    dialInfoButton->setID("dial-info-button");

    dialRow->addChild(dialInfoButton);
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

    auto growthRateButtonMenu = CCMenu::create();
    growthRateButtonMenu->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));
    growthRateButtonMenu->setLayout(AnchorLayout::create());
    growthRateButtonMenu->setAnchorPoint({0.5f, 0.5f});
    growthRateButtonMenu->setContentSize({m_buttonMenu->getContentSize().height, 30.f});
    growthRateButtonMenu->setID("delicious-decimal-menu");


    // Rate Title
    auto rateTextTitle = CCLabelBMFont::create("Delicious Deviation Degree (DDD):", "bigFont.fnt");
    rateTextTitle->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));
    rateTextTitle->setScale(0.3f);
    rateTextTitle->setID("delicious-deviation-degree-label");

    // Rate Input
    auto rateInput = TextInput::create(100.f, fmt::format("{:.1f}", deliciousRate), "chatFont.fnt");
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

    auto rateInfoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    rateInfoSprite->setScale(0.6f);
    auto rateInfoButton = CCMenuItemSpriteExtra::create(rateInfoSprite, this, menu_selector(DeliciousPopup::onRateInfoButton));
    rateInfoButton->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Right)->setOffset({-7.f, 0.f}));
    rateInfoButton->setID("degree-info-button");


    growthRateButtonMenu->addChild(rateInfoButton);
    growthRateRow->addChild(growthRateButtonMenu);

    growthRateRow->addChild(rateTextTitle);
    growthRateRow->addChild(rateInput);
    growthRateButtonMenu->updateLayout();
    growthRateRow->updateLayout();


    auto topLeftCorner = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    topLeftCorner->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::TopLeft));
    topLeftCorner->setAnchorPoint({0.f, 1.f});
    topLeftCorner->setFlipY(true);
    topLeftCorner->setID("top-right-corner");

    auto bottomLeftCorner = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    bottomLeftCorner->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::BottomLeft));
    bottomLeftCorner->setAnchorPoint({0.f, 0.f});
    bottomLeftCorner->setID("bottom-left-corner");

    auto bottomRightCorner = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    bottomRightCorner->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::BottomRight));
    bottomRightCorner->setAnchorPoint({1.f, 0.f});
    bottomRightCorner->setFlipX(true);
    bottomRightCorner->setID("bottom-right-corner");

    auto topRightCorner = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    topRightCorner->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::TopRight));
    topRightCorner->setAnchorPoint({1.f, 1.f});
    topRightCorner->setFlipX(true);
    topRightCorner->setFlipY(true);
    topRightCorner->setID("top-right-corner");

    m_mainLayer->addChild(topLeftCorner);
    m_mainLayer->updateLayout();

    mainContainer->addChild(bottomLeftCorner);
    mainContainer->addChild(bottomRightCorner);
    mainContainer->addChild(topRightCorner);
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

//TO DO: create a popup for help
//Note: FLAlertLayer does not fit my needs

void DeliciousPopup::onDecimalInfoButton(CCObject* sender) {

    //"Delicious Decimals (DD): Help"
    //"The amount of <cp>Delicious Decimals (DD)</cp> you want to display.\n\nMin: 0\n\nMax: This value will be limited to match your screen size when you open a level/close the mod Popup in the Pause Menu."
}
void DeliciousPopup::onDialInfoButton(CCObject* sender) {

    //"Delicious Dial (DD): Help"
    //"Choose how your <cp>Delicious Decimals (DD)</cp> behave.\n\n<cj>Definitive</cj>: The 'classic' mode. It directly uses your <cp>Delicious Decimals (DD)</cp> input.\n\n<cj>Dynamix</cj>: Keeps your % in a constant place. This is noticeable with a larger <cp>Delicious Decimals (DD)</cp>.\n<cr>RISK</cr>: With larger values, this will lag the game.\n\n<cj>Deviation</cj>: Sequentially adds more <cp>Delicious Decimals (DD)</cp> to your screen based on <cf>Delicious Deviation Degree</cf> <cp>(DDD)</cp>"
}

void DeliciousPopup::onRateInfoButton(CCObject* sender) {

    //"Delicious Deviation Degree (DDD): Help"
    //"The rate of which the <cp>Delicious Decimals (DD)</cp> increase\n\nEnsure you enable the <cj>Distribution</cj> in <cp>Delicious Dial (DD)</cp> setting to use this feature.\n\nMin: 0.1\n\nMax: 100.0"
}