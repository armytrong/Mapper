//
// Created by jgier on 27.06.2024.
//

#ifndef MAPPER_PROJECTION_H
#define MAPPER_PROJECTION_H
#include "typedefs.h"

namespace MapperLib{

class Projection {
public:
    virtual ~Projection() = default;
    [[nodiscard]] virtual Matrix project(Matrix const& data) const = 0;
};

class CoordinatePlaneProjection final : public  Projection{
public:
    explicit CoordinatePlaneProjection(std::vector<Dimension> dimensions);
    [[nodiscard]] Matrix project (Matrix const& data) const override;
private:
    std::vector<Dimension> _dimensions;
};

} // Mapper

#endif //MAPPER_PROJECTION_H
