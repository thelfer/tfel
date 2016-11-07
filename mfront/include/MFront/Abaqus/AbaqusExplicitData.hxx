/*!
 * \file   AbaqusExplicitData.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   29 mars 2016
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSEXPLICITDATA_HXX
#define LIB_MFRONT_ABAQUS_ABAQUSEXPLICITDATA_HXX

#include"TFEL/Math/General/StridedRandomAccessIterator.hxx"
#include"TFEL/Math/General/DifferenceRandomAccessIterator.hxx"
#include"TFEL/Material/OutOfBoundsPolicy.hxx"

namespace abaqus{

  template<typename T>
  struct AbaqusExplicitData{
    //! a simple alias
    using const_iterator = const T*;
    //! a simple alias
    using strided_iterator =
      tfel::math::StridedRandomAccessIterator<T*>;
    //! a simple alias
    using strided_const_iterator =
      tfel::math::StridedRandomAccessIterator< const T*>;
    using diff_strided_const_iterator =
      tfel::math::DifferenceRandomAccessIterator<strided_const_iterator>;
    //! time increment
    const T dt;
    //! material properties
    const const_iterator props;
    /*!
     * \brief properties used to compute the stiffness tensor or the
     * thermal expansion, if required (see the
     * `@RequireStiffnessTensor` or `@RequireThermalExpansionTensor`
     * `MFront` keywords)
     */
    const const_iterator eth_props;
    const T density;
    const T tempOld;
    const strided_const_iterator fieldOld;
    const strided_const_iterator stateOld;
    const T enerInternOld;
    const T enerInelasOld;
    const T tempNew;
    const diff_strided_const_iterator dfield;
    const strided_iterator stateNew;
    T& enerInternNew;
    T& enerInelasNew;
    //! out of bounds policy
    const tfel::material::OutOfBoundsPolicy policy;
  }; // end of struct AbaqusExplicitData
  
} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSEXPLICITDATA_HXX_ */
