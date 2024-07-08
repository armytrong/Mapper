//
// Created by jakob on 23.06.24.
//

#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <vector>

namespace MapperLib{

using Scalar = double;
using Vector = std::vector<Scalar>;
using Matrix = std::vector<std::vector<Scalar>>;
using PointId = size_t;
using Dimension = size_t;
using SimplexId = size_t;

struct Simplex{
    std::vector<PointId> points;
    [[nodiscard]] Dimension dimension() const { return points.size() - 1;}
    [[nodiscard]] size_t num_nodes() const { return points.size(); }
    [[nodiscard]] PointId operator[](size_t index) const {return points[index]; }
};
}


#endif //TYPEDEFS_H
