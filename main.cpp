#include <iostream>

#include "SingleLinkage.h"
#include "DataCover.h"
#include "typedefs.h"

int main() {
    using namespace Mapper;
    const Matrix points = {{1, 2}, {3, 4}, {5, 6}, {8, 8}, {9, 9}};
    SingleLinkage clusterer(std::nullopt, 3);

    auto clusters = clusterer.predict(points);

    for(int i = 0; i < clusters.size(); i++) {
        std::cout << "Cluster " << i << ": { ";
        for(auto const point_id: clusters[i]) {
            std::cout << "[" << points[point_id][0] << ", " << points[point_id][1] << "], ";
        }
        std::cout << "\b\b }" << std::endl;
    }

    const Matrix data = {{1,2,3}, {2,3,4}, {3,4,5}, {5,6,7}, {6,7,8}, {6,7,8},{7,8,9},{8,9,10}};


    DataCover const cover(3,0.5,data);
    for (size_t i = 0; i < cover.get_total_num_cubes(); i++){
        std::cout << i << ": " << cover.convert_to_linear_cube_id(cover.convert_to_cube_id(i)) << std::endl;
    }

    std::cout << cover.get_native_cube_id({9,9,9}) << std::endl;

    return 0;
}
