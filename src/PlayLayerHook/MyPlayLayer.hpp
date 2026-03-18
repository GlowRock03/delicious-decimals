#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/binding/GameManager.hpp>

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

        float lineHeight = 15.f;
        size_t cachedMaxDecimals = 0;
        bool maxDecimalsComputed = false;
        bool computedWithProgressbar = true;

        double cachedPercentRounded = -1.0;
        size_t cachedDecimalLength = 0;

        const int MAX_DECIMALS = 2000;

        size_t currentDeviationDecimals = 0;
        float deviationAccumulator = 0.f; 
    };

    template <typename T>
    static void onModify(T& self) {
        if (!self.setHookPriorityPost("PlayLayer::updateProgressbar", Priority::Last)) {
            geode::log::warn("Failed to set hook priority");
        }
    }

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
    void updateProgressbar();

    float getScreenWidth();
    float getScreenHeight();
    float getAvailableWidth();
    double getActualProgress();
    std::vector<float> getCharacterAdvances(CCLabelBMFont* label);

    size_t getMaxDecimals();
    size_t getDynamicDecimals(size_t decimalLength);
};