#include "Utility.hpp"

#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <cmath>
#include <cstdint>

std::vector<std::string> Utility::wrapTextByAdvances(const std::string& text, const std::vector<float>& advances, float firstLineWidth, float otherLineWidth) {
    
    std::vector<std::string> lines;

    if (text.empty()) return lines;

    size_t count = std::min(text.size(), advances.size());
    size_t lineStart = 0;

    float cursor = 0.f;
    float limit = firstLineWidth;

    for (size_t i = 0; i < count; i++) {

        if (cursor + advances[i] > limit && i > lineStart) {

            size_t start = std::min(lineStart, text.size());
            size_t len = std::min(i - lineStart, text.size() - start);

            lines.push_back(text.substr(start, len));

            lineStart = i;
            cursor = 0.f;
            limit = otherLineWidth;
        }

        cursor += advances[i];
    }

    if (lineStart < text.size()) {

        lines.push_back(text.substr(lineStart));
    }

    return lines;
}

std::string Utility::formatWithDecimals(double value, size_t decimalLength) {

    constexpr size_t MIN_PRECISION = 15;
    size_t precisionDecimals = std::min(decimalLength, MIN_PRECISION);

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precisionDecimals) << value;
    std::string base = ss.str();

    size_t dot = base.find('.');
    if (dot == std::string::npos) {

        base += '.';
        dot = base.size() - 1;
    }

    size_t currentDecimals = base.size() - dot - 1;

    if (currentDecimals < precisionDecimals) {

        base += std::string(precisionDecimals - currentDecimals, '0');
    }

    return base;
}

std::string Utility::addFakeDecimals(const std::string& base, size_t decimalLength, double value) {

    if (std::remainder(value, 1.0) == 0.0) {
        return padWithZeros(base, decimalLength);
    }

    std::string result = base;
    size_t dot = result.find('.');
    if (dot == std::string::npos) {
        result += '.';
        dot = result.size() - 1;
    }

    size_t currentDecimals = result.size() - dot - 1;

    auto deterministicDigit = [value](size_t index) -> char {

        std::uint64_t seed = static_cast<std::uint64_t>(std::llround(value * 1e6)) + (index * 2654435761u);
        seed ^= (seed >> 13);
        seed *= 0x5DEECE66Dull;
        seed ^= (seed >> 17);
        seed = seed % 10;
        return '0' + static_cast<char>(seed);
    };

    for (size_t i = currentDecimals; i < decimalLength; i++) {
        result += deterministicDigit(i);
    }

    return result;
}

std::string Utility::padWithZeros(const std::string& base, size_t decimals) {

    std::string result = base;

    size_t dot = result.find('.');
    if (dot == std::string::npos) {

        result += '.';
        dot = result.size() - 1;
    }

    size_t currentDecimals = result.size() - dot - 1;

    if (currentDecimals < decimals) {

        result += std::string(decimals - currentDecimals, '0');
    }

    return result;
}