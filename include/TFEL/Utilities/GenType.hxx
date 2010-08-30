/*!
 * \file   GenType.hxx
 * \see    GenerateGenType.cxx to see how this file was generated.
 * \brief  This file declares the GenType template class.
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_GENTYPE_H_
#define _LIB_TFEL_GENTYPE_H_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/TypeList.hxx"
#include"TFEL/Metaprogramming/IsSubClassOf.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"

#include"GenTypeBase.hxx"

namespace tfel{

  namespace utilities{

    //! a simple typedef for reducing the GenType.ixx file size (!).
    typedef tfel::meta::TLE GTEnd;

    /*!
     * \class GenType
     * GenType can hold up to 50 different elements. All template
     * parameters takes the default value GTEnd which is ignored.
     *
     * The GenType class defines several methods which are mostly
     * defines in the GenTypeBase class:
     * - a constructor for every type in argument (except GTEnd).
     * - an assignement operator for every type in argument (except GTEnd).
     * - a getType method
     * - cast operator for every type in argument (except GTEnd).
     * - get and set members for every type in argument (except GTEnd).
     * - specialized accessors for specific types. For example, if
     * double is part of GenType arguments, then the methods
     * getDouble, setDouble are available.
     *
     * For example, GenType<double,int,std::string> defines a GenType that
     * can hold either a double, a int or a string.
     *
     * \pre a type must be declared only once.
     * \see GenTypeBase.
     * \see TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR for defining
     * specialized accessors.
     * \author Helfer Thomas
     * \date 20 Apr. 2007
     */
    template<typename T0=GTEnd,typename T1=GTEnd,
	     typename T2=GTEnd,typename T3=GTEnd,
	     typename T4=GTEnd,typename T5=GTEnd,
	     typename T6=GTEnd,typename T7=GTEnd,
	     typename T8=GTEnd,typename T9=GTEnd,
	     typename T10=GTEnd,typename T11=GTEnd,
	     typename T12=GTEnd,typename T13=GTEnd,
	     typename T14=GTEnd,typename T15=GTEnd,
	     typename T16=GTEnd,typename T17=GTEnd,
	     typename T18=GTEnd,typename T19=GTEnd,
	     typename T20=GTEnd,typename T21=GTEnd,
	     typename T22=GTEnd,typename T23=GTEnd,
	     typename T24=GTEnd,typename T25=GTEnd,
	     typename T26=GTEnd,typename T27=GTEnd,
	     typename T28=GTEnd,typename T29=GTEnd,
	     typename T30=GTEnd,typename T31=GTEnd,
	     typename T32=GTEnd,typename T33=GTEnd,
	     typename T34=GTEnd,typename T35=GTEnd,
	     typename T36=GTEnd,typename T37=GTEnd,
	     typename T38=GTEnd,typename T39=GTEnd,
	     typename T40=GTEnd,typename T41=GTEnd,
	     typename T42=GTEnd,typename T43=GTEnd,
	     typename T44=GTEnd,typename T45=GTEnd,
	     typename T46=GTEnd,typename T47=GTEnd,
	     typename T48=GTEnd,typename T49=GTEnd>
    class GenType;

  } // end of namespace utilities

} // end of namespace tfel

#include"TFEL/Utilities/GenType.ixx"

#include"TFEL/Utilities/GenTypeSpecialisation.ixx"

#endif /* _LIB_TFEL_GENTYPE_H_ */
