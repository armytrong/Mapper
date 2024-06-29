//
// Created by jakob on 27.06.2024.
//

#include "Projection.h"

namespace Mapper {
CoordinatePlaneProjection::CoordinatePlaneProjection(std::vector<Dimension> dimensions) : _dimensions(std::move
(dimensions)){}

Matrix CoordinatePlaneProjection::project(const Matrix &data) const {
    Matrix result;
    result.reserve(data.size());
    for(auto const& point : data) {
        Vector vec;
        for (auto const dim : _dimensions) {
            vec.push_back(point[dim]);
        }
        result.push_back(vec);
    }
    return result;
}
} // Mapper