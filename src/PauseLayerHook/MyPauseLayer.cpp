#include "MyPauseLayer.hpp"

void MyPauseLayer::customSetup() {

    PauseLayer::customSetup();

    auto rightSideMenu = this->getChildByID("right-button-menu");

    auto plainButtonSprite = CCSprite::createWithSpriteFrameName("GJ_plainBtn_001.png");
    plainButtonSprite->setScale(0.6f);

    auto button = CCMenuItemSpriteExtra::create(plainButtonSprite, nullptr, this, menu_selector(MyPauseLayer::onDeliciousMenuOpened));

    button->setID("delicious-button"_spr);
    rightSideMenu->addChild(button);
    rightSideMenu->updateLayout();
}

void MyPauseLayer::onDeliciousMenuOpened(CCObject*){
    log::info("delicious button was called");
    auto deliciousPopup = DeliciousPopup::create(0);
    deliciousPopup->setID("Delicious-Popup"_spr);
    deliciousPopup->setZOrder(999);
    this->addChild(deliciousPopup);
}