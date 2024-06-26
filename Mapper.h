//
// Created by jakob on 26.06.24.
//

#ifndef MAPPER_H
#define MAPPER_H
#include <vector>
#include "DataCover.h"
#include "Clusterer.h"

namespace Mapper {

struct Cluster {
    std::vector<PointId> points;
    DataCover::CubeId parent_cube;
};
class Mapper {

    static Matrix project(Matrix const& data, std::vector<size_t> const& dimensions);


};

} // Mapper

#endif //MAPPER_H
