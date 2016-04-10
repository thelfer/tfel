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
    using iterator       =
      tfel::math::StridedRandomAccessIterator<T*>;
    //! a simple alias
    using const_iterator =
      tfel::math::StridedRandomAccessIterator< const T*>;
    using diff_const_iterator =
      tfel::math::DifferenceRandomAccessIterator<const_iterator>;
    // time increment
    const T dt;
    const const_iterator props;
    const const_iterator density;
    const T tempOld;
    const const_iterator fieldOld;
    const const_iterator stateOld;
    const T enerInternOld;
    const T enerInelasOld;
    const T tempNew;
    const diff_const_iterator dfield;
    const iterator stateNew;
    T& enerInternNew;
    T& enerInelasNew;
    //! out of bounds policy
    const tfel::material::OutOfBoundsPolicy policy;
  }; // end of struct AbaqusExplicitData
  
} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSEXPLICITDATA_HXX_ */
