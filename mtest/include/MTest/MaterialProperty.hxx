/*!
 * \file   MaterialProperty.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10/08/2021
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MATERIALPROPERTY_HXX
#define LIB_MTEST_MATERIALPROPERTY_HXX

#include <memory>
#include <string>
#include <vector>
#include <cstddef>
#include "MTest/Types.hxx"
#include "MTest/Config.hxx"

namespace mtest {

  //! \brief base class for material properties
  struct MTEST_VISIBILITY_EXPORT MaterialProperty {
    /*!
     * \brief build a material property
     * \param[in] i: interface
     * \param[in] l: library
     * \param[in] f: function
     */
    static std::shared_ptr<MaterialProperty> getMaterialProperty(
        const std::string&, const std::string&, const std::string&);
    //! \return the names of the arguments
    virtual std::size_t getNumberOfVariables() const = 0;
    //! \return the names of the arguments
    virtual std::vector<std::string> getVariablesNames() const = 0;
    //! \return the names of the parameters
    virtual std::vector<std::string> getParametersNames() const = 0;
    //! \return the names of the output
    virtual std::string getOutputName() const = 0;
    /*!
     * \brief set variable value
     * \param[in] n: name of the variable
     * \param[in] v: value the variable
     */
    virtual void setVariableValue(const std::string&, const real) = 0;
    /*!
     * \brief set variable value
     * \param[in] p: position of the variable
     * \param[in] v: value the variable
     */
    virtual void setVariableValue(const std::size_t, const real) = 0;
    //! \brief return the value of the material property
    virtual real getValue() const = 0;
    /*!
     * \brief set the value of a parameter
     * \param[in] n: name of the variable
     * \param[in] v: value the variable
     */
    virtual void setParameter(const std::string&, const real) = 0;
    //! \brief destructor
    virtual ~MaterialProperty();
  };  // end of MaterialProperty

  /*!
   * \return the value of a material property for the given arguments
   * \param[in] mp: material property
   * \param[in] args: mapping between values and arguments
   * \param[in] b: allow missing arguments
   */
  MTEST_VISIBILITY_EXPORT real getValue(MaterialProperty&,
                                        const std::map<std::string, real>&,
                                        const bool = false);
  /*!
   * \return the value of a material property for the given arguments
   * \param[in] mp: material property
   * \param[in] values: argument values
   */
  MTEST_VISIBILITY_EXPORT real getValue(MaterialProperty&,
                                        const std::vector<real>&);
  /*!
   * \return the value of a material property for the given argument
   * \param[in] mp: material property
   * \param[in] v: argument value
   */
  MTEST_VISIBILITY_EXPORT real getValue(MaterialProperty&, const real);
  /*!
   * \return the value of a constant material property
   * \param[in] mp: material property
   */
  MTEST_VISIBILITY_EXPORT real getValue(MaterialProperty&);

}  // namespace mtest

#endif /* LIB_MTEST_MATERIALPROPERTY_HXX */
