//
// Created by jgier on 27.06.2024.
//

#ifndef MAPPER_PROJECTION_H
#define MAPPER_PROJECTION_H
#include <memory>

#include "typedefs.h"

namespace MapperLib{
/**
 * @class Projection
 * @brief Abstract base class for projection methods
 */
class Projection {
public:
    virtual ~Projection() = default;
    [[nodiscard]] virtual Matrix project(Matrix const& data) const = 0;
};

/**
 * @class CoordinatePlaneProjection
 * @brief projection of data to coordinate planes
 *
 * This implements a very basic projection algorithm. The data is projected to a chosen coordinate hyperplane
 */
class CoordinatePlaneProjection final : public  Projection{
public:
    /**
     * Initialize a projection
     * @param dimensions the axis included in the coordinate hyperplane
     */
    explicit CoordinatePlaneProjection(std::vector<Dimension> dimensions);
    [[nodiscard]] static std::shared_ptr<Projection> make_shared(std::vector<Dimension> dimensions);

    /**
     * project the data to the coordinate hyperplane defined in the constructor
     * @param data the data to project
     * @return a Matrix of the dimension of the hyperplane containing all projected points
     */
    [[nodiscard]] Matrix project (Matrix const& data) const override;
private:
    std::vector<Dimension> _dimensions;
};

} // Mapper

#endif //MAPPER_PROJECTION_H
