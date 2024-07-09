//
// Created by jakob on 14.06.24.
//

#ifndef SINGLELINKAGE_H
#define SINGLELINKAGE_H
#include <limits>
#include <memory>
#include <optional>

#include "Clusterer.h"
#include "LinalgHelpers.h"

namespace MapperLib {

class SingleLinkage : public Clusterer {
public:

    SingleLinkage(std::optional<int> num_clusters, std::optional<Scalar> distance_threshold);
    [[nodiscard]] static std::shared_ptr<Clusterer> make_shared(std::optional<int> num_clusters, std::optional<Scalar> distance_threshold);

    ClusterAssignment predict(Matrix const &data, std::vector<PointId> data_filter) override;

private:
    [[nodiscard]] static Scalar min_distance(
        Matrix const& distances,
        std::vector<PointId> const& cluster1,
        std::vector<PointId> const& cluster2
    );

    std::optional<size_t> _num_clusters;
    std::optional<Scalar> _distance_threshold;

};

} // Cluster

#endif //SINGLELINKAGE_H
