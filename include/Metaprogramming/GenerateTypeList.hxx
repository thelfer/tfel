/*!
 * \file   GenerateTypeList.hxx
 * \see    GenerateTypeList.cxx to see how this file was generated.
 * \brief  This file declares the GenerateTypeList template class.
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_GENERATETYPELIST_H_
#define _LIB_TFEL_GENERATETYPELIST_H_ 

#include"Metaprogramming/TypeList.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \class GenerateTypeList
     *
     * GenerateTypeList is an helper class which can generate TypeList containing up to 50 elements. The idea of this class was taken from Andrei Alexandrescu works\latexonly\cite{alexandrescu01:_c_desig,alexandrescu02}\endlatexonly\htmlonly(http://www.ddj.com/dept/cpp/184403813)\endhtmlonly.
     *
     * All template parameters takes the default value TypeListEndType.
     *
     * \author Helfer Thomas
     */
    template<typename T0=TypeListEndType,typename T1=TypeListEndType,
             typename T2=TypeListEndType,typename T3=TypeListEndType,
             typename T4=TypeListEndType,typename T5=TypeListEndType,
             typename T6=TypeListEndType,typename T7=TypeListEndType,
             typename T8=TypeListEndType,typename T9=TypeListEndType,
             typename T10=TypeListEndType,typename T11=TypeListEndType,
             typename T12=TypeListEndType,typename T13=TypeListEndType,
             typename T14=TypeListEndType,typename T15=TypeListEndType,
             typename T16=TypeListEndType,typename T17=TypeListEndType,
             typename T18=TypeListEndType,typename T19=TypeListEndType,
             typename T20=TypeListEndType,typename T21=TypeListEndType,
             typename T22=TypeListEndType,typename T23=TypeListEndType,
             typename T24=TypeListEndType,typename T25=TypeListEndType,
             typename T26=TypeListEndType,typename T27=TypeListEndType,
             typename T28=TypeListEndType,typename T29=TypeListEndType,
             typename T30=TypeListEndType,typename T31=TypeListEndType,
             typename T32=TypeListEndType,typename T33=TypeListEndType,
             typename T34=TypeListEndType,typename T35=TypeListEndType,
             typename T36=TypeListEndType,typename T37=TypeListEndType,
             typename T38=TypeListEndType,typename T39=TypeListEndType,
             typename T40=TypeListEndType,typename T41=TypeListEndType,
             typename T42=TypeListEndType,typename T43=TypeListEndType,
             typename T44=TypeListEndType,typename T45=TypeListEndType,
             typename T46=TypeListEndType,typename T47=TypeListEndType,
             typename T48=TypeListEndType,typename T49=TypeListEndType>
    struct GenerateTypeList;

  } // end of namespace meta

} // end of namespace tfel

#include"Metaprogramming/GenerateTypeList.ixx"

#endif /* _LIB_TFEL_GENERATETYPELIST_H_ */
