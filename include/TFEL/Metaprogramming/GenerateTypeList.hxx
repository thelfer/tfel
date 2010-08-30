/*!
 * \file   GenerateTypeList.hxx
 * \see    GenerateTypeList.cxx to see how this file was generated.
 * \brief  This file declares the GenerateTypeList template class.
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_GENERATETYPELIST_H_
#define _LIB_TFEL_GENERATETYPELIST_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/TypeList.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \class GenerateTypeList
     *
     * GenerateTypeList is an helper class which can generate TL containing up to 50 elements. The idea of this class was taken from Andrei Alexandrescu works\latexonly\cite{alexandrescu01:_c_desig,alexandrescu02}\endlatexonly\htmlonly(http://www.ddj.com/dept/cpp/184403813)\endhtmlonly.
     *
     * All template parameters takes the default value TLE.
     *
     * \author Helfer Thomas
     */
    template<typename T0=TLE,typename T1=TLE,
             typename T2=TLE,typename T3=TLE,
             typename T4=TLE,typename T5=TLE,
             typename T6=TLE,typename T7=TLE,
             typename T8=TLE,typename T9=TLE,
             typename T10=TLE,typename T11=TLE,
             typename T12=TLE,typename T13=TLE,
             typename T14=TLE,typename T15=TLE,
             typename T16=TLE,typename T17=TLE,
             typename T18=TLE,typename T19=TLE,
             typename T20=TLE,typename T21=TLE,
             typename T22=TLE,typename T23=TLE,
             typename T24=TLE,typename T25=TLE,
             typename T26=TLE,typename T27=TLE,
             typename T28=TLE,typename T29=TLE,
             typename T30=TLE,typename T31=TLE,
             typename T32=TLE,typename T33=TLE,
             typename T34=TLE,typename T35=TLE,
             typename T36=TLE,typename T37=TLE,
             typename T38=TLE,typename T39=TLE,
             typename T40=TLE,typename T41=TLE,
             typename T42=TLE,typename T43=TLE,
             typename T44=TLE,typename T45=TLE,
             typename T46=TLE,typename T47=TLE,
             typename T48=TLE,typename T49=TLE>
    struct GenerateTypeList;

  } // end of namespace meta

} // end of namespace tfel

#include"TFEL/Metaprogramming/GenerateTypeList.ixx"

#endif /* _LIB_TFEL_GENERATETYPELIST_H_ */
