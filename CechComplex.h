//
// Created by jgier on 29.06.2024.
//

#ifndef MAPPER_CECHCOMPLEX_H
#define MAPPER_CECHCOMPLEX_H

#include <vector>
#include <coroutine>
#include "typedefs.h"
#include "Mapper.h"

namespace Mapper {

class Complex {
    [[nodiscard]] virtual std::vector<Simplex> generate(std::vector<Mapper::MapperCluster> const &clusters) const = 0;
};

class CechComplex : public Complex {
public:
    ~CechComplex() = default;

    CechComplex(DataCover const &data_cover, Dimension max_dimension);
    [[nodiscard]] std::vector<Simplex> generate(std::vector<Mapper::MapperCluster> const& clusters) const override;

private:
    DataCover const &_data_cover;
    Dimension _max_dimension;


};

} // Mapper

#endif //MAPPER_CECHCOMPLEX_H
