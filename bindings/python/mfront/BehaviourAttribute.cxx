/*!
 * \file   bindings/python/mfront/BehaviourAttribute.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   04 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "TFEL/Python/MapConverter.hxx"
#include "MFront/BehaviourAttribute.hxx"

template <typename T>
static void add_def(boost::python::class_<mfront::BehaviourAttribute>& w,
                    const std::string& n) {
  using namespace boost::python;
  using namespace mfront;
  bool (BehaviourAttribute::*is_ptr)() const = &BehaviourAttribute::is<T>;
  const T& (BehaviourAttribute::*get_ptr)() const = &BehaviourAttribute::get<T>;
  w.def(("is" + n).c_str(), is_ptr)
      .def(("get" + n).c_str(), get_ptr,
           return_value_policy<copy_const_reference>());
}

void declareBehaviourAttribute();

void declareBehaviourAttribute() {
  using namespace boost::python;
  using namespace mfront;
  using namespace tfel::python;
  class_<BehaviourAttribute> w("BehaviourAttribute");
  add_def<bool>(w, "Bool");
  add_def<unsigned short>(w, "UnsignedShort");
  add_def<std::string>(w, "String");

  initializeMapConverter<std::string, BehaviourAttribute>();
}
