//
// Created by jakob on 24.06.24.
//

#ifndef DATACOVER_H
#define DATACOVER_H

#include <memory>
#include <ostream>

#include "SingleLinkage.h"
#include "typedefs.h"

namespace MapperLib {
/**
 * @class DataCover
 * @brief Class for sectioning data into hypercubes
 *
 * This class provides the framework for splitting data into overlapping hypercubes for the Mapper algorithm.
 */
class DataCover {
public:
    using CubeId = std::vector<int>;

    /**
     * Create a sectioning of the data space into hypercubes.
     * @param resolution Integer variable declaring how many intervals there should be along each axis
     * @param perc_overlap Floating point variable declaring how much the hypercubes should overlap, has to be  nonnegative and <= 0.5.
     * @param data A reference to the data points, to interpolate the minima and maxima
     * @param minima A vector defining the minima that should be used in each dimension
     * @param maxima A vector defining the maxima that should be used in each dimension
     */
    DataCover(
        size_t resolution,
        double perc_overlap,
        Matrix const& data,
        std::optional<Vector> minima = std::nullopt,
        std::optional<Vector> maxima = std::nullopt
    );

    /**
     * Create a sectioning of the data space into hypercubes.
     * @param resolution vector defining the number of intervals along each axis
     * @param perc_overlap Floating point variable declaring how much the hypercubes should overlap, has to be  nonnegative and <= 0.5.
     * @param data A reference to the data points, to interpolate the minima and maxima
     * @param minima A vector defining the minima that should be used in each dimension
     * @param maxima A vector defining the maxima that should be used in each dimension
     */
    DataCover(
        std::vector<size_t> resolution,
        double perc_overlap,
        Matrix const& data,
        std::optional<Vector> minima = std::nullopt,
        std::optional<Vector> maxima = std::nullopt
    );

    /**
     * Compute the cube a vector lies in disregarding the overlaps
     * @param vec data point
     * @return the id of the native cube of vec
     */
    [[nodiscard]] CubeId get_native_cube_id(Vector const& vec) const;

    /**
     * Get the indices of the points in a given cube.
     * @param cube_id
     * @return Vector of points in the cube
     */
    [[nodiscard]] std::vector<PointId> get_points_in_cube(IntegerCubeId cube_id) const;

    /**
     * Get all cubes neighboring the given cube
     * @param integer_cube_id
     * @return vector of cube ids
     */
    [[nodiscard]] std::vector<IntegerCubeId> get_neighbor_cubes(IntegerCubeId integer_cube_id) const;

    /**
     * Convert the coordinates of a cube (i.e. the cube id) to the integer id
     * @param cube_id the id to convert
     * @return an integer uniquely representing the cube
     */
    IntegerCubeId convert_to_integer_cube_id(CubeId const& cube_id) const;

    /**
     * Convert the numerical integer cube id of a cube to its coordinate in the cube grid
     * @param integer_cube_id the id to convert
     * @return a cube id, i.e. the coordinates of the cube in the grid.
     */
    CubeId convert_to_cube_id(IntegerCubeId integer_cube_id) const;

    /**
     * get the number of all cubes in the cover
     * @return number of cubes
     */
    size_t get_total_num_cubes() const;

    /**
     * get the resolution in a certain dimension
     * @param dim the dimension to query
     * @return an unsigned integer representing the resolution
     */
    size_t get_num_cubes_in_dimension(Dimension dim) const;

    /**
     * Determine whether a data point is inside a cube, taking overlaps into account
     * @param vec the data point
     * @param cube_id the cube to test against
     * @return true if the cube contains the point, else false
     */
    bool is_vector_in_cube(Vector const& vec, CubeId const& cube_id) const;

private:
    void initialize_cube_cache() const;

    /**
     * Get all cubes containing the data point v
     * @param v data point to look up
     * @return vector of CubeIds containing v
     */
    std::vector<CubeId> get_parent_cubes(Vector const& v) const;

    /**
     * get the minimum value of any data point in the given dimension
     * @param dimension the dimension to query
     * @return the minimum Scalar in the respective dimension
     */
    [[nodiscard]] Scalar get_data_min_in_dimension(Dimension dimension) const;
    /**
     * get the maximum value of any data point in the given dimension
     * @param dimension the dimension to query
     * @return the maximum Scalar in the respective dimension
     */
    [[nodiscard]] Scalar get_data_max_in_dimension(Dimension dimension) const;

    [[nodiscard]] Scalar get_data_width_in_dimension(Dimension dimension) const;

    static constexpr Scalar overhang_factor = 0.05;
    [[nodiscard]] Scalar get_overhang_length_in_dimension(Dimension dimension) const;

    void initialize_minima_from_data();
    void initialize_maxima_from_data();

    [[nodiscard]] Vector get_cube_center(CubeId const& cube_id) const;
    [[nodiscard]] std::vector<CubeId> get_neighbor_cubes(CubeId const& cube_id) const;


    std::vector<size_t> _resolution;
    double const _perc_overlap;
    Matrix const& _data;
    size_t const _data_dimension;
    Vector _minima;
    Vector _maxima;

    mutable std::optional<std::vector<std::vector<PointId>>> _cube_cache;

};

/**
 *@class DataCoverFactory
 * @brief Factory class creating DataCover objects
 *
 * used to create the object within the map method of Mapper
 */
class DataCoverFactory
{
public:
    /**
     * Constructor for DataCoverFactory with single resolution
     * @param resolution Single resolution value for all dimensions
     * @param perc_overlap Percentage of overlap between hypercubes
     * @param minima Optional minima for each dimension
     * @param maxima Optional maxima for each dimension
     */
    DataCoverFactory(
        size_t resolution,
        double perc_overlap,
        std::optional<Vector> minima = std::nullopt,
        std::optional<Vector> maxima = std::nullopt
    );

    /**
     * Constructor for DataCoverFactory with multiple resolutions
     * @param resolution Vector of resolution values for each dimension
     * @param perc_overlap Percentage of overlap between hypercubes
     * @param minima Optional minima for each dimension
     * @param maxima Optional maxima for each dimension
     */
    DataCoverFactory(
        std::vector<size_t> resolution,
        double perc_overlap,
        std::optional<Vector> minima = std::nullopt,
        std::optional<Vector> maxima = std::nullopt
    );

    /**
     * Static factory method to create a shared pointer to DataCoverFactory
     * @param resolution Single resolution value for all dimensions
     * @param perc_overlap Percentage of overlap between hypercubes
     * @param minima Optional minima for each dimension
     * @param maxima Optional maxima for each dimension
     * @return Shared pointer to a new DataCoverFactory instance
     */
    [[nodiscard]] static std::shared_ptr<DataCoverFactory> make_shared(
        size_t resolution,
        double perc_overlap,
        std::optional<Vector> minima = std::nullopt,
        std::optional<Vector> maxima = std::nullopt
    );

    /**
     * Static factory method to create a shared pointer to DataCoverFactory
     * @param resolution Vector of resolution values for each dimension
     * @param perc_overlap Percentage of overlap between hypercubes
     * @param minima Optional minima for each dimension
     * @param maxima Optional maxima for each dimension
     * @return Shared pointer to a new DataCoverFactory instance
     */
    [[nodiscard]] static std::shared_ptr<DataCoverFactory> make_shared(
        std::vector<size_t> resolution,
        double perc_overlap,
        std::optional<Vector> minima = std::nullopt,
        std::optional<Vector> maxima = std::nullopt
    );

    /**
     * Create a DataCover object
     * @param data A reference to the data points
     * @return Unique pointer to a new DataCover instance
     */
    [[nodiscard]] std::unique_ptr<DataCover> create_data_cover(Matrix const& data) const;

private:
    std::vector<size_t> _resolution; ///< Resolution of the data cover
    std::optional<size_t> _single_resolution; ///< Single resolution value if applicable
    double _perc_overlap; ///< Percentage of overlap between hypercubes
    std::optional<Vector> _minima; ///< Optional minima for each dimension
    std::optional<Vector> _maxima; ///< Optional maxima for each dimension
};


} // Mapper
std::ostream& operator<<(std::ostream& stream, MapperLib::DataCover::CubeId const& vec);



#endif //DATACOVER_H
