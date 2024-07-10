//
// Created by jakob on 10.07.24.
//

#include "SLink_SingleLinkage.h"

#include <cassert>
#include <bits/ranges_algo.h>

#include "LinalgHelpers.h"

namespace MapperLib {
SLink_SingleLinkage::SLink_SingleLinkage(std::optional<int> num_clusters, std::optional<Scalar> distance_threshold)
{
    assert(num_clusters.has_value() or distance_threshold.has_value());
    _num_clusters = num_clusters;
    _distance_threshold = distance_threshold;
}

std::shared_ptr<Clusterer> SLink_SingleLinkage::make_shared(std::optional<int> num_clusters, std::optional<Scalar> distance_threshold)
{
    return std::make_shared<SLink_SingleLinkage>(num_clusters, distance_threshold);
}

ClusterAssignment SLink_SingleLinkage::predict(Matrix const &data, std::vector<PointId> data_filter)
{
    std::ranges::sort(data_filter);
    auto const num_data = data_filter.size();
    // TODO: Better variable names than in the paper
    std::vector<PointId> cluster_parents = {0};
    std::vector<Scalar> data_entering_cluster_times = {std::numeric_limits<Scalar>::infinity()};
    std::vector<Scalar> m(num_data,0);

    for(PointId id = 1; id < num_data; id++) {
        cluster_parents.push_back(id);
        data_entering_cluster_times.push_back(std::numeric_limits<Scalar>::infinity());
        for(PointId i = 0; i < id; i++) {
            m[i] = euclididan_distance(data[data_filter[i]], data[data_filter[id]]);
        }

        for(PointId i = 0; i < id; i++) {
            if(data_entering_cluster_times[i] >= m[i]) {
                m[cluster_parents[i]] = std::min(m[cluster_parents[i]], data_entering_cluster_times[i]);
                data_entering_cluster_times[i] = m[i];
                cluster_parents[i] = id;
            } else {
                m[cluster_parents[i]] = std::min(m[cluster_parents[i]],m[i]);
            }
        }


        for(PointId i = 0; i < id; i++) {
            if (data_entering_cluster_times[i] >= data_entering_cluster_times[cluster_parents[i]]) {
                //cluster_parents[i] = id;
            }
        }
    }

    auto const compare_cluster_enter_time = [&](PointId a, PointId b) {
        return data_entering_cluster_times[a] < data_entering_cluster_times[b];
    };

    auto const find_cluster_representative = [&](PointId id) {
        return cluster_parents[id];
    };

    std::vector<PointId> points(num_data);
    ClusterAssignment cluster_assignment(num_data);
    for(PointId i = 0; i < num_data; i++) {
        points[i] = i;
        cluster_assignment[i] = {i};
    }

    //std::ranges::sort(points, compare_cluster_enter_time);
    size_t points_done = 0;
    for(auto const point_id : points) {
        if(_num_clusters.has_value() and
           num_data - points_done < _num_clusters.value())
        {
            break;
        }
        if(_distance_threshold.has_value() and
           data_entering_cluster_times[point_id] > _distance_threshold.value())
        {
            continue;
        }
        points_done++;
        auto const cluster = find_cluster_representative(point_id);
        if(cluster == point_id) {
            continue;
        }
        cluster_assignment[cluster].insert(cluster_assignment[cluster].end(), cluster_assignment[point_id].begin(), cluster_assignment[point_id].end());
        cluster_assignment[point_id] = {};
    }

    ClusterAssignment result;
    for(auto const& cluster : cluster_assignment) {
        if (cluster.size() > 2) {
            result.push_back(cluster);
            std::ranges::transform(result.back().begin(), result.back().end(), result.back().begin(), [&](PointId id){return data_filter[id];});
        }
    }
    return result;


}



} // Mapper