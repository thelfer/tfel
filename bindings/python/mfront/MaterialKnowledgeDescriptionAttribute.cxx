/*!
 * \file   bindings/python/mfront/MaterialKnowledgeDescriptionAttribute.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/MaterialKnowledgeAttribute.hxx"

template <typename T>
static void add_def(pybind11::class_<mfront::MaterialKnowledgeAttribute>& w,
                    const std::string& n) {
  using namespace mfront;
  bool (MaterialKnowledgeAttribute::*is_ptr)() const =
      &MaterialKnowledgeAttribute::is<T>;
  const T& (MaterialKnowledgeAttribute::*get_ptr)() const =
      &MaterialKnowledgeAttribute::get<T>;
  w.def(("is" + n).c_str(), is_ptr).def(("get" + n).c_str(), get_ptr);
}

void declareMaterialKnowledgeAttribute(pybind11::module_&);

void declareMaterialKnowledgeAttribute(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<MaterialKnowledgeAttribute> w(m,
                                                 "MaterialKnowledgeAttribute");
  add_def<bool>(w, "Bool");
  add_def<unsigned short>(w, "UnsignedShort");
  add_def<std::string>(w, "String");
}
