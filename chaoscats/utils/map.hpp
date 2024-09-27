#pragma once

#include <map>
#include <unordered_map>

template <typename Map, typename Key>
inline bool mapContains(const Map &map, const Key &element)
{
    return map.find(element) == map.end();
}