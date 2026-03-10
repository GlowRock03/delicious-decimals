#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Utility {
public:

    static std::vector<std::string> wrapTextByAdvances(const std::string& text, const std::vector<float>& advances, float firstLineWidth, float otherLineWidth);

    static std::string formatWithDecimals(double value, size_t decimalLength);
    static std::string addFakeDecimals(const std::string& base, size_t decimalLength, double value);
    static std::string padWithZeros(const std::string& base, size_t decimals);
};