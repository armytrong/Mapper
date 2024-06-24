//
// Created by jakob on 23.06.24.
//

#ifndef CLUSTERER_H
#define CLUSTERER_H

#include <vector>
#include "typedefs.h"


namespace Cluster {

using ClusterAssignment = std::vector<std::vector<PointId>>;

class Clusterer {
public:
    virtual ~Clusterer() = default;
    virtual ClusterAssignment predict(Matrix const& data) = 0;
private:
};

} // cluster

#endif //CLUSTERER_H
