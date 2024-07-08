//
// Created by jgier on 29.06.2024.
//

#include "CechComplex.h"

#include <cassert>
#include <ranges>
#include <generator>
#include <bits/ranges_algo.h>

namespace MapperLib {

std::vector<Simplex> CechComplex::generate(const std::vector<Mapper::MapperCluster> &clusters) const {
    std::vector<Simplex> result(clusters.size());
    for(SimplexId id = 0; id < clusters.size(); id++){
        result[id] = {{id}};
    }




    return result;
}

CechComplex::CechComplex(const DataCover &data_cover, Dimension max_dimension): _data_cover(data_cover),
_max_dimension(max_dimension) {

}
} // Mapper