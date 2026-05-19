#pragma once

#include "point_cloud_type.h"

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace pointcloud 
{

class PointCloud 
{
public:
    PointCloud() = default;

    explicit PointCloud(PointCloudType type, std::size_t points_count) : type_(type), points_count_(points_count)
    {
        const std::size_t ps = PointSize();
        data_.resize(points_count_ * ps);
    }

    PointCloudType Type() const noexcept { return type_; }
    std::size_t Size() const noexcept { return points_count_; }

    std::size_t PointSize() const noexcept 
    {
        switch (type_) 
        {
            case PointCloudType::XYZIR:    
                return PointTraits<PointCloudType::XYZIR>::kPointSize;
            case PointCloudType::XYZIRDAT: 
                return PointTraits<PointCloudType::XYZIRDAT>::kPointSize;
        }
        return 0;
    }

    const double* RawData() const noexcept { return data_.data(); }
    double* RawData() noexcept { return data_.data(); }
    std::size_t RawDataSize() const noexcept { return data_.size(); }

    const double* Point(std::size_t index) const 
    {
        CheckBounds(index);
        return data_.data() + index * PointSize();
    }

    double* Point(std::size_t index) 
    {
        CheckBounds(index);
        return data_.data() + index * PointSize();
    }

    double X(std::size_t index) const { return Point(index)[0]; }
    double Y(std::size_t index) const { return Point(index)[1]; }
    double Z(std::size_t index) const { return Point(index)[2]; }

    void SetPoint(std::size_t index, const double* point, std::size_t point_size) 
    {
        CheckBounds(index);
        const std::size_t ps = PointSize();
        if (point_size != ps) 
        {
            throw std::runtime_error(
                "Point size mismatch: expected " + std::to_string(ps) +
                ", got " + std::to_string(point_size));
        }
        for (std::size_t i = 0; i < ps; ++i)
        {
            data_[index * ps + i] = point[i];
        }
    }

    void AssignData(std::vector<double>&& data, std::size_t new_points_count) 
    {
        data_ = std::move(data);
        points_count_ = new_points_count;
    }

    const std::vector<double>& DataVector() const noexcept { return data_; }

    PointCloud CloneMetadata() const 
    {
        PointCloud pc;
        pc.type_ = type_;
        pc.points_count_ = 0;
        return pc;
    }

private:
    void CheckBounds(std::size_t index) const 
    {
        if (index >= points_count_)
            throw std::out_of_range("Point index out of range: " + std::to_string(index));
    }

    PointCloudType type_{};
    std::size_t points_count_ = 0;
    std::vector<double> data_;
};

inline PointCloud MakePointCloud(PointCloudType type, std::size_t points_count,
    const std::vector<double>& data) 
{
    const std::size_t ps = (type == PointCloudType::XYZIR)
                           ? PointTraits<PointCloudType::XYZIR>::kPointSize
                           : PointTraits<PointCloudType::XYZIRDAT>::kPointSize;

    if (data.size() < points_count * ps) 
        throw std::runtime_error("Not enough data to fill point cloud");

    PointCloud pc(type, points_count);
    for (std::size_t i = 0; i < points_count; ++i) 
    {
        pc.SetPoint(i, data.data() + ps * i, ps);
    }
    return pc;
}

}
