/*!
 * \file   include/MTest/CyranoMaterialProperty.hxx
 * \brief
 * \author Thomas Helfer
 * \date   11/08/2021
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CYRANOMATERIALPROPERTY_HXX
#define LIB_MTEST_CYRANOMATERIALPROPERTY_HXX

#include "Cyrano/MaterialProperty.hxx"
#include "MTest/MaterialPropertyBase.hxx"

namespace mtest {

  /*!
   * \brief material property based on the `Cyrano` interface
   */
  struct CyranoMaterialProperty : MaterialPropertyBase {
    /*!
     * \brief constructor
     * \param[in] l: library
     * \param[in] f: function
     */
    CyranoMaterialProperty(const std::string&, const std::string&);
    //
    real getValue() const override;
    //! \brief destructor
    ~CyranoMaterialProperty() override;

   private:
    //! \brief the castem function
    ::CyranoMaterialPropertyPtr fct;
  };  // end of struct CyranoMaterialProperty

}  // end of namespace mtest

#endif /* LIB_MTEST_CYRANOMATERIALPROPERTY_HXX */
