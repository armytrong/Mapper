//
// Created by jgier on 29.06.2024.
//

#include "CechComplex.h"

#include <cassert>
#include <ranges>
#include <generator>
#include <bits/ranges_algo.h>
#include "DataCover.h"

namespace MapperLib {

std::vector<Simplex> CechComplex::generate(const std::vector<MapperCluster> &clusters) const {
    std::vector<Simplex> result;
    std::cout << "-> Generating Simplical Complex" << std::endl;
    for(Dimension k = 0; k <= _max_dimension; k++){
        std::cout << "--> Generating Simplices of Dimension " << k << " ...";
        auto const k_simplices = generate_k_simplices(clusters, k);
        result.insert(result.end(), k_simplices.begin(), k_simplices.end());
        std::cout << "\b\b\bDone!" << std::endl;
    }
    return result;
}

std::vector<Simplex> CechComplex::generate_k_simplices(std::vector<MapperCluster> const &clusters, Dimension const k) const
{
    // Create single vertex simplices if k = 0
    if(k == 0) {
        std::vector<Simplex> result;
        for(auto const& cluster : clusters) {
            result.push_back(Simplex{{cluster.cluster_id}});
        }
        return result;
    }
    std::vector<std::vector<MapperCluster const*>> clusters_by_cubes(_data_cover.get_total_num_cubes());
    for (auto const& cluster : clusters) {
        clusters_by_cubes[cluster.linear_cube_id].push_back(&cluster);
    }

    std::vector<Simplex> result;

    // Iterate over all clusters
    // The iteration order is inefficient as of yet. It would be better to iterate over the cubes and do a lot of computation less often.
    for(auto const& cluster : clusters) {
        auto const root_cube = cluster.linear_cube_id;
        auto const neighbor_cubes = _data_cover.get_neighbor_cubes(root_cube);
        std::vector<MapperCluster const*> relevant_clusters;
        for (auto const neighbor : neighbor_cubes) {
            relevant_clusters.insert(relevant_clusters.end(), clusters_by_cubes[neighbor].begin(), clusters_by_cubes[neighbor].end());
        }
        if(relevant_clusters.size() < k) continue;
        relevant_clusters.push_back(&cluster);
        for(auto subset_indexset : generate_k_subsets_of_range(relevant_clusters.size() - 1, k)) {
            assert(subset_indexset.size() == k);
            if(not std::ranges::is_sorted(subset_indexset)) { continue; }
            subset_indexset.push_back(relevant_clusters.size()-1);
            if(check_cluster_intersection(relevant_clusters, subset_indexset)) {
                std::vector<ClusterId> id_vector;
                for(auto const subset_index : subset_indexset) {
                    id_vector.push_back(relevant_clusters[subset_index]->cluster_id);
                }
                result.push_back({id_vector});
            }
        }
    }
    return result;
}

std::generator<std::vector<size_t>> CechComplex::generate_k_subsets_of_range(size_t const index_max, size_t const k)
{
    std::vector bitmask(k, 1);
    bitmask.resize(index_max, 0);
    do {
        std::vector<size_t> result;
        for (size_t i = 0; i < index_max; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) result.push_back(i);
        }
        co_yield result;
    } while (std::ranges::prev_permutation(bitmask).found);
    co_return;
}

bool CechComplex::check_cluster_intersection(std::vector<MapperCluster const*> const& all_clusters,
    std::vector<size_t> const&  relevant_indices)
{
    if(all_clusters.empty()) return false;
    auto cluster_intersection = all_clusters[relevant_indices[0]]->points;
    for(auto const i :relevant_indices) {
        cluster_intersection = get_vector_intersection(cluster_intersection, all_clusters[i]->points);
    }
    return not cluster_intersection.empty();
}

std::vector<size_t> CechComplex::get_vector_intersection(std::vector<size_t> vec_1, std::vector<size_t> vec_2)
{
    std::ranges::sort(vec_1);
    std::ranges::sort(vec_2);
    std::vector<size_t> result;
    while(not vec_1.empty() and not vec_2.empty()) {
        if(vec_1.back() == vec_2.back()) {
            result.push_back(vec_1.back());
            vec_1.pop_back();
            vec_2.pop_back();
        } else if(vec_1.back() > vec_2.back()) {
            vec_1.pop_back();
        } else {
            vec_2.pop_back();
        }
    }
    return result;
}

CechComplexFactory::CechComplexFactory(Dimension const max_dimension): _max_dimension(max_dimension)
{}

std::shared_ptr<ComplexFactory> CechComplexFactory::make_shared(Dimension max_dimension)
{return std::make_shared<CechComplexFactory>(max_dimension);}

CechComplex::CechComplex(const DataCover &data_cover, Dimension const max_dimension): _data_cover(data_cover),
                                                                                _max_dimension(max_dimension) {

}

std::unique_ptr<Complex> CechComplexFactory::create_complex(DataCover const &data_cover) const
{
    return std::make_unique<CechComplex>(data_cover, _max_dimension);
}
} // Mapper