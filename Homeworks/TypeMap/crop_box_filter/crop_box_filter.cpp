#include "crop_box_filter.h"

#include <cmath>
#include <vector>

namespace pointcloud 
{
namespace preprocessor 
{

std::unique_ptr<PointCloud> CropBoxFilter::ApplyImpl(PointCloud* pc) 
{
    const std::size_t point_size = pc->PointSize();
    const std::size_t total_points = pc->Size();

    std::vector<double> output;
    output.reserve(total_points * point_size);

    std::size_t output_points_count = 0;
    bool nan_logged = false;

    for (std::size_t i = 0; i < total_points; ++i) 
    {
        const double* point = pc->Point(i);
        double x = point[0];
        double y = point[1];
        double z = point[2];

        if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) 
        {
            if (!nan_logged) 
            {
                logger_.log("Ignoring points containing NaN/infinite values "
                           "(further such points will be silently skipped)");
                nan_logged = true;
            }
            continue;
        }

        bool inside = (x > box_params_.min_x && x < box_params_.max_x &&
                       y > box_params_.min_y && y < box_params_.max_y &&
                       z > box_params_.min_z && z < box_params_.max_z);

        bool keep = box_params_.negative ? !inside : inside;
        if (keep) 
        {
            for (std::size_t j = 0; j < point_size; ++j) 
            {
                output.push_back(point[j]);
            }
            ++output_points_count;
        }
    }

    auto result = std::make_unique<PointCloud>(pc->CloneMetadata());
    result->AssignData(std::move(output), output_points_count);
    return result;
}

void CropBoxFilter::SetParamsImpl(const FilterParams& param) 
{
    auto new_params = CropBoxParams::FromFilterParams(param);

    if (box_params_ != new_params) 
    {
        logger_.log("[paramCallback] Setting the minimum point to: " +
                    std::to_string(new_params.min_x) + " " +
                    std::to_string(new_params.min_y) + " " +
                    std::to_string(new_params.min_z));
        logger_.log("[paramCallback] Setting the maximum point to: " +
                    std::to_string(new_params.max_x) + " " +
                    std::to_string(new_params.max_y) + " " +
                    std::to_string(new_params.max_z));
        logger_.log("[paramCallback] Setting the filter negative flag to: " +
                    std::string(new_params.negative ? "true" : "false"));
        box_params_ = new_params;
    }

    params_ = param;
}

}
} 
