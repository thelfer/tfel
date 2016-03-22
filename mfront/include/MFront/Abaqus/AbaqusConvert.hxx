/*!
 * \file   AbaqusConvert.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   22 mars 2016
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSCONVERT_HXX
#define LIB_MFRONT_ABAQUS_ABAQUSCONVERT_HXX

namespace abaqus{

  /*!
   * \brief class defining the convertion from abaqus to mfront for
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
    exe(tfel::math::stensor<N,T>& s,const AbaqusReal* const v){
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
    exe(tfel::math::stensor<2u,T>& s,const AbaqusReal* const v){
#if defined __INTEL_COMPILER
      const AbaqusReal cste =
	tfel::math::constexpr_fct::sqrt(AbaqusReal(2));
#else
      constexpr const AbaqusReal cste =
	tfel::math::constexpr_fct::sqrt(AbaqusReal(2));
#endif
      s[0]=v[0];
      s[1]=v[1];
      s[2]=AbaqusReal{0};
      s[3]=v[2]*cste;
    } // end of exe
  }; // end of struct UMATImportThermodynamicForces
  /*!
   * \brief class defining the convertion from mfront to abaqus for
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
    exe(AbaqusReal* const v,const tfel::math::stensor<N,T>& s){
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
    exe(AbaqusReal* const v,const tfel::math::stensor<2u,T>& s){
#if defined __INTEL_COMPILER
      const AbaqusReal cste =
	1/tfel::math::constexpr_fct::sqrt(AbaqusReal(2));
#else
      constexpr const AbaqusReal cste =
	1/tfel::math::constexpr_fct::sqrt(AbaqusReal(2));
#endif
      v[0]=s[0];
      v[1]=s[1];
      v[2]=s[3]*cste;
    } // end of exe
  }; // end of struct UMATExportThermodynamicForces

} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSCONVERT_HXX_ */
