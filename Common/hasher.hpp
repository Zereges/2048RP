#pragma once
#include <string>
#include <algorithm>

class hasher // caesar sipher with right shift of 5. Something like SHA-x would be more usefull, but for testing purposes, it suffices.
{
    public:
        static std::string hash(const std::string& input)
        {
            std::string result = input;
            std::for_each(result.begin(), result.end(), [](char& c) { c += SHIFT; });
            return std::move(result);
        }

    private:
        const static int SHIFT = +5;
};
