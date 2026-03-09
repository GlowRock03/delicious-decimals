#include "Utility.hpp"

#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <cmath>

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

    constexpr size_t MIN_PRECISION = 15;
    size_t precisionDecimals = std::min(decimalLength, MIN_PRECISION);

    if (std::remainder(value, 1.f) == 0) return padWithZeros(base, decimalLength);

    std::string result = base;

    size_t dot = result.find('.');
    if (dot == std::string::npos) {

        result += '.';
        dot = result.size() - 1;
    }

    size_t currentDecimals = result.size() - dot - 1;

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 9);

    for (size_t i = currentDecimals; i < decimalLength; i++) {

        result += char('0' + dist(rng));
    }

    if (decimalLength > precisionDecimals && result[dot + 1 + precisionDecimals] == '0') {

        result[dot + 1 + precisionDecimals] = char('1' + dist(rng) % 9);
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