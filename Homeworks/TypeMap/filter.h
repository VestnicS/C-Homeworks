#pragma once

#include "Logger.h"
#include "point_cloud.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace pointcloud 
{
namespace preprocessor 
{

class FilterParams 
{
public:
    FilterParams() = default;
    explicit FilterParams(std::unordered_map<std::string, double> params): params_(std::move(params)) {}

    void Set(const std::string& name, double value) 
    {
        params_[name] = value;
    }

    double Get(const std::string& name) const 
    {
        return params_.at(name);
    }

    double Get(const std::string& name, double default_val) const noexcept 
    {
        auto it = params_.find(name);
        return (it != params_.end()) ? it->second : default_val;
    }

    bool Has(const std::string& name) const noexcept 
    {
        return params_.find(name) != params_.end();
    }

private:
    std::unordered_map<std::string, double> params_;
};

template <typename Derived>
class Filter 
{
public:
    explicit Filter(std::string filter_name = "pointcloud_preprocessor_filter")
        : filter_name_(std::move(filter_name))
        , logger_(filter_name_) {}

    ~Filter() = default;

    Filter(const Filter&) = delete;
    Filter& operator=(const Filter&) = delete;
    Filter(Filter&&) = delete;
    Filter& operator=(Filter&&) = delete;

    const std::string& FilterName() const noexcept { return filter_name_; }

    std::unique_ptr<PointCloud> Apply(PointCloud* pc) 
    {
        return static_cast<Derived*>(this)->ApplyImpl(pc);
    }

    void SetParams(const FilterParams& params) {
        static_cast<Derived*>(this)->SetParamsImpl(params);
    }

    const FilterParams& Params() const noexcept { return params_; }

protected:
    std::string filter_name_;
    Logger logger_;
    FilterParams params_;
};

}
}
