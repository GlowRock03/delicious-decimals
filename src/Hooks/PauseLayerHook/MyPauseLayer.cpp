#include "MyPauseLayer.hpp"

void MyPauseLayer::customSetup() {

    PauseLayer::customSetup();

    auto rightSideMenu = this->getChildByID("right-button-menu");

    auto plainButtonSprite = CCSprite::createWithSpriteFrameName("GJ_plainBtn_001.png");
    plainButtonSprite->setScale(0.65f);
    plainButtonSprite->setLayout(AnchorLayout::create());

    auto decimalText = CCLabelBMFont::create("D.D%", "goldFont.fnt");
    decimalText->setScale(0.55f);
    decimalText->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center)->setOffset({1.f, 1.f}));
    plainButtonSprite->addChild(decimalText);

    plainButtonSprite->updateLayout();

    auto button = CCMenuItemSpriteExtra::create(plainButtonSprite, nullptr, this, menu_selector(MyPauseLayer::onDeliciousMenuOpened));

    button->setID("delicious-button"_spr);
    rightSideMenu->addChild(button);
    rightSideMenu->updateLayout();
}

void MyPauseLayer::onDeliciousMenuOpened(CCObject*) {

    auto deliciousPopup = DeliciousPopup::create();
    deliciousPopup->setID("Delicious-Popup"_spr);
    deliciousPopup->setZOrder(999);
    this->addChild(deliciousPopup);
}