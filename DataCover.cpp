//
// Created by jakob on 24.06.24.
//

#include "DataCover.h"

#include <cassert>
#include <algorithm>
#include <cmath>

namespace Mapper {
DataCover::DataCover(
    unsigned const resolution,
    double const perc_overlap,
    Matrix const& data,
    std::optional<Vector> minima,
    std::optional<Vector> maxima
):
    _resolution(resolution),
    _perc_overlap(perc_overlap),
    _data(data),
    _data_dimension(Helper::get_data_dimension(data))
{
    assert(Helper::check_data_equal_dimension(data));
    assert(perc_overlap <= 0.5);
    if(minima.has_value() and minima.value().size() == _data_dimension) {
        _minima = std::move(minima.value());
    } else {
        initialize_minima_from_data();
    }
    if(maxima.has_value() and minima.value().size() == _data_dimension) {
        _maxima = std::move(maxima.value());
    } else {
        initialize_maxima_from_data();
    }
}

DataCover::CubeId DataCover::get_native_cube_id(Vector const& vec) const
{
    CubeId result(_data_dimension);
    for(size_t dim = 0; dim < _data_dimension; dim++) {
        Scalar const cube_size_in_dim = (_maxima[dim] - _minima[dim])/_resolution;
        result[dim] = std::floor((vec[dim]-_minima[dim])/cube_size_in_dim);
    }
    return result;
}

std::vector<PointId> DataCover::get_points_in_cube(LinearCubeId const cube_id) const
{
    if(not _cube_cache.has_value()) {
        initialize_cube_cache();
    }
    return _cube_cache.value()[cube_id];
}

DataCover::LinearCubeId DataCover::convert_to_linear_cube_id(CubeId const &cube_id) const
{
    LinearCubeId result = 0;
    for (size_t dim = 0; dim < _data_dimension; dim++) {
        result += cube_id[dim]*static_cast<LinearCubeId>(std::pow(_resolution,dim));
    }
    return result;
}

DataCover::CubeId DataCover::convert_to_cube_id(LinearCubeId const linear_id) const
{
    CubeId result(_data_dimension);
    for (size_t dim = 0; dim < _data_dimension; dim++) {
        result[dim] = linear_id / static_cast<LinearCubeId>(std::pow(_resolution,dim)) % _resolution; // TODO: is this correct?
    }
    return result;
}

void DataCover::initialize_cube_cache() const
{
    _cube_cache = std::vector<std::vector<PointId>>(get_total_num_cubes());
    for (size_t i = 0; i < _data.size(); i++) {
        auto const cubes = get_parent_cubes(_data[i]);
        std::ranges::for_each(cubes, [&](CubeId const& cube_id){_cube_cache.value()[convert_to_linear_cube_id(cube_id)].push_back(i);});
    }
}

std::vector<DataCover::CubeId> DataCover::get_parent_cubes(Vector const &v) const
{
    auto const native_cube = get_native_cube_id(v);
    auto const neighbor_cubes = get_neighbor_cubes(native_cube);
    std::vector<CubeId> parent_cubes;
    for (auto const& cube : neighbor_cubes) {
        if(is_vector_in_cube(v, cube)){parent_cubes.push_back(cube);}
    }
    return parent_cubes;
}

bool DataCover::is_vector_in_cube(Vector const &vec, CubeId const &cube_id) const
{
    auto const cube_center = get_cube_center(cube_id);
    for (size_t dim = 0; dim < _data_dimension; dim++) {
        auto const cube_size_in_dim = (_maxima[dim] - _minima[dim])/_resolution;
        if(std::abs(vec[dim] - cube_center[dim]) > (0.5+_perc_overlap)*cube_size_in_dim) {
            return false;
        }
    }
    return true;
}

size_t DataCover::get_total_num_cubes() const
{
    return static_cast<size_t>(std::pow(_resolution, _data_dimension));
}

Scalar DataCover::get_data_min_in_dimension(size_t const dimension) const
{
    auto min = std::numeric_limits<Scalar>::max();
    std::ranges::for_each(_data, [&](Vector const& v) {
        if (v[dimension] < min) {
            min = v[dimension];
        }
    });
    return min;
}

Scalar DataCover::get_data_max_in_dimension(size_t const dimension) const
{
    auto max = std::numeric_limits<Scalar>::min();
    std::ranges::for_each(_data, [&](Vector const& v) {
        if (v[dimension] > max) {
            max = v[dimension];
        }
    });
    return max;
}

void DataCover::initialize_minima_from_data()
{
    _minima.resize(_data_dimension);
    for(size_t i = 0; i < _data_dimension; i++) {
        _minima[i] = get_data_min_in_dimension(i);
    }
}


void DataCover::initialize_maxima_from_data()
{
    _maxima.resize(_data_dimension);
    for(size_t i = 0; i < _data_dimension; i++) {
        _maxima[i] = get_data_max_in_dimension(i);
    }
}

Vector DataCover::get_cube_center(CubeId const &cube_id) const
{
    Vector result(_data_dimension);
    for (size_t dim = 0; dim < _data_dimension; dim++) {
        Scalar const cube_size_in_dim = (_maxima[dim] - _minima[dim])/_resolution;
        result[dim] = (static_cast<double>(cube_id[dim]) + 0.5) * cube_size_in_dim;
    }
    return result;
}

std::vector<DataCover::CubeId> DataCover::get_neighbor_cubes(CubeId const &cube_id) const
{
    std::vector<CubeId> neighbors;
    auto const num_neighbors = static_cast<size_t>(std::pow(3, _data_dimension));
    for(size_t i = 0; i < num_neighbors; i++) {
        auto neighbor = cube_id;
        for (size_t dim = 0; dim < _data_dimension; dim++) {
            size_t const dimension_modifier = - 1 + dim / static_cast<size_t>(std::pow(3,dim)) % 3;
            neighbor[dim] += dimension_modifier;
        }
        neighbors.push_back(neighbor);
    }
    return neighbors;
}

} // Mapper
std::ostream & operator<<(std::ostream &stream, Mapper::DataCover::CubeId const& vec)
{
    stream << "[";
    for(auto const x : vec) {
        stream << x << ", ";
    }
    stream << "\b\b]";
    return stream;
}
