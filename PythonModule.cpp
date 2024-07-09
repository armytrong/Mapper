//
// Created by jakob on 08.07.24.
//
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "CechComplex.h"
#include "SingleLinkage.h"
#include "DataCover.h"
#include "Mapper.h"
#include "typedefs.h"
#include "Projection.h"
namespace MapperLib {


PYBIND11_MODULE(MapperLib, mod)
{
    namespace py = pybind11;
    py::class_<DataCover, std::shared_ptr<DataCover>>(mod, "DataCover")
        .def(py::init<size_t, double, Matrix const&>());
    py::class_<DataCoverFactory, std::shared_ptr<DataCoverFactory>>(mod, "DataCoverFactory")
        .def(py::init<size_t, double>(), py::arg("resulution"), py::arg("overlap"))
        .def("make_shared", &DataCoverFactory::make_shared)
        .def("create_data_cover", &DataCoverFactory::create_data_cover);

    py::class_<Clusterer, std::shared_ptr<Clusterer>>(mod, "Clusterer")
        .def("predict", &Clusterer::predict);
    py::class_<SingleLinkage, Clusterer, std::shared_ptr<SingleLinkage>>(mod, "SingleLinkage")
        .def(py::init<std::optional<int>, std::optional<Scalar>>(),py::arg("num_clusters"), py::arg("distance_threshold"))
        .def("make_shared", &SingleLinkage::make_shared)
        .def("predict", &SingleLinkage::predict);

    py::class_<Complex, std::shared_ptr<Complex>>(mod, "Complex")
        .def("generate", &Complex::generate);
    py::class_<CechComplex, Complex, std::shared_ptr<CechComplex>>(mod, "CechComplex")
        .def(py::init<DataCover const&, Dimension>(), py::arg("data_cover"), py::arg("dimension"))
        .def("generate", &CechComplex::generate);
    py::class_<ComplexFactory, std::shared_ptr<ComplexFactory>>(mod, "ComplexFactory")
        .def("create_complex", &ComplexFactory::create_complex);
    py::class_<CechComplexFactory, ComplexFactory, std::shared_ptr<CechComplexFactory>>(mod, "CechComplexFactory")
        .def(py::init<Dimension>(), py::arg("max_dimension"))
        .def("make_shared", &CechComplexFactory::make_shared)
        .def("create_complex", &CechComplexFactory::create_complex);

    py::class_<Projection, std::shared_ptr<Projection>>(mod, "Projection");
    py::class_<CoordinatePlaneProjection, Projection, std::shared_ptr<CoordinatePlaneProjection>>(mod, "CoordinatePlaneProjection")
        .def(py::init<std::vector<Dimension>>(), py::arg("dimensions"))
        .def("make_shared", &CoordinatePlaneProjection::make_shared);

    py::class_<Mapper>(mod, "Mapper")
        .def(py::init<std::shared_ptr<DataCoverFactory>, std::shared_ptr<ComplexFactory>, std::shared_ptr<Clusterer>, std::shared_ptr<Projection>>(),
            py::arg("data_cover_factory"), py::arg("complex_factory"), py::arg("clusterer"), py::arg("projection"))
        .def("map", &Mapper::map);
}
} //MapperLib