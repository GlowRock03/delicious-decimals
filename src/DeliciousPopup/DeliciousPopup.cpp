#include "DeliciousPopup.hpp"

DeliciousPopup* DeliciousPopup::create(int value) {

    auto popup = new DeliciousPopup;

    if (popup->init(value)) {

        popup->autorelease();
        return popup;
    }

    delete popup;
    return nullptr;
}

bool DeliciousPopup::init(int value) {

    if (!Popup::init(320.f, 140.f)) return false;

    auto titleSprite = CCLabelBMFont::create("Title Sprite", "bigFont.fnt");
    titleSprite->setPosition({ this->getContentWidth() / 2, (this->getContentHeight() / 2) + (this->getContentHeight() / 8) });
    this->addChild(titleSprite);

    auto inputTextTitle = CCLabelBMFont::create("Delicious Decimals:", "bigFont.fnt");
    inputTextTitle->setScale(0.4f);
    inputTextTitle->setPosition({ this->getContentWidth() / 2, (this->getContentHeight() / 2) });
    this->addChild(inputTextTitle);

    auto decimalInput = TextInput::create(100.f, "", "chatFont.fnt");
    decimalInput->setFilter("0123456789");
    decimalInput->setCallback([this](std::string const& text) {
        inputValue = text;
        log::info("Result from input: {}", inputValue);
    });
    decimalInput->setString(inputValue);
    decimalInput->setScale(0.8f);
    decimalInput->setPosition({ this->getContentWidth() / 2, (this->getContentHeight() / 2) - (this->getContentHeight() / 12)});
    decimalInput->setID("decimal-input"_spr);
    this->addChild(decimalInput);

    return true;
}