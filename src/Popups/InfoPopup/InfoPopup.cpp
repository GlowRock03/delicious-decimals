#include "InfoPopup.hpp"

InfoPopup* InfoPopup::create(std::string const& title, std::string const& content, float popupHeight) {
    auto ret = new InfoPopup();

    if (ret && ret->setup(title, content, popupHeight)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool InfoPopup::setup(std::string const& title, std::string const& content, float popupHeight) {

    if (!Popup::init(360.f, popupHeight, "GJ_square04.png", {0, 0, 80, 80})) return false;

    this->setTitle(title.c_str());
    m_mainLayer->setLayout(AnchorLayout::create());

    auto textArea = TextArea::create(content, "chatFont.fnt", 0.6f, 300.f, {0.f, 1.f}, 10.f, false);
    textArea->setAnchorPoint({0.f, 1.f});
    textArea->setPosition({16.f, popupHeight - 50.f});
    m_mainLayer->addChild(textArea);

    auto btnSpr = ButtonSprite::create("OK");
    auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(InfoPopup::onClose));

    auto menu = CCMenu::create();
    menu->addChild(btn);
    menu->setPosition({m_size.width / 2, 30.f});

    auto topLeftCorner = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    topLeftCorner->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::TopLeft));
    topLeftCorner->setAnchorPoint({0.f, 1.f});
    topLeftCorner->setFlipY(true);
    topLeftCorner->setID("top-right-corner");
    m_mainLayer->addChild(topLeftCorner);

    // Bottom Left
    auto bottomLeftCorner = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    bottomLeftCorner->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::BottomLeft));
    bottomLeftCorner->setAnchorPoint({0.f, 0.f});
    bottomLeftCorner->setID("bottom-left-corner");
    m_mainLayer->addChild(bottomLeftCorner);

    // Bottom Right
    auto bottomRightCorner = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    bottomRightCorner->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::BottomRight));
    bottomRightCorner->setAnchorPoint({1.f, 0.f});
    bottomRightCorner->setFlipX(true);
    bottomRightCorner->setID("bottom-right-corner");
    m_mainLayer->addChild(bottomRightCorner);

    // Top Right
    auto topRightCorner = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
    topRightCorner->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::TopRight));
    topRightCorner->setAnchorPoint({1.f, 1.f});
    topRightCorner->setFlipX(true);
    topRightCorner->setFlipY(true);
    topRightCorner->setID("top-right-corner");
    m_mainLayer->addChild(topRightCorner);

    m_mainLayer->addChild(menu);
    m_mainLayer->updateLayout();
    m_closeBtn->setVisible(false);

    return true;
}