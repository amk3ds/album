/**
 * @file Photo.hpp
 * @author asmodak
 * @brief Declaration of an Abstract HashStratergy class
 * @version 0.1
 * @date 2021-02-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <cstdint>
#include <array>
#include <numeric>
#include <bitset>

#include "Photo.hpp"

/**
 * @brief A-HashStratergy class implementation.
 * @tparam _T DataType of Photo class pixel data.
 */
template <typename _T = int16_t>
class AHashStrategy
{
private:
  
    /**
     * @brief PhotoLite struct implementation.
     * @tparam _W Scaled Photo class height.
     * @tparam _H Scaled Photo class height.
     * @tparam _T DataType of Scaled Photo class pixel data.
     */
    template <uint64_t _W = 8, uint64_t _H = 8>
    struct PhotoLite
    {        
        constexpr static auto wid() noexcept { return _W; }
        constexpr static auto hei() noexcept { return _H; }
        using array_type = std::array<_T, wid()*hei()>;
        array_type pixels_ {};
    };

public:
    using value_type = _T;
    using pointer_type = Photo<_T>*;

    /**
     * @brief method that computes a hash value with the a-hash algorithm
     * @return hashkey - a 64 bit integer
     */
    uint64_t generate(pointer_type photoPtr)
    {
        if (!photoPtr) return uint64_t{};
        PhotoLite phl;
        uint64_t length {static_cast<uint64_t>(CHANNEL_WIDTH<value_type> * phl.wid() * phl.hei())};
        uint64_t start { 0 }, finish { length };
        for (auto& pixel : phl.pixels_)
        {
          auto begin { photoPtr->begin()+start };
          auto end { photoPtr->begin()+finish };
          pixel = std::accumulate(begin, end, uint64_t{0}) / double(length);
          start += length, finish += length;
        }
        value_type average = std::accumulate( phl.pixels_.begin(), phl.pixels_.end(), uint64_t{0}) / phl.pixels_.size();
        uint64_t itr { 0 };
        auto isgreater = [&average](const value_type& pixel) {return pixel > average;};
        std::bitset<phl.pixels_.size()> bitset;
        for (const auto& pixel: phl.pixels_)
        {
            bitset.set(itr++, isgreater(pixel));
        }
        return uint64_t{bitset.to_ullong()};
    }
};
