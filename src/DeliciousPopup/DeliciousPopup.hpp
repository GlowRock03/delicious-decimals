#pragma once

#include "../Utility/Utility.hpp"
#include "../PlayLayerHook/MyPlayLayer.hpp"
#include "../InfoPopup/InfoPopup.hpp"

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class DeliciousPopup : public Popup {
public: 

    static DeliciousPopup* create();

protected:

    int64_t deliciousDecimals;
    float deliciousRate;
    std::vector<std::string> dialOptions = { "Definitive", "Dynamix", "Deviation" };
    int dialSettingIndex = 0;

    TextInput* dialInput = nullptr;

    bool init(int value);
    void onExit() override;

    void onLeftDialButton(CCObject* sender);
    void onRightDialButton(CCObject* sender);
    void onDecimalInfoButton(CCObject* sender);
    void onDialInfoButton(CCObject* sender);
    void onRateInfoButton(CCObject* sender);
};