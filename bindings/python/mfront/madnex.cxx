/*!
 * \file   bindings/python/mfront/madnex.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/03/2023
 */

#include <pybind11/pybind11.h>

#ifdef MFRONT_HAVE_MADNEX
#include "Madnex/File.hxx"
#include "Madnex/MFrontImplementation.hxx"
#endif /* MFRONT_HAVE_MADNEX */

void declareMadnexSupport(pybind11::module_&);

void declareMadnexSupport(pybind11::module_& m) {
#ifdef MFRONT_HAVE_MADNEX

  auto writeMFrontImplementation = [](const std::string& fn,
                                      const std::string& gn,
                                      const madnex::MFrontImplementation& i) {
    auto f = madnex::File(fn, H5F_ACC_RDWR);
    auto r = f.getRoot();
    auto g = madnex::openGroup(r, gn);
    madnex::write(g, i);
  };

  pybind11::class_<madnex::MFrontMetaData>(m, "MFrontMetaData")
      .def(pybind11::init<>())
      .def_readwrite("author", &madnex::MFrontMetaData::author)
      .def_readwrite("date", &madnex::MFrontMetaData::date)
      .def_readwrite("description", &madnex::MFrontMetaData::description);
  pybind11::class_<madnex::MFrontImplementation>(m, "MFrontImplementation")
      .def(pybind11::init<>())
      .def_readwrite("name", &madnex::MFrontImplementation::name)
      .def_readwrite("source", &madnex::MFrontImplementation::source)
      .def_readwrite("metadata", &madnex::MFrontImplementation::metadata)
      .def_readwrite("parameters", &madnex::MFrontImplementation::parameters)
      .def_readwrite("lower_bounds",
                     &madnex::MFrontImplementation::lower_bounds)
      .def_readwrite("upper_bounds",
                     &madnex::MFrontImplementation::upper_bounds);
  m.def("write", +writeMFrontImplementation);
#else  /* MFRONT_HAVE_MADNEX */
  static_cast<void>(m);
#endif /* MFRONT_HAVE_MADNEX */
}  // end of declareMadnexSupport
