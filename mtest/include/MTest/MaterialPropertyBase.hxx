/*!
 * \file   include/MTest/MaterialPropertyBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10/08/2021
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MATERIALPROPERTYBASE_HXX
#define LIB_MTEST_MATERIALPROPERTYBASE_HXX

#include <vector>
#include <string>
#include "TFEL/System/ExternalMaterialPropertyDescription.hxx"
#include "MTest/Types.hxx"
#include "MTest/Config.hxx"
#include "MTest/MaterialProperty.hxx"

namespace mtest {

  /*!
   * \brief a base class for material properties.
   *
   * This class handles a buffer to store variables' value and a map between
   * variables names and variables offset.
   */
  struct MTEST_VISIBILITY_EXPORT MaterialPropertyBase
      : public MaterialProperty,
        protected tfel::system::ExternalMaterialPropertyDescription {
    /*!
     * \brief constructor
     * \param[in] l: library
     * \param[in] f: function
     */
    MaterialPropertyBase(const std::string&, const std::string&);
    //
    std::size_t getNumberOfVariables() const override;
    std::vector<std::string> getVariablesNames() const override;
    std::vector<std::string> getParametersNames() const override;
    std::string getOutputName() const override;
    void setVariableValue(const std::string&, const real) override;
    void setVariableValue(const std::size_t, const real) override;
    void setParameter(const std::string&, const real) override;
    //! \brief destructor
    ~MaterialPropertyBase() override;

   protected:
    //! \brief variables values
    std::vector<real> arguments_values;
  };  // end of MaterialPropertyBase

}  // end of namespace mtest

#endif /* LIB_MTEST_MATERIALPROPERTYBASE_HXX */
