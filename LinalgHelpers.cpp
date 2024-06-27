//
// Created by jakob on 23.06.24.
//


#include <cassert>
#include <cmath>
#include <algorithm>
#include "LinalgHelpers.h"
#include "DataCover.h"

#include <ostream>

namespace Mapper {
Scalar euclididan_distance(Vector const &vec1, Vector const &vec2) {
    assert(vec1.size() == vec2.size());
    Scalar squared_sum = 0;
    for(size_t i = 0; i < vec1.size(); i++) {
        squared_sum += (vec1[i]-vec2[i])*(vec1[i]-vec2[i]);
    }
    return std::sqrt(squared_sum);
}

Scalar maximum_distance(Vector const &vec1, Vector const &vec2)
{
    assert(vec1.size() == vec2.size());
    Scalar max_distance = 0;
    for(size_t i = 0; i < vec1.size(); i++) {
        max_distance = std::max(max_distance, std::abs(vec1[i]-vec2[i]));
    }
    return max_distance;
}

bool check_data_equal_dimension(Matrix const &mat)
{
    bool is_equal = true;
    std::ranges::for_each(mat, [&](Vector const& v) {
        is_equal = is_equal and (v.size() == mat[0].size());
    });
    return is_equal;
}

size_t get_data_dimension(Matrix const &mat)
{
    return mat[0].size();
}



} // Helper
std::ostream & operator<<(std::ostream &os, Mapper::Vector const &vec)
{
    os << "[";
    for(auto const x : vec) {
        os << x << ", ";
    }
    os << "\b\b]";
    return os;
}

std::ostream & operator<<(std::ostream &os, Mapper::Matrix const &mat)
{
    os << "[";
    for(auto const& row : mat) {
        os << row << ", ";
    }
    os << "\b\b]";
    return os;
}
