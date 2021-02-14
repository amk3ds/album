/**
 * @file Collection.hpp
 * @author asmodak
 * @brief Implementation of a collection class
 * @version 0.1
 * @date 2021-02-14
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once

#include <cstdint>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <cassert>

#include "AHashStratergy.hpp"
#include "Photo.hpp"

 /**
  * @brief Collection class
  * @tparam _T DataType of photo object pixel data.
  * @tparam _HS Hashing algorithm for photo object.
  */
template <typename _T = int16_t, typename _HS = AHashStrategy<_T>>
class Collection 
{
public:
    
    using stratergy_type = _HS;

    using value_type = Photo<_T>;

    using reference_type = value_type&;

    using raw_ptr_type = value_type*;

    using unique_ptr_type = std::unique_ptr<value_type>;

    using vector_type = std::vector<unique_ptr_type>;

    using list_type = std::list<raw_ptr_type>;

    using map_type = std::unordered_map<uint64_t, list_type>;

private:

    /**
     * @brief Vector storing photo objects for quick access.
     */
    vector_type photoVec_ {};
    /**
     * @brief Vector storing photo objects for quick access.
     */
    map_type photoMap_ {};

    /**
     * @brief stratergy object based on photo hashing algorithm.
     */
    stratergy_type stratergy_ {};

public:

    /**
     * @brief Gets the number of Photos in the collection)
     * @return the number of photos in the collection
     */
	uint64_t albumSize() 
    {
        return static_cast<uint64_t>(photoVec_.size());
    }

	/**
     * @brief Get Photo object stored at index
     * @param given index in the collection
     * @return reference to photo object for the given index in the collection
     */
    reference_type GetPhoto(uint64_t index) 
    {
        assert(photoVec_.at(static_cast<size_t>(index)) != nullptr);
        return *photoVec_.at(static_cast<size_t>(index));
    }

    /**
     * @brief Adds Photo object to the collection. 
     * @param given index in the collection
     * @return true if the Photo was added to the collection
     */
    bool AddPhoto(std::string filename) 
    {
        auto photo { std::make_unique<value_type>(filename) };
        if (photo == nullptr)  return false;
        uint64_t key { photo->hashKey(stratergy_) };
        auto it { photoMap_.find(key) };
        if (it == photoMap_.end())
        {
            photoVec_.push_back(std::move(photo));
            list_type list { photoVec_.back().get() };
            photoMap_[key] = list;
            return true;
        }
        for (auto const& p : it->second)
        {
            if (p && *p == *photo) 
                return false;
        }
        photoVec_.push_back(std::move(photo));
        it->second.push_back(photoVec_.back().get());
        return true;
    }
};
