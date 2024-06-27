//
// Created by jgier on 27.06.2024.
//

#ifndef MAPPER_PROJECTION_H
#define MAPPER_PROJECTION_H
#include "typedefs.h"

namespace Mapper {

class Projection {
    [[nodiscard]] virtual Matrix project(Matrix const& data) const = 0;
};

class CoordinatePlaneProjection : public  Projection{
    explicit CoordinatePlaneProjection(std::vector<size_t> dimensions);
    [[nodiscard]] Matrix project (Matrix const& data) const override;
private:
    std::vector<size_t> _dimensions;
};

} // Mapper

#endif //MAPPER_PROJECTION_H
