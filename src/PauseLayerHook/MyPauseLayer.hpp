#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

#include "../DeliciousPopup/DeliciousPopup.hpp"

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
public:

    void customSetup();
    void onDeliciousMenuOpened(CCObject*);
};