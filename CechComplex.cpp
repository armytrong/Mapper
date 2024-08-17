//
// Created by jgier on 29.06.2024.
//

#include "CechComplex.h"

#include <cassert>
#include <ranges>
#include <generator>
#include <algorithm>
#include <mutex>
#include <thread>
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

CechComplex::SimplexComputer::SimplexComputer(
    Iterator const begin,
    Iterator const end,
    ClustersByCube &clusters_by_cube,
    Dimension const dim,
    DataCover const &data_cover,
    std::vector<Simplex> &result,
    std::mutex &mutex
):  _begin(begin),
    _end(end),
    _clusters_by_cube(clusters_by_cube),
    _dim(dim),
    _data_cover(data_cover),
    _result(result),
    _mutex(mutex)
{
    _id = SimplexComputer::id_counter++;
}

void CechComplex::SimplexComputer::compute()
{
    assert(not _clusters_by_cube.empty());
    for(auto cluster_ptr = _begin; cluster_ptr < _end; cluster_ptr++) {
        auto const& cluster = *cluster_ptr;
        auto const root_cube = cluster.integer_cube_id;

        // only clusters in neighboring cubes are relevant, as the overlap is fixed to <= 0.5.
        auto const neighbor_cubes = _data_cover.get_neighbor_cubes(root_cube);
        std::vector<MapperCluster const*> relevant_clusters;
        for (auto const neighbor : neighbor_cubes) {
            relevant_clusters.insert(relevant_clusters.end(), _clusters_by_cube[neighbor].begin(), _clusters_by_cube[neighbor].end());
        }

        // check that there are enough clusters to make a simplex of dimension _dim
        if(relevant_clusters.size() < _dim) continue;
        relevant_clusters.push_back(&cluster);
        for(auto subset_indexset : generate_k_subsets_of_range(relevant_clusters.size() - 1, _dim)) {
            assert(subset_indexset.size() == _dim);
            if(not std::ranges::is_sorted(subset_indexset)) { continue; }
            subset_indexset.push_back(relevant_clusters.size()-1);
            if(check_cluster_intersection(relevant_clusters, subset_indexset)) {
                std::vector<ClusterId> id_vector;
                for(auto const subset_index : subset_indexset) {
                    id_vector.push_back(relevant_clusters[subset_index]->cluster_id);
                }
                _own_result.emplace_back(id_vector);
            }
        }
    }
    std::unique_lock lock(_mutex);
    _result.insert(_result.begin(), _own_result.begin(), _own_result.end());
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

    // Sort clusters into their respective cubes. Do this to decrease lookup-times.
    ClustersByCube clusters_by_cubes(_data_cover.get_total_num_cubes());
    for (auto const& cluster : clusters) {
        clusters_by_cubes[cluster.integer_cube_id].push_back(&cluster);
    }

    // Create at most one thread per cluster
    auto const num_threads = std::min(NUM_THREADS, clusters.size());
    std::vector<std::thread> threads;
    std::vector<SimplexComputer> computers;
    threads.reserve(num_threads);
    computers.reserve(num_threads);

    // Save results here, mutex locks results.
    std::vector<Simplex> results;
    std::mutex results_mutex;

    // Create computers and threads
    for(size_t i = 0; i < num_threads; i++) {
        computers.emplace_back(
            clusters.begin() + static_cast<unsigned>(i * clusters.size() /num_threads),
            std::min(clusters.begin() + static_cast<unsigned>((i + 1) * clusters.size()/num_threads), clusters.end()),
            clusters_by_cubes,
            k,
            _data_cover,
            results,
            results_mutex
        );
        // use computer wrappers to avoid copying the computers.
        threads.emplace_back(ComputerWrapper{computers.back()});
    }
    std::vector<Simplex> result;
    for(auto& thread : threads) {
        thread.join();
    }
    return results;
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
{
    return std::make_shared<CechComplexFactory>(max_dimension);
}

CechComplex::CechComplex(const DataCover &data_cover, Dimension const max_dimension): _data_cover(data_cover),
                                                                                _max_dimension(max_dimension) {

}

std::unique_ptr<Complex> CechComplexFactory::create_complex(DataCover const &data_cover) const
{
    return std::make_unique<CechComplex>(data_cover, _max_dimension);
}
} // Mapper