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

    deliciousDecimals = Mod::get()->getSettingValue<int64_t>("delicious-decimals");

    auto titleSprite = CCLabelBMFont::create("Title Sprite", "bigFont.fnt");
    titleSprite->setPosition({ this->getContentWidth() / 2, (this->getContentHeight() / 2) + (this->getContentHeight() / 8) });
    this->addChild(titleSprite);

    auto inputTextTitle = CCLabelBMFont::create("Delicious Decimals:", "bigFont.fnt");
    inputTextTitle->setScale(0.4f);
    inputTextTitle->setPosition({ this->getContentWidth() / 2, (this->getContentHeight() / 2) });
    this->addChild(inputTextTitle);

    auto decimalInput = TextInput::create(100.f, std::to_string(deliciousDecimals), "chatFont.fnt");
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
    decimalInput->setPosition({ this->getContentWidth() / 2, (this->getContentHeight() / 2) - (this->getContentHeight() / 12)});
    decimalInput->setID("decimal-input"_spr);
    this->addChild(decimalInput);

    return true;
}

void DeliciousPopup::onExit() {

    Popup::onExit();

    if (auto pl = static_cast<PlayLayer*>(PlayLayer::get())) {
        pl->updateProgressbar();
    }
}