//
// Created by jgier on 29.06.2024.
//

#ifndef MAPPER_CECHCOMPLEX_H
#define MAPPER_CECHCOMPLEX_H

#include <vector>
#include <generator>
#include <memory>

#include "typedefs.h"

namespace MapperLib {
class DataCover;

class Complex {
public:
    virtual ~Complex() = default;
    [[nodiscard]] virtual std::vector<Simplex> generate(std::vector<MapperCluster> const &clusters) const = 0;
};

class CechComplex final : public Complex {
public:

    CechComplex(DataCover const& data_cover, Dimension max_dimension);
    [[nodiscard]] std::vector<Simplex> generate(std::vector<MapperCluster> const& clusters) const override;
private:
    using Iterator = std::vector<MapperCluster>::const_iterator;
    using ClustersByCube = std::vector<std::vector<MapperCluster const*>>;

    /**
     * @class SimplexComputer
     * @brief Class for outsourcing simplex computations for parallelization.
     *
     * This class handles the computation of simplices in a separate thread.
     */
    class SimplexComputer {
        Iterator _begin; ///< Start iterator for the clusters.
        Iterator _end;   ///< End iterator for the clusters.
        ClustersByCube& _clusters_by_cube; ///< Reference to clusters organized by cube.
        Dimension const _dim; ///< Dimension of the simplices to compute.
        DataCover const& _data_cover; ///< Reference to the DataCover object.
        std::vector<Simplex> _own_result; ///< Local results for this computer.
        std::vector<Simplex> &_result; ///< Global results shared among computers.
        std::mutex &_mutex; ///< Mutex for thread-safe access to results.
        int _id; ///< Unique identifier for the computer.

    public:
        static inline int id_counter = 0; ///< Static counter for unique IDs.

        /**
         * @brief Constructs a SimplexComputer object.
         *
         * Initializes the simplex computer with the given parameters.
         *
         * @param begin Start iterator for the clusters.
         * @param end End iterator for the clusters.
         * @param clusters_by_cube Reference to clusters organized by cube.
         * @param dim Dimension of the simplices to compute.
         * @param data_cover Reference to the DataCover object.
         * @param result Reference to the global results vector.
         * @param mutex Reference to the mutex for thread safety.
         */
        SimplexComputer(Iterator begin, Iterator end, ClustersByCube &clusters_by_cube, Dimension dim,
                        DataCover const &data_cover, std::vector<Simplex> &result, std::mutex &mutex);

        /**
         * @brief Computes simplices for the assigned clusters.
         *
         * This method performs the actual computation of simplices based on the clusters assigned to this computer.
         */
        void compute();
    };

    /**
     * @struct ComputerWrapper
     * @brief Wrapper for SimplexComputer to facilitate threading.
     *
     * This struct allows the SimplexComputer to be called as a callable object in threads and prevents unnecessary copying.
     */
    struct ComputerWrapper {
        SimplexComputer & computer; ///< Reference to the SimplexComputer.

        /**
         * @brief Calls the compute method of the SimplexComputer.
         */
        void operator()() const { computer.compute(); }
    };

    [[nodiscard]] std::vector<Simplex> generate_k_simplices(std::vector<MapperCluster> const& clusters, Dimension k) const;

    static std::generator<std::vector<size_t>> generate_k_subsets_of_range(size_t index_max, size_t k);
    static bool check_cluster_intersection(std::vector<MapperCluster const*> const& all_clusters, std::vector<size_t> const& relevant_indices);
    ;

    static std::vector<size_t> get_vector_intersection(std::vector<size_t> vec_1, std::vector<size_t> vec_2);


    DataCover const &_data_cover;
    Dimension _max_dimension;
};

class ComplexFactory
{
public:
    virtual ~ComplexFactory() = default;
    [[nodiscard]] virtual std::unique_ptr<Complex> create_complex(DataCover const& data_cover) const = 0;
};

class CechComplexFactory final : public ComplexFactory
{
public:
    explicit CechComplexFactory(Dimension max_dimension);
    [[nodiscard]] static std::shared_ptr<ComplexFactory> make_shared(Dimension max_dimension);

    [[nodiscard]] std::unique_ptr<Complex> create_complex(DataCover const &data_cover) const override;

private:
    Dimension _max_dimension;
};

} // Mapper

#endif //MAPPER_CECHCOMPLEX_H
