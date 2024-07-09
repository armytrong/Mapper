//
// Created by jakob on 26.06.24.
//

#ifndef MAPPER_H
#define MAPPER_H
#include <vector>
#include <memory>
#include "typedefs.h"
#include "DataCover.h"



namespace MapperLib {
class Complex;
class Clusterer;
class Projection;

class Mapper {
public:
    using ClusterId = size_t;
    struct MapperCluster{
        std::vector<PointId> points;
        ClusterId cluster_id;
        DataCover::LinearCubeId linear_cube_id;
    };

    using LinearCubeId = DataCover::LinearCubeId;
    Mapper(std::unique_ptr<Clusterer> clusterer, std::unique_ptr<DataCover> data_cover,
           std::unique_ptr<Projection> projection, std::unique_ptr<Complex> complex);

    [[nodiscard]] std::vector<Simplex> map(Matrix const& data) ;

private:
    std::unique_ptr<Clusterer> _clusterer;
    std::unique_ptr<DataCover> _data_cover;
    std::unique_ptr<Projection> _projection;
    std::unique_ptr<Complex> _complex;
};

} // Mapper

#endif //MAPPER_H
