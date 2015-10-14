/*! 
 * \file  mfront/include/MFront/Abaqus/Abaqus.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 janv. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ABAQUS_H_
#define LIB_MFRONT_ABAQUS_H_ 

#ifndef TFEL_ABAQUS_INLINE
#define TFEL_ABAQUS_INLINE inline
#endif /* LIB_MFRONT_ABAQUS_H_ */

#ifndef TFEL_ABAQUS_INLINE2
#define TFEL_ABAQUS_INLINE2 inline
#endif /* LIB_MFRONT_ABAQUS_H_ */

#include<type_traits>

#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace abaqus{

  using AbaqusInt  = int;
  using AbaqusReal = double;

  /*!
   * prototype of a function handling stress-free expansion at the
   * beginning of the time step
   *
   * - first  paramater : driving variable
   * - second paramater : driving variable at the end of the time step
   *   or driving variable increment
   * - third paramater  : expansion value at the beginning of the time
   *   step
   * - fourth paramater : expansion value at the end of the time step
   * - third  parameter : spatial dimension
   */
  typedef void (*StressFreeExpansionHandler)(AbaqusReal * const,
					     AbaqusReal * const,
					     const AbaqusReal *const,
					     const AbaqusReal *const,
					     const AbaqusInt);
  /*!
   * \brief class defining the convertion from abaqus to mfront for
   * thermodynamic forces
   * \tparam H: modelling hypothesis
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H>
  struct ImportThermodynamicForces
  {
    //! space dimension
    static constexpr const unsigned short N =
      tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template<typename T>
    static inline void
    exe(tfel::math::stensor<N,T>& s,const AbaqusReal* const v){
      s.importTab(v);
    } // end of exe
  }; // end of struct ImportThermodynamicForces
  /*!
   * \brief partial specialisation of the ImportThermodynamicForces
   * for the plane stress modelling hypothesis.
   */
  template<>
  struct ImportThermodynamicForces<tfel::material::ModellingHypothesis::PLANESTRESS>
  {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template<typename T>
    static inline void
    exe(tfel::math::stensor<2u,T>& s,const AbaqusReal* const v){
      constexpr AbaqusReal cste = tfel::math::constexpr_fct::sqrt(AbaqusReal(2));
      s[0]=v[0];
      s[1]=v[1];
      s[2]=AbaqusReal{0};
      s[3]=v[2]*cste;
    } // end of exe
  }; // end of struct ImportThermodynamicForces
  /*!
   * \brief class defining the convertion from mfront to abaqus for
   * thermodynamic forces
   * \tparam H: modelling hypothesis
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H>
  struct ExportThermodynamicForces
  {
    //! space dimension
    static constexpr const unsigned short N =
      tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] v: values
     * \param[in]  s: symmetric tensor to be exported
     */
    template<typename T>
    static inline void
    exe(AbaqusReal* const v,const tfel::math::stensor<N,T>& s){
      s.exportTab(v);
    } // end of exe
  }; // end of struct ExportThermodynamicForces
  /*!
   * \brief partial specialisation of the ExportThermodynamicForces
   * for the plane stress modelling hypothesis.
   */
  template<>
  struct ExportThermodynamicForces<tfel::material::ModellingHypothesis::PLANESTRESS>
  {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] v: values
     * \param[in]  s: symmetric tensor to be exported
     */
    template<typename T>
    static inline void
    exe(AbaqusReal* const v,const tfel::math::stensor<2u,T>& s){
      constexpr AbaqusReal cste = 1/tfel::math::constexpr_fct::sqrt(AbaqusReal(2));
      v[0]=s[0];
      v[1]=s[1];
      v[2]=s[3]*cste;
    } // end of exe
  }; // end of struct ExportThermodynamicForces
  
} // end of namespace abaqus

// #include"MFront/Abaqus/Abaqus.ixx"

#endif /* LIB_MFRONT_ABAQUS_H_ */










