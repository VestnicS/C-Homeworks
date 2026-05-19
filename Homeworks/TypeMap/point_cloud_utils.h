#pragma once

#include "point_cloud.h"

#include <cmath>
#include <stdexcept>

namespace pointcloud 
{
namespace utils 
{

template <PointCloudType Type>
double GetDistance(const PointCloud& pc, std::size_t index) 
{
    using Traits = PointTraits<Type>;
    if constexpr (Traits::kHasDistance) 
        return pc.Point(index)[Traits::kDistance];
    else 
    {
        double x = pc.X(index);
        double y = pc.Y(index);
        double z = pc.Z(index);
        return std::sqrt(x * x + y * y + z * z);
    }
}

template <PointCloudType Type>
double GetAzimuth(const PointCloud& pc, std::size_t index) 
{
    using Traits = PointTraits<Type>;
    if constexpr (Traits::kHasAzimuth)
        return pc.Point(index)[Traits::kAzimuth];
    else
    {
        double x = pc.X(index);
        double y = pc.Y(index);
        return std::atan2(y, x);
    }
}

inline double GetDistance(const PointCloud& pc, std::size_t index) 
{
    switch (pc.Type()) 
    {
        case PointCloudType::XYZIR:    
            return GetDistance<PointCloudType::XYZIR>(pc, index);
        case PointCloudType::XYZIRDAT: 
            return GetDistance<PointCloudType::XYZIRDAT>(pc, index);
    }
    throw std::runtime_error("Unsupported point cloud type");
}

inline double GetAzimuth(const PointCloud& pc, std::size_t index) 
{
    switch (pc.Type()) 
    {
        case PointCloudType::XYZIR:    
            return GetAzimuth<PointCloudType::XYZIR>(pc, index);
        case PointCloudType::XYZIRDAT: 
            return GetAzimuth<PointCloudType::XYZIRDAT>(pc, index);
    }
    throw std::runtime_error("Unsupported point cloud type");
}

}
}
