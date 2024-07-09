//
// Created by jakob on 26.06.24.
//

#include "Mapper.h"
#include "DataCover.h"

#include "CechComplex.h"
#include "Clusterer.h"
#include "Projection.h"
namespace MapperLib {

Mapper::Mapper(std::unique_ptr<Clusterer> clusterer,
               std::unique_ptr<DataCover> data_cover,
               std::unique_ptr<Projection> projection,
               std::unique_ptr<Complex> complex) :
        _clusterer(std::move(clusterer)),
        _data_cover(std::move(data_cover)),
        _projection(std::move(projection)),
        _complex(std::move(complex))
{}

std::vector<Simplex> Mapper::map(const Matrix &data)
{
    auto const projected_data = _projection->project(data);
    _data_cover = std::make_unique<DataCover>(3,0.5,projected_data);
    _complex = std::make_unique<CechComplex>(*_data_cover, 2);

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