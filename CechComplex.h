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

/**
 * @class Complex
 * @brief Abstract base class for simplicial complexes.
 *
 * This class provides an interface for generating simplicial complices from clusters.
 */
class Complex {
public:
    virtual ~Complex() = default;

    /**
     * @brief Generates simplices from the provided clusters.
     *
     * This method must be implemented by derived classes to generate a vector of simplices.
     *
     * @param clusters A vector of clusters as generated by the mapper clusterer.
     * @return A vector of simplices representing a simplicial complex.
     */
    [[nodiscard]] virtual std::vector<Simplex> generate(std::vector<MapperCluster> const &clusters) const = 0;
};

/**
 * @class CechComplex
 * @implements Complex
 * @brief Class for generating Cech complexes from overlapping clusters.
 *
 * This class implements the Complex interface and provides methods to generate a cech complex on the given clusters
 * containing simplices up to a specified dimension
 */
class CechComplex final : public Complex {
    static constexpr size_t NUM_THREADS = 8; ///< Maximum number of threads for parallel computation.
public:
    /**
     * @brief Constructs a CechComplex object.
     *
     * Initializes the CechComplex with a data cover and maximum dimension. The data cover is used to facilitate
     * reasonably efficient enumeration of possible simplices.
     *
     * @param data_cover A reference to a data_cover object.
     * @param max_dimension The maximum dimension of simplices to generate.
     */
    CechComplex(DataCover const& data_cover, Dimension max_dimension);

    /**
     * @brief Generates a Cech complex on overlapping clusters.
     *
     * This method iterates through the specified dimensions and generates simplices for each dimension.
     * The process is multithreaded.
     * @warning The number of threads is currently hard coded to be <= 8. Change NUM_THREADS for more or less cores.
     *
     * @param clusters A vector of clusters as generated by the mapper clusterer.
     * @return A vector of simplices representing a simplicial complex.
     */
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

    /**
     * @brief Generates the Mapper simplices of dimension k for a set of clusters.
     *
     * This method computes simplices of a specific dimension based on the provided clusters.
     *
     * @param clusters A vector containing the data-clusters.
     * @param k The dimension of the required simplices.
     * @return A vector of simplices of dimension k.
     */
    [[nodiscard]] std::vector<Simplex> generate_k_simplices(std::vector<MapperCluster> const& clusters, Dimension k) const;

    /**
     * @brief Generates all subsets of {0,...,index_max-1} of size k.
     *
     * This generator yields all combinations of indices of a specified size from a range.
     *
     * @param index_max Upper limit of the returned set.
     * @param k Size of yielded sets.
     * @return A generator yielding subsets of specified size.
     */
    static std::generator<std::vector<size_t>> generate_k_subsets_of_range(size_t index_max, size_t k);

    /**
     * @brief Checks if there is an intersection among the specified clusters.
     *
     * This method verifies whether the clusters at the given indices have any points in common.
     *
     * @param all_clusters A vector of all clusters.
     * @param relevant_indices Indices of the clusters to check for intersection.
     * @return True if there is an intersection, false otherwise.
     */
    static bool check_cluster_intersection(std::vector<MapperCluster const*> const& all_clusters, std::vector<size_t> const& relevant_indices);

    /**
     * @brief Computes the intersection of two vectors.
     *
     * This method returns the common elements between two vectors.
     *
     * @param vec_1 First vector.
     * @param vec_2 Second vector.
     * @return A vector containing the intersection of the two input vectors.
     */
    static std::vector<size_t> get_vector_intersection(std::vector<size_t> vec_1, std::vector<size_t> vec_2); //ToDo: This should probably be in a helper file

    DataCover const &_data_cover; ///< Reference to the DataCover object.
    Dimension _max_dimension; ///< Maximum dimension for simplices.
};

/**
 * @class ComplexFactory
 * @brief Abstract factory class for creating Complex objects.
 *
 * This class provides an interface for creating instances of Complex subclasses.
 */
class ComplexFactory {
public:
    virtual ~ComplexFactory() = default;

    /**
     * @brief Creates a Complex object.
     *
     * This method must be implemented by derived classes to create a specific type of Complex.
     *
     * @param data_cover A reference to the DataCover object.
     * @return A unique pointer to the created Complex object.
     */
    [[nodiscard]] virtual std::unique_ptr<Complex> create_complex(DataCover const& data_cover) const = 0;
};

/**
 * @class CechComplexFactory
 * @brief Factory class for creating CechComplex objects.
 *
 * This class implements the ComplexFactory interface to create instances of CechComplex.
 */
class CechComplexFactory final : public ComplexFactory {
public:
    /**
     * @brief Constructs a CechComplexFactory object.
     *
     * Initializes the factory with the specified maximum dimension.
     *
     * @param max_dimension The maximum dimension for the CechComplex to be created.
     */
    explicit CechComplexFactory(Dimension max_dimension);

    /**
     * @brief Creates a shared pointer to a CechComplexFactory instance.
     *
     * This static method allows for easy memory management and object creation.
     *
     * @param max_dimension The maximum dimension for the CechComplex.
     * @return A shared pointer to the newly created CechComplexFactory instance.
     */
    [[nodiscard]] static std::shared_ptr<ComplexFactory> make_shared(Dimension max_dimension);

    /**
     * @brief Creates a CechComplex object.
     *
     * This method creates a new instance of CechComplex using the provided DataCover.
     *
     * @param data_cover A reference to the DataCover object.
     * @return A unique pointer to the created CechComplex object.
     */
    [[nodiscard]] std::unique_ptr<Complex> create_complex(DataCover const &data_cover) const override;

private:
    Dimension _max_dimension; ///< Maximum dimension for simplices in the CechComplex.
};

} // namespace MapperLib

#endif // MAPPER_CECHCOMPLEX_H
