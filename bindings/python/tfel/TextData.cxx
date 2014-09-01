/*! 
 * \file  bindings/python/tfel/TextData.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 mars 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>

#include<TFEL/Utilities/TextData.hxx>

void
declareTextData()
{
  using namespace std;
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::utilities;
  std::vector<double>
    (TextData:: *getColumn)(const unsigned short) const = &TextData::getColumn;

  class_<TextData,noncopyable>("TextData",init<string>())
    .def("getColumn",getColumn)
    ;

} // end of declaretvector
