/*! 
 * \file  bindings/python/tfel/TextData.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 mars 2013
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
