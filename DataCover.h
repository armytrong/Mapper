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
    explicit DataCover(
        size_t resolution,
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
     * Determine whether a data point is inside a cube, taking overlaps into account
     * @param vec the data point
     * @param cube_id the cube to test against
     * @return true if the cube contains the point, else false
     */
    bool is_vector_in_cube(Vector const& vec, CubeId const& cube_id) const;

private:
    void initialize_cube_cache() const;

    std::vector<CubeId> get_parent_cubes(Vector const& v) const;

    [[nodiscard]] Scalar get_data_min_in_dimension(Dimension dimension) const;
    [[nodiscard]] Scalar get_data_max_in_dimension(Dimension dimension) const;

    void initialize_minima_from_data();
    void initialize_maxima_from_data();

    [[nodiscard]] Vector get_cube_center(CubeId const& cube_id) const;
    [[nodiscard]] std::vector<CubeId> get_neighbor_cubes(CubeId const& cube_id) const;


    size_t const _resolution;
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
    DataCoverFactory(size_t resolution, double perc_overlap, std::optional<Vector> minima = std::nullopt, std::optional<Vector> maxima = std::nullopt);
    [[nodiscard]] static std::shared_ptr<DataCoverFactory> make_shared(size_t resolution, double perc_overlap, std::optional<Vector> minima = std::nullopt, std::optional<Vector> maxima = std::nullopt);

    [[nodiscard]] std::unique_ptr<DataCover> create_data_cover(Matrix const& data) const;

private:
    size_t _resolution;
    double _perc_overlap;
    std::optional<Vector> _minima;
    std::optional<Vector> _maxima;
};


} // Mapper
std::ostream& operator<<(std::ostream& stream, MapperLib::DataCover::CubeId const& vec);



#endif //DATACOVER_H
