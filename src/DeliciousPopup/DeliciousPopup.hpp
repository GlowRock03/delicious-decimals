#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class DeliciousPopup : public Popup {
public: 

    static DeliciousPopup* create(int value);

protected:

    std::string inputValue = "0";

    bool init(int value);
};