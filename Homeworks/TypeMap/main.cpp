#include "point_cloud.h"
#include "crop_box_filter.h"

#include <iostream>
#include <string>
#include <vector>

std::ostream& operator<<(std::ostream& stream, const pointcloud::PointCloud& pc) 
{
    stream << "Point cloud (" << pointcloud::ToString(pc.Type()) << "): " << std::endl;
    for (std::size_t i = 0; i < pc.Size(); ++i) 
    {
        const double* point = pc.Point(i);
        for (std::size_t j = 0; j < pc.PointSize(); ++j) 
        {
            stream << point[j] << " ";
        }
        stream << std::endl;
    }
    return stream;
}

int main() 
{
    try 
    {
        auto pc_xyzir = pointcloud::MakePointCloud(
            pointcloud::PointCloudType::XYZIR, 3,
            { 5.0, 1.2, 2.1, 0.5, 1.0,
             -3.2, 0.2, 1.1, 0.7, 1.0,
              2.2, 2.1, 7.0, 0.1, 2.0 });

        std::cout << "We have: " << pc_xyzir << std::endl;

        pointcloud::preprocessor::CropBoxFilter cbf;
        cbf.SetParams(pointcloud::preprocessor::FilterParams({
            {"min_x", -4.0}, {"max_x", -2.0},
            {"min_y", 0.1},  {"max_y", 2.0},
            {"min_z", 0.5},  {"max_z", 3.0},
        }));

        auto out = cbf.Apply(&pc_xyzir);
        if (!out) 
        {
            std::cout << "Result is null!" << std::endl;
            return 1;
        }
        std::cout << "after CBF: " << *out << std::endl;

    } 
    catch (const std::exception& e) 
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
