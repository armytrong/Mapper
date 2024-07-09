//
// Created by jakob on 26.06.24.
//

#include "Mapper.h"

#include <cassert>
#include <utility>
#include "DataCover.h"

#include "CechComplex.h"
#include "Clusterer.h"
#include "Projection.h"
namespace MapperLib {
Mapper::Mapper(
        std::shared_ptr<DataCoverFactory> data_cover_factory,
        std::shared_ptr<ComplexFactory> complex_factory,
        std::shared_ptr<Clusterer> clusterer,
        std::shared_ptr<Projection> projection):
    _data_cover_factory(std::move(data_cover_factory)),
    _complex_factory(std::move(complex_factory)),
    _clusterer(std::move(clusterer)),
    _projection(std::move(projection))
{
    assert(_data_cover_factory);
    assert(_complex_factory);
    assert(_clusterer);
    assert(_projection);
}

std::vector<Simplex> Mapper::map(const Matrix &data)
{
    auto const projected_data = _projection->project(data);
    _data_cover = _data_cover_factory->create_data_cover(projected_data);
    _complex = _complex_factory->create_complex(*_data_cover);

    std::vector<MapperCluster> clusters;
    for(LinearCubeId linear_cube_id = 0; linear_cube_id < _data_cover->get_total_num_cubes(); linear_cube_id++){
        auto const data_in_cube = _data_cover->get_points_in_cube(linear_cube_id);
        auto const cluster_assignment = _clusterer->predict(data, data_in_cube);
        for(auto const& cluster: cluster_assignment){
            clusters.push_back({cluster, clusters.size(), linear_cube_id});
        }
    }
    return _complex->generate(clusters);
}


} // Mapper