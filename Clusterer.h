//
// Created by jakob on 23.06.24.
//

#ifndef CLUSTERER_H
#define CLUSTERER_H

#include <vector>
#include "typedefs.h"


namespace Mapper {

using Cluster = std::vector<PointId>;
using ClusterAssignment = std::vector<Cluster>;

class Clusterer {
public:
    virtual ~Clusterer() = default;
    virtual ClusterAssignment predict(Matrix const &data, std::vector<PointId> data_filter) = 0;
private:
};

} // cluster



#endif //CLUSTERER_H
