#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>

namespace pointcloud 
{

enum class PointCloudType 
{
    XYZIR,
    XYZIRDAT
};

inline std::string ToString(PointCloudType type) 
{
    switch (type) 
    {
        case PointCloudType::XYZIR:     
            return "XYZIR";
        case PointCloudType::XYZIRDAT:  
            return "XYZIRDAT";
    }
    return "Unknown";
}

inline PointCloudType ParsePointCloudType(const std::string& str) 
{
    if (str == "XYZIR")     
        return PointCloudType::XYZIR;
    if (str == "XYZIRDAT")  
        return PointCloudType::XYZIRDAT;
    throw std::runtime_error("Unknown pointcloud type: " + str);
}

template <PointCloudType Type>
struct PointTraits;

template <>
struct PointTraits<PointCloudType::XYZIR> 
{
    static constexpr std::size_t kPointSize = 5;
    static constexpr std::size_t kX = 0;
    static constexpr std::size_t kY = 1;
    static constexpr std::size_t kZ = 2;
    static constexpr std::size_t kIntensity = 3;
    static constexpr std::size_t kRing = 4;
    static constexpr bool kHasDistance = false;
    static constexpr bool kHasAzimuth = false;
};

template <>
struct PointTraits<PointCloudType::XYZIRDAT> 
{
    static constexpr std::size_t kPointSize = 8;
    static constexpr std::size_t kX = 0;
    static constexpr std::size_t kY = 1;
    static constexpr std::size_t kZ = 2;
    static constexpr std::size_t kIntensity = 3;
    static constexpr std::size_t kRing = 4;
    static constexpr std::size_t kDistance = 5;
    static constexpr std::size_t kAzimuth = 6;
    static constexpr std::size_t kTimestamp = 7;
    static constexpr bool kHasDistance = true;
    static constexpr bool kHasAzimuth = true;
};

}
