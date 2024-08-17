//
// Created by jakob on 10.07.24.
//

#ifndef SLINK_SINGLELINKAGE_H
#define SLINK_SINGLELINKAGE_H

#include <memory>

#include "Clusterer.h"

namespace MapperLib {
/**
 * @class SLink_SingleLinkage
 * @brief efficient implementation of single linkage clustering
 *
 * A class implementing the SLINK algorithm for single linkage agglomorative clustering.
 * The main algorithm is due to R. Sibson: "SLINK: An optimally efficient algorithm for the single-link cluster method".
 */
class SLink_SingleLinkage : public Clusterer{
public:

    /**
    * @brief Constructs an SLink_SingleLinkage object.
    *
    * Initializes the clustering parameters with the provided optional values.
    * At least one of num_clusters or distance_threshold must be specified.
    *
    * @param num_clusters Optional integer specifying the desired number of clusters.
    * @param distance_threshold Optional Scalar value for the maximum distance threshold.
    */
    SLink_SingleLinkage(std::optional<int> num_clusters, std::optional<Scalar> distance_threshold);
    [[nodiscard]] static std::shared_ptr<Clusterer> make_shared(std::optional<int> num_clusters, std::optional<Scalar> distance_threshold);

    /**
     * @brief Predicts cluster assignments for the given dataset.
     *
     * This method performs the single linkage clustering algorithm on the provided data and returns the cluster assignments.
     *
     * @param data A constant reference to a Matrix containing the data points to be clustered.
     * @param data_filter A vector of ids that filtering the data points for prediction.
     * @return ClusterAssignment The resulting cluster assignments for the input data.
     */
    ClusterAssignment predict(Matrix const &data, std::vector<PointId> data_filter) override;
private:
    std::optional<size_t> _num_clusters;
    std::optional<Scalar> _distance_threshold;
};

} // Mapper

#endif //SLINK_SINGLELINKAGE_H
