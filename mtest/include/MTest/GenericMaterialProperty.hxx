/*!
 * \file   include/MTest/GenericMaterialProperty.hxx
 * \brief
 * \author Thomas Helfer
 * \date   11/08/2021
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_GENERICMATERIALPROPERTY_HXX
#define LIB_MTEST_GENERICMATERIALPROPERTY_HXX

#include "MFront/GenericMaterialProperty/MaterialProperty.h"
#include "MTest/MaterialPropertyBase.hxx"

namespace mtest {

  //! \brief material property based on the `generic` interface
  struct GenericMaterialProperty : MaterialPropertyBase {
    /*!
     * \brief constructor
     * \param[in] l: library
     * \param[in] f: function
     */
    GenericMaterialProperty(const std::string&, const std::string&);
    //
    real getValue() const override;
    //! \brief destructor
    ~GenericMaterialProperty() override;

   private:
    //! \brief the castem function
    ::mfront_gmp_MaterialPropertyPtr fct;
  };  // end of struct GenericMaterialProperty

}  // end of namespace mtest

#endif /* LIB_MTEST_GENERICMATERIALPROPERTY_HXX */
