#pragma once

#include "../PlayLayerHook/MyPlayLayer.hpp"

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class DeliciousPopup : public Popup {
public: 

    static DeliciousPopup* create();

protected:

    int64_t deliciousDecimals;

    bool init(int value);
    void onExit() override;
};