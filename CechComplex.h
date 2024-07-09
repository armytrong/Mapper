//
// Created by jgier on 29.06.2024.
//

#ifndef MAPPER_CECHCOMPLEX_H
#define MAPPER_CECHCOMPLEX_H

#include <vector>
#include <generator>
#include <memory>

#include "typedefs.h"

namespace MapperLib {
class DataCover;

class Complex {
public:
    virtual ~Complex() = default;
    [[nodiscard]] virtual std::vector<Simplex> generate(std::vector<MapperCluster> const &clusters) const = 0;
};

class CechComplex final : public Complex {
public:

    CechComplex(DataCover const& data_cover, Dimension max_dimension);
    [[nodiscard]] std::vector<Simplex> generate(std::vector<MapperCluster> const& clusters) const override;
private:
    [[nodiscard]] std::vector<Simplex> generate_k_simplices(std::vector<MapperCluster> const& clusters, Dimension k) const;

    static std::generator<std::vector<size_t>> generate_k_subsets_of_range(size_t index_max, size_t k);
    static bool check_cluster_intersection(std::vector<MapperCluster const*> const& all_clusters, std::vector<size_t> const& relevant_indices);
    ;

    static std::vector<size_t> get_vector_intersection(std::vector<size_t> vec_1, std::vector<size_t> vec_2);


    DataCover const &_data_cover;
    Dimension _max_dimension;
};

class ComplexFactory
{
public:
    virtual ~ComplexFactory() = default;
    [[nodiscard]] virtual std::unique_ptr<Complex> create_complex(DataCover const& data_cover) const = 0;
};

class CechComplexFactory final : public ComplexFactory
{
public:
    explicit CechComplexFactory(Dimension max_dimension);
    [[nodiscard]] static std::shared_ptr<ComplexFactory> make_shared(Dimension max_dimension);

    [[nodiscard]] std::unique_ptr<Complex> create_complex(DataCover const &data_cover) const override;

private:
    Dimension _max_dimension;
};

} // Mapper

#endif //MAPPER_CECHCOMPLEX_H
