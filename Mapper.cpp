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
               std::unique_ptr<Projection> projection) :
        _clusterer(std::move(clusterer)),
        _data_cover(std::move(data_cover)),
        _projection(std::move(projection))
{}

std::vector<Simplex> Mapper::map(const Matrix &data) const
{
    std::vector<MapperCluster> clusters;
    for(LinearCubeId id = 0; id < _data_cover->get_total_num_cubes(); id++){
        auto const data_in_cube = _data_cover->get_points_in_cube(id);
        auto const cluster_assignment = _clusterer->predict(data, data_in_cube);
        for(auto const& cluster: cluster_assignment){
            clusters.push_back({cluster, id});
        }
    }
}


} // Mapper