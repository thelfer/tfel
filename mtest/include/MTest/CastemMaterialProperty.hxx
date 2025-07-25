/*!
 * \file   include/MTest/CastemMaterialProperty.hxx
 * \brief
 * \author Thomas Helfer
 * \date   11/08/2021
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CASTEMMATERIALPROPERTY_HXX
#define LIB_MTEST_CASTEMMATERIALPROPERTY_HXX

#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/MaterialPropertyBase.hxx"

namespace mtest {

  /*!
   * \brief material property based on the `Cast3M` interface
   */
  struct CastemMaterialProperty : MaterialPropertyBase {
    /*!
     * \brief constructor
     * \param[in] l: library
     * \param[in] f: function
     */
    CastemMaterialProperty(const std::string&, const std::string&);
    //
    real getValue() const override;
    //! \brief destructor
    ~CastemMaterialProperty() override;

   private:
    //! \brief the castem function
    tfel::system::CastemFunctionPtr fct;
  };  // end of struct CastemMaterialProperty

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMMATERIALPROPERTY_HXX */
