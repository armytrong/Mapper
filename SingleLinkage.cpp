//
// Created by jakob on 14.06.24.
//

#include "SingleLinkage.h"

#include <cassert>
#include <algorithm>
#include <iostream>
#include <iomanip>

namespace MapperLib {
SingleLinkage::SingleLinkage(std::optional<int> const num_clusters, std::optional<Scalar> const distance_threshold) {
    assert(num_clusters.has_value() or distance_threshold.has_value());
    _num_clusters = num_clusters;
    _distance_threshold = distance_threshold;
}

std::shared_ptr<Clusterer> SingleLinkage::make_shared(std::optional<int> num_clusters,
    std::optional<Scalar> distance_threshold)
{ return std::make_shared<SingleLinkage>(num_clusters, distance_threshold);}

ClusterAssignment SingleLinkage::predict(Matrix const &data, std::vector<PointId> data_filter) {
    Matrix distance_matrix;
    ClusterAssignment clusters;


    size_t const numdata = data.size();
    clusters.resize(numdata);
    for (PointId i = 0; i < numdata; ++i) {
        clusters[i].push_back(i);
    }

    // Initialize distance matrix
    distance_matrix.resize(numdata, std::vector<double>(numdata, 0.0));
    for (auto const i: data_filter) {
        for (auto const j: data_filter) {
            if(j <= i) continue;
            distance_matrix[i][j] = distance_matrix[j][i] = euclididan_distance(data[i], data[j]);
        }
    }

    size_t const num_clusters = _num_clusters.has_value() ? _num_clusters.value() : 1;
    while (num_clusters < data_filter.size()) {
        Scalar minimum_found_distance = std::numeric_limits<double>::infinity();
        std::optional<size_t> cluster1, cluster2;

        // Find the closest pair of clusters
        for (auto const i: data_filter) {
            for (auto const j: data_filter) {
                if (j <= i) continue;
                Scalar const dist = min_distance(distance_matrix, clusters[i], clusters[j]);
                if (dist < minimum_found_distance) {
                    minimum_found_distance = dist;
                    cluster1 = i;
                    cluster2 = j;
                }
            }
        }

        // Merge the two closest clusters
        assert(cluster1.has_value() and cluster2.has_value());
        clusters[*cluster1].insert(clusters[*cluster1].end(), clusters[*cluster2].begin(), clusters[*cluster2].end());
        //clusters.erase(clusters.begin() + static_cast<long>(*cluster2));
        data_filter.erase(std::ranges::find(data_filter, *cluster2));

        if (_distance_threshold.has_value()) {
            if (minimum_found_distance > _distance_threshold.value()) {
                ClusterAssignment result;
                for (size_t const index: data_filter) {
                    result.push_back(clusters[index]);
                }
                return result;
            }
        }
    }
    ClusterAssignment result;
    for (size_t index: data_filter) {
        if(clusters[index].size() < 10) continue;
        result.push_back(clusters[index]);
    }
    return result;

}

Scalar SingleLinkage::min_distance(Matrix const &distances, std::vector<PointId> const &cluster1,
                                   std::vector<PointId> const &cluster2) {
    double min_dist = std::numeric_limits<Scalar>::max();
    for (auto const i: cluster1) {
        for (auto const j: cluster2) {
            if (distances[i][j] < min_dist) {
                min_dist = distances[i][j];
            }
        }
    }
    return min_dist;
}
} // Cluster