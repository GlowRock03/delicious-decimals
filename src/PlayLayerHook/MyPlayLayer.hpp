#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <vector>
#include <string>

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
public:

    struct Fields {
        bool wrappingInitialized = false;
        std::vector<CCLabelBMFont*> extraLabels;
        CCNode* extraLabelContainer = nullptr;
        std::string cachedPercentText = "";
        double cachedPercentValue = -1.0;
    };

    static void onModify(auto& self);

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
    void updateProgressbar();

    float getScreenWidth();
    float getAvailableWidth();
    double getActualProgress();
    std::vector<float> getCharacterAdvances(CCLabelBMFont* label);
};