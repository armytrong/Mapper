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
class ComplexFactory;
class Complex;
class Clusterer;
class Projection;

class Mapper {
public:


    Mapper(
        std::shared_ptr<DataCoverFactory> data_cover_factory,
        std::shared_ptr<ComplexFactory> complex_factory,
        std::shared_ptr<Clusterer> clusterer,
        std::shared_ptr<Projection> projection
    );

    [[nodiscard]] std::vector<Simplex> map(Matrix const& data) ;

private:
    std::shared_ptr<DataCoverFactory> _data_cover_factory;
    std::shared_ptr<ComplexFactory> _complex_factory;
    std::shared_ptr<Clusterer> _clusterer;
    std::shared_ptr<Projection> _projection;
    std::unique_ptr<DataCover> _data_cover;
    std::unique_ptr<Complex> _complex;
};

} // Mapper

#endif //MAPPER_H
