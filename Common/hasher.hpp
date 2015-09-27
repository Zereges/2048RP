#pragma once
#include <string>
#include <algorithm>

/**!
    \ingroup common
    \brief Class providing hashing function used in password hashing.

    The sipher used is simple ceasar sipher with right shift of \ref SHIFT. Somehing like
    SHA-x would be more useful, but for testing purposes, it suffices.
    \sa client::login
*/
class hasher
{
    public:
        //! Hashes given input.
        //! \param input string to hash.
        //! \return hashed string
        static std::string hash(const std::string& input)
        {
            std::string result = input;
            std::for_each(result.begin(), result.end(), [](char& c) { c += SHIFT; });
            return std::move(result);
        }

    private:
        const static int SHIFT = +5; //!< Caesar sipher shift.
};
