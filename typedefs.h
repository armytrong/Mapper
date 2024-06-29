//
// Created by jakob on 23.06.24.
//

#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <vector>

namespace Mapper{

using Scalar = double;
using Vector = std::vector<Scalar>;
using Matrix = std::vector<std::vector<Scalar>>;
using PointId = size_t;
using Dimension = size_t;

struct Simplex{
    std::vector<PointId> points;
};
}


#endif //TYPEDEFS_H
