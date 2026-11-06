/*!
 * \file   mfront/include/MFront/GenericMaterialPropertyInterfaceBase.hxx
 * \brief  This file declares the GenericMaterialPropertyInterfaceBase class
 * \author Thomas Helfer
 * \date 06/05/2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICMATERIALPROPERTYINTERFACEBASE_HXX
#define LIB_MFRONT_GENERICMATERIALPROPERTYINTERFACEBASE_HXX

#include <string>
#include <cstddef>
#include <iosfwd>
#include <functional>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  // forward declaration
  struct LibraryDescription;

  /*!
   * \brief a base class for material properties' interfaces such as the
   * `cyrano` and `generic` interfaces.
   */
  struct MFRONT_VISIBILITY_EXPORT GenericMaterialPropertyInterfaceBase
      : public AbstractMaterialPropertyInterface {
    //! \brief default constructor
    GenericMaterialPropertyInterfaceBase();
    /*!
     * \brief append the generated material property to the given library
     * description. This method allows to choose to which library is attached
     * the material property (which is not the case of the
     * `getTargetsDescription` method).
     * \param[in] d: targets description
     * \param[in] l: library description
     * \param[in] mpd: material property description
     */
    virtual void getLibraryDescription(
        TargetsDescription& d,
        LibraryDescription&,
        const MaterialPropertyDescription&) const;
    /*!
     * \return the name of the generated function
     * \param[in] mpd : material property description
     */
    [[nodiscard]] virtual std::string getFunctionName(
        const MaterialPropertyDescription&) const;
    //
    void setOptions(const DataMap&) override;
    void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    [[nodiscard]] std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void writeOutputFiles(const MaterialPropertyDescription&,
                          const FileDescription&) const override;
    /*!
     * \brief description of the check of a bound of a variable, in the code
     * evaluating a material property at a point
     */
    struct BoundCheck {
      //! \brief checked variable
      const VariableDescription& variable;
      /*!
       * \brief rank of the variable: `1` for the first input, the output
       * being after the inputs
       */
      std::size_t rank;
      //! \brief true for a physical bound
      bool physical;
      //! \brief true for an upper bound
      bool upper;
      //! \brief value of the bound
      long double bound;
      /*!
       * \brief identifier of the check (see `getPhysicalBoundCheck` and
       * `getBoundCheck`)
       */
      unsigned long long identifier;
    };
    //! \brief options of the code evaluating a material property at a point
    struct PointEvaluationOptions {
      //! \brief name of the macro disabling the checks of the bounds
      std::string bounds_check_macro;
      //! \brief prefix of the out of bounds policies
      std::string out_of_bounds_policy_prefix;
      /*!
       * \brief write the code reporting a failed check (physical bound, or
       * bound with the strict policy) and ending the evaluation at the point
       */
      std::function<void(std::ostream&, const BoundCheck&)> failure;
      /*!
       * \brief write the code reporting a bound violated with the warning
       * policy: the evaluation continues
       */
      std::function<void(std::ostream&, const BoundCheck&)> warning;
    };
    /*!
     * \brief write the code evaluating a material property at a point,
     * shared by the `generic`, `cyrano` and `generic-parallel` interfaces:
     * physical bounds of the inputs, bounds of the inputs, body of the
     * material property, physical bounds and bounds of the output. The
     * variables are checked in the order of the inputs, the output being
     * the last one. The physical bounds always make the evaluation fail.
     * The bounds make it fail with the strict policy, and are reported with
     * the warning policy: as with the `generic` interface, an unknown policy
     * is treated as the `None` policy. The checks are disabled by the given
     * macro, or not written if the runtime checks are disabled. The
     * variables (inputs, parameters, output) and the out of bounds policy
     * (`mfront_out_of_bounds_policy`) must be declared by the caller.
     * \param[out] os: output stream
     * \param[in] mpd: material property description
     * \param[in] opts: options
     */
    static void writePointEvaluation(std::ostream&,
                                     const MaterialPropertyDescription&,
                                     const PointEvaluationOptions&);
    /*!
     * \return the identifier of the check of a physical bound
     * \param[in] i: index of the variable (the output is after the inputs)
     * \param[in] upper: true for the upper bound
     */
    [[nodiscard]] static unsigned long long getPhysicalBoundCheck(
        const std::size_t, const bool);
    /*!
     * \return the identifier of the check of a bound
     * \param[in] mpd: material property description
     * \param[in] i: index of the variable (the output is after the inputs)
     * \param[in] upper: true for the upper bound
     */
    [[nodiscard]] static unsigned long long getBoundCheck(
        const MaterialPropertyDescription&, const std::size_t, const bool);
    //! \brief identifier of the check of the finiteness of the result
    static constexpr unsigned long long invalid_result_check = 0;
    //! \brief destructor
    ~GenericMaterialPropertyInterfaceBase() override;

    //! \brief types used by the generated functions
    struct TypesDescription {
      std::string real_type;
      std::string integer_type;
      std::string out_of_bounds_policy_type;
      std::string output_status_type;
    };

   protected:
    /*!
     * \brief names and types of the function evaluating a material property
     * at a point, given explicitly so that a derived interface can write the
     * function of the `generic` interface
     */
    struct PointwiseFunctionDescription {
      //! \brief types of the arguments and of the output status
      TypesDescription types;
      //! \brief namespace of the parameters handler
      std::string interface_namespace;
      //! \brief name of the macro disabling the checks of the bounds
      std::string bounds_check_macro;
      //! \brief prefix of the out of bounds policies
      std::string out_of_bounds_policy_prefix;
    };
    /*!
     * \brief write the function evaluating a material property at a point,
     * with the conventions of the `generic` interface (`try` block, `errno`,
     * statuses `-2` to `-6`). The parameters handler and the header
     * declaring the function must have been written.
     * \param[out] os: output stream
     * \param[in] mpd: material property description
     * \param[in] fd: file description
     * \param[in] name: name of the function
     * \param[in] d: names and types
     */
    static void writePointwiseFunction(std::ostream&,
                                       const MaterialPropertyDescription&,
                                       const FileDescription&,
                                       const std::string&,
                                       const PointwiseFunctionDescription&);
    [[nodiscard]] virtual TypesDescription getTypesDescription() const = 0;
    [[nodiscard]] virtual std::vector<std::string> getInterfaceHeaderFiles()
        const = 0;
    [[nodiscard]] virtual std::string getInterfaceName() const = 0;
    [[nodiscard]] virtual std::string getInterfaceNameInCamelCase() const = 0;
    [[nodiscard]] virtual std::string getInterfaceNameInUpperCase() const = 0;
    [[nodiscard]] virtual std::string getOutOfBoundsPolicyEnumerationPrefix()
        const = 0;
    //! \return the name of the header file
    [[nodiscard]] virtual std::string getHeaderFileName(
        const std::string&) const;
    //! \return the name of the source file
    [[nodiscard]] virtual std::string getSourceFileName(
        const std::string&) const;
    /*!
     * \brief generate the output files
     * \param[in] mpd: material property description
     * \param[in] fd: mfront file description
     */
    virtual void writeHeaderFile(const MaterialPropertyDescription&,
                                 const FileDescription&) const;
    /*!
     * \brief generate the output files
     * \param[in] mpd: material property description
     * \param[in] fd:  mfront file description
     */
    virtual void writeSrcFile(const MaterialPropertyDescription&,
                              const FileDescription&) const;
  };  // end of MfrontGenericMaterialPropertyInterfaceBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICMATERIALPROPERTYINTERFACEBASE_HXX */
