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

class DataCover {
public:
    using CubeId = std::vector<int>;
    explicit DataCover(
        size_t resolution,
        double perc_overlap,
        Matrix const& data,
        std::optional<Vector> minima = std::nullopt,
        std::optional<Vector> maxima = std::nullopt
    );

    [[nodiscard]] CubeId get_native_cube_id(Vector const& vec) const;
    [[nodiscard]] std::vector<PointId> get_points_in_cube(LinearCubeId cube_id) const;

    [[nodiscard]] std::vector<LinearCubeId> get_neighbor_cubes(LinearCubeId linear_cube_id) const;

    LinearCubeId convert_to_linear_cube_id(CubeId const& cube_id) const;
    CubeId convert_to_cube_id(LinearCubeId linear_id) const;

    size_t get_total_num_cubes() const;

private:
    void initialize_cube_cache() const;

    std::vector<CubeId> get_parent_cubes(Vector const& v) const;
    bool is_vector_in_cube(Vector const& vec, CubeId const& cube_id) const;

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
