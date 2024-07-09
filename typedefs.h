//
// Created by jakob on 23.06.24.
//

#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <vector>
#include <iostream>

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



inline std::ostream& operator<<(std::ostream& os, std::vector<size_t> const& vec)
{
    os << "[";
    for(auto const elt: vec) {
        os << elt << ", ";
    }
    os << "\b\b]";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, MapperLib::Simplex const& simplex)
{
    return os << simplex.points;
}

#endif //TYPEDEFS_H
