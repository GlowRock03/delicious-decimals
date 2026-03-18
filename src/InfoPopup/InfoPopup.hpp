#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class InfoPopup : public Popup {
public:
    static InfoPopup* create(std::string const& title, std::string const& content, float popupHeight);

protected:
    bool setup(std::string const& title, std::string const& content, float popupHeight);
};