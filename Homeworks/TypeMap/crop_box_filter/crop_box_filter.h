#pragma once

#include "filter.h"
#include "point_cloud.h"

#include <memory>
#include <vector>

namespace pointcloud 
{
namespace preprocessor 
{

struct CropBoxParams 
{
    double min_x = 0.0;
    double max_x = 0.0;
    double min_y = 0.0;
    double max_y = 0.0;
    double min_z = 0.0;
    double max_z = 0.0;
    bool negative = false;

    static CropBoxParams FromFilterParams(const FilterParams& fp) 
    {
        CropBoxParams p;
        p.min_x = fp.Get("min_x", p.min_x);
        p.max_x = fp.Get("max_x", p.max_x);
        p.min_y = fp.Get("min_y", p.min_y);
        p.max_y = fp.Get("max_y", p.max_y);
        p.min_z = fp.Get("min_z", p.min_z);
        p.max_z = fp.Get("max_z", p.max_z);
        p.negative = static_cast<bool>(fp.Get("negative", p.negative ? 1.0 : 0.0));
        return p;
    }

    bool operator==(const CropBoxParams& other) const noexcept 
    {
        return min_x == other.min_x && max_x == other.max_x &&
               min_y == other.min_y && max_y == other.max_y &&
               min_z == other.min_z && max_z == other.max_z &&
               negative == other.negative;
    }

    bool operator!=(const CropBoxParams& other) const noexcept 
    {
        return !(*this == other);
    }
};

class CropBoxFilter : public Filter<CropBoxFilter> 
{
public:
    CropBoxFilter() : Filter("CropBoxFilter") {}
    std::unique_ptr<PointCloud> ApplyImpl(PointCloud* pc);
    void SetParamsImpl(const FilterParams& param);

private:
    CropBoxParams box_params_;
};

}
}
