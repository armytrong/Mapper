#include <iostream>
#include <memory>
#include "CechComplex.h"
#include "SingleLinkage.h"
#include "DataCover.h"
#include "Mapper.h"
#include "typedefs.h"
#include "Projection.h"
#include "SLink_SingleLinkage.h"

int main() {
    using namespace MapperLib;
    const Matrix points = {{1, 2}, {3, 4}, {5, 6}, {8, 8}, {9, 9}};
    SingleLinkage clusterer(1,std::nullopt);

    auto clusters = clusterer.predict(points,{0,1,2,3,4});

    for(size_t i = 0; i < clusters.size(); i++) {
        std::cout << "Cluster " << i << ": { ";
        for(auto const point_id: clusters[i]) {
            std::cout << "[" << points[point_id][0] << ", " << points[point_id][1] << "], ";
        }
        std::cout << "\b\b }" << std::endl;
    }

    const Matrix data = {{1,2,3}, {2,3,4}, {-3,-4,-5}, {5,6,7}, {6,7,8}, {6,7,8},{7,8,9},{8,9,10}};


    DataCover const cover(2,0.5,data);
    for (size_t i = 0; i < cover.get_total_num_cubes(); i++){
        std::cout << i << ": " << cover.convert_to_linear_cube_id(cover.convert_to_cube_id(i)) << std::endl;
        std::cout << cover.convert_to_cube_id(i) << std::endl;
    }


    std::cout << cover.get_native_cube_id({8,9,9}) << std::endl;

    auto data_cover_factory = std::make_shared<DataCoverFactory>(2,0.5);
    std::shared_ptr<ComplexFactory> complex_factory = std::make_shared<CechComplexFactory>(2);

    std::shared_ptr<Clusterer> clusterer_ptr = SLink_SingleLinkage::make_shared(10, {});
    std::shared_ptr<Projection> projection_ptr = std::make_shared<CoordinatePlaneProjection>(std::vector<size_t>({0,1}));

    Mapper mapper(data_cover_factory, complex_factory, clusterer_ptr, projection_ptr);
    auto const simplices = mapper.map(data);
    std::cout << "Simplices: " << std::endl;
    for (auto const& simplex : simplices) {
        std::cout << simplex << std::endl;
    }

    return 0;
}
