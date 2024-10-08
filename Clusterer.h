//
// Created by jakob on 23.06.24.
//

#ifndef CLUSTERER_H
#define CLUSTERER_H

#include "typedefs.h"


namespace MapperLib {

using Cluster = std::vector<PointId>;
using ClusterAssignment = std::vector<Cluster>;

/**
 * @class Clusterer
 * @brief Abstract base class for clustering algorithms.
 */
class Clusterer {
public:
    virtual ~Clusterer() = default;
    virtual ClusterAssignment predict(Matrix const &data, std::vector<PointId> data_filter) = 0;
private:
};

} // cluster



#endif //CLUSTERER_H
