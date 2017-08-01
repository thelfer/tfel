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
   */
  struct ImportDrivingVariables
  {
    //! space dimension
    static constexpr const unsigned short N = 3u;
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template<typename T>
    static inline void
    exe(tfel::math::stensor<N,T>& e,const CalculiXReal* const v){
      e.importTab(v);
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
  }; // end of struct ImportDrivingVariables
  /*!
   * \brief class defining the convertion from calculix to mfront for
   * thermodynamic forces
   */
  struct ImportThermodynamicForces
  {
    //! space dimension
    static constexpr const unsigned short N = 3u;
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
  }; // end of struct ImportThermodynamicForces
  /*!
   * \brief class defining the convertion from mfront to calculix for
   * thermodynamic forces
   */
  struct ExportThermodynamicForces
  {
    //! space dimension
    static constexpr const unsigned short N = 3u;
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
  }; // end of struct ExportThermodynamicForces

} // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXCONVERT_HXX */
