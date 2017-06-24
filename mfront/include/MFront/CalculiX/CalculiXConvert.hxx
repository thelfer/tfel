/*!
 * \file   CalculiXConvert.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   22 mars 2016
 */

#ifndef LIB_MFRONT_CALCULIX_CALCULIXCONVERT_HXX
#define LIB_MFRONT_CALCULIX_CALCULIXCONVERT_HXX

#include"TFEL/Math/General/MathConstants.hxx"

namespace calculix{

  /*!
   * \brief class defining the convertion from calculix to mfront for
   * driving variables
   * \tparam H: modelling hypothesis
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H>
  struct UMATImportDrivingVariables
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
    exe(tfel::math::stensor<N,T>& e,const CalculiXReal* const v){
      e.importVoigt(v);
    } // end of exe
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template<typename T>
    static inline void
    exe(tfel::math::tensor<N,T>& F,const CalculiXReal* const v){
      tfel::math::tensor<N,T>::buildFromFortranMatrix(F,v);
    } // end of exe
  }; // end of struct UMATImportDrivingVariables
  /*!
   * \brief partial specialisation of the UMATImportDrivingVariables
   * for the plane stress modelling hypothesis.
   */
  template<>
  struct UMATImportDrivingVariables<tfel::material::ModellingHypothesis::PLANESTRESS>
  {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template<typename T>
    static inline void
    exe(tfel::math::stensor<2u,T>& e,const CalculiXReal* const v){
      constexpr const auto icste = tfel::math::Cste<CalculiXReal>::isqrt2;
      e[0]=v[0];
      e[1]=v[1];
      e[2]=CalculiXReal{0};
      e[3]=v[2]*icste;
    } // end of exe
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template<typename T>
    static inline void
    exe(tfel::math::tensor<2u,T>& F,const CalculiXReal* const v){
      tfel::math::tensor<2u,T>::buildFromFortranMatrix(F,v);
    } // end of exe
  }; // end of struct UMATImportDrivingVariables
  /*!
   * \brief class defining the convertion from calculix to mfront for
   * thermodynamic forces
   * \tparam H: modelling hypothesis
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H>
  struct UMATImportThermodynamicForces
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
    exe(tfel::math::stensor<N,T>& s,const CalculiXReal* const v){
      s.importTab(v);
    } // end of exe
  }; // end of struct UMATImportThermodynamicForces
  /*!
   * \brief partial specialisation of the UMATImportThermodynamicForces
   * for the plane stress modelling hypothesis.
   */
  template<>
  struct UMATImportThermodynamicForces<tfel::material::ModellingHypothesis::PLANESTRESS>
  {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template<typename T>
    static inline void
    exe(tfel::math::stensor<2u,T>& s,const CalculiXReal* const v){
      constexpr const auto cste = tfel::math::Cste<CalculiXReal>::sqrt2;
      s[0]=v[0];
      s[1]=v[1];
      s[2]=CalculiXReal{0};
      s[3]=v[2]*cste;
    } // end of exe
  }; // end of struct UMATImportThermodynamicForces
  /*!
   * \brief class defining the convertion from mfront to calculix for
   * thermodynamic forces
   * \tparam H: modelling hypothesis
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H>
  struct UMATExportThermodynamicForces
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
    exe(CalculiXReal* const v,const tfel::math::stensor<N,T>& s){
      s.exportTab(v);
    } // end of exe
  }; // end of struct UMATExportThermodynamicForces
  /*!
   * \brief partial specialisation of the UMATExportThermodynamicForces
   * for the plane stress modelling hypothesis.
   */
  template<>
  struct UMATExportThermodynamicForces<tfel::material::ModellingHypothesis::PLANESTRESS>
  {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] v: values
     * \param[in]  s: symmetric tensor to be exported
     */
    template<typename T>
    static inline void
    exe(CalculiXReal* const v,const tfel::math::stensor<2u,T>& s){
      constexpr const auto icste = tfel::math::Cste<CalculiXReal>::isqrt2;
      v[0]=s[0];
      v[1]=s[1];
      v[2]=s[3]*icste;
    } // end of exe
  }; // end of struct UMATExportThermodynamicForces

} // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXCONVERT_HXX */
