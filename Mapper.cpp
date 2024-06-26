//
// Created by jakob on 26.06.24.
//

#include "Mapper.h"

namespace Mapper {
Matrix Mapper::project(Matrix const &data, std::vector<size_t> const &dimensions)
{
    Matrix result;
    result.reserve(data.size());
    for(auto const& point : data) {
        Vector vec;
        for (auto const dim : dimensions) {
            vec.push_back(point[dim]);
        }
        result.push_back(vec);
    }
    return result;
}
} // Mapper