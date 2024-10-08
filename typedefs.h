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
using ClusterId = size_t;
using IntegerCubeId = size_t;

/**
 * @struct Simplex
 * @brief Struct representing a simplex
 *
 * A simplex consisting of node ids instead of actual points to save space.
 */
struct Simplex{
    std::vector<PointId> points; ///< The points making up the simplex
    [[nodiscard]] std::vector<PointId> get_points(){ return points; }
    [[nodiscard]] Dimension dimension() const { return points.size() - 1;}
    [[nodiscard]] size_t num_nodes() const { return points.size(); }
    [[nodiscard]] PointId operator[](size_t index) const {return points[index]; }
};

/**
 * @struct MapperCluster
 * @brief a cluster containign additional information
 *
 * This cluster is used in the mapper algorithm to decrease lookup times.
 */
struct MapperCluster{
    std::vector<PointId> points;
    ClusterId cluster_id;
    IntegerCubeId integer_cube_id;
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

inline std::ostream& operator<<(std::ostream& os, std::vector<MapperLib::Simplex> const& vec)
{
    os << "[";
    for(auto const& elt: vec) {
        os << elt << ", ";
    }
    os << "\b\b]";
    return os;
}


#endif //TYPEDEFS_H
