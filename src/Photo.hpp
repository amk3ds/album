/**
 * @file Photo.hpp
 * @author asmodak
 * @brief Implementation of a Photo class
 * @version 0.1
 * @date 2021-02-15
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <utility>

 /**
  * @brief Width of the color channel
  */
template<class _T = int16_t>
inline constexpr _T CHANNEL_WIDTH {3};

/**
 * @brief Photo class
 * @tparam _T DataType of pixel data.
 */
template <typename _T = int16_t>
class Photo
{
private:

    /**
     * @brief Photo filename on the disk.
     */
    std::string filename_;

    /**
     * @brief Photo dimensions (width and height).
     */
    uint64_t wid{}, hei{};

    /**
     * @brief Photo's RGB pixel data
     * Data is stored contiguously for the entire photo
     */
    std::vector<_T> pixels_;

public:

    using value_type = _T;

    using internal_pixels_t = decltype(pixels_);
    
    /**
     * @brief Constructor for Photo class
     *
     * @param inputfile Input file path.
     */
    explicit Photo(std::string inputfile) : filename_(std::move(inputfile))
    {
        std::ifstream ifs;
        ifs.open(filename_.c_str(), std::ios::binary);
        if (ifs.fail()) 
            throw std::runtime_error("Failed to open file: " + filename_);
        std::string header;
        uint64_t w, h, b;
        ifs >> header;
        if (header != std::string{"P6"})
            throw std::runtime_error("Unable to read file:" + filename_);
        ifs >> w >> h >> b;
        wid = w;
        hei = h;
        uint64_t dimensions {wid * hei};
        pixels_.reserve(CHANNEL_WIDTH<value_type> * dimensions);
        ifs.ignore(256, '\n');
        unsigned char pixel[CHANNEL_WIDTH<value_type>];
        for (uint64_t i {0}; i < dimensions; ++i) 
        {
            ifs.read(reinterpret_cast<char *>(pixel), CHANNEL_WIDTH<value_type>);
            for (uint64_t j {0}; j < CHANNEL_WIDTH<value_type>; ++j)
                pixels_.push_back(static_cast<value_type>(pixel[j]));
        }
        ifs.close();
    }

    /**
     * @brief Returns the width of the photooto, in pixels_
     * @return the width of the photo
     */
    uint64_t GetWidth() const noexcept {return wid;}

    /**
     * @brief Returns the height of the photooto, in pixels_
     * @return the width of the photo
     */
    uint64_t GetHeight() const noexcept {return hei;}

    /**
     * @brief Returns the name of the file as a string
     * @return the file name of the photo
     */
    std::string GetFileName() const noexcept {return filename_;}

    /**
     * @brief Returns a pointer to the start of the photo's RGB data
     * @return the pointer to the raw data of the photooto
     */
    value_type* GetData() const noexcept {return pixels_.data();}

    /**
     * @brief Saves the photo object data to disk
     * @param outputfile Output file name.
     */
    void save(std::string outputfile) 
    { 
        if (wid != 0 && hei != 0)
        {
            std::ofstream ofs; 
            ofs.open(outputfile, std::ios::binary);
            if (ofs.fail())
                throw std::runtime_error("Failed to open output file: " + outputfile);
            ofs << "P6\n" << wid << " " << hei << "\n255\n";
            uint64_t dimensions {wid * hei};
            for (uint64_t i {0}; i < dimensions; ++i)
            {
                for (uint64_t j {0}; j < CHANNEL_WIDTH<value_type>; ++j)
                    ofs << static_cast<unsigned char>(pixels_[CHANNEL_WIDTH<value_type> * i + j]);
            }
            ofs.close();
        }
    }

    /**
     * @brief get iterator to the start of the pixels_
     *
     * @return iterator
     */
    constexpr typename internal_pixels_t::iterator begin() noexcept
    {
        return pixels_.begin();
    }

    /**
     * @brief get iterator to beyond-the-end of the pixels_
     *
     * @return iterator
     */
    constexpr typename internal_pixels_t::iterator end() noexcept
    {
        return pixels_.end();
    }

    /**
     * @brief get the computed hash key from the hashing algorithm.
     *
     * @return 64-bit integer hash
     */
    template <typename StratergyLike>
    auto hashKey(StratergyLike &hs) -> decltype(hs.generate(std::declval<typename StratergyLike::pointer_type>()), uint64_t())
    {
        return hs.generate(this);
    }

    /**
     * @brief Check if Photos are equal
     * @param i_lhs First Photo
     * @param i_rhs Second Photo
     * @return true if photos are equal
     */
    friend constexpr bool operator==(const Photo& i_lhs, const Photo<value_type>& i_rhs) noexcept
    {
        if (i_lhs.GetWidth() != i_rhs.GetWidth())    return false;
        if (i_lhs.GetHeight() != i_rhs.GetHeight())  return false;
        return i_lhs.pixels_ == i_rhs.pixels_;
    }
};
