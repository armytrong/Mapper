//
// Created by jakob on 10.07.24.
//

#ifndef SLINK_SINGLELINKAGE_H
#define SLINK_SINGLELINKAGE_H

#include <memory>

#include "Clusterer.h"

namespace MapperLib {

class SLink_SingleLinkage : public Clusterer{
public:
    SLink_SingleLinkage(std::optional<int> num_clusters, std::optional<Scalar> distance_threshold);
    [[nodiscard]] static std::shared_ptr<Clusterer> make_shared(std::optional<int> num_clusters, std::optional<Scalar> distance_threshold);

    ClusterAssignment predict(Matrix const &data, std::vector<PointId> data_filter) override;
private:
    std::optional<size_t> _num_clusters;
    std::optional<Scalar> _distance_threshold;
};

} // Mapper

#endif //SLINK_SINGLELINKAGE_H
