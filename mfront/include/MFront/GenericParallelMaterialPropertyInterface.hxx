/*!
 * \file   mfront/include/MFront/GenericParallelMaterialPropertyInterface.hxx
 * \brief  This file declares the GenericParallelMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date 06/05/2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLELMATERIALPROPERTYINTERFACEBASE_HXX
#define LIB_MFRONT_GENERICPARALLELMATERIALPROPERTYINTERFACEBASE_HXX

#include <string>
#include <memory>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/GenericMaterialPropertyInterfaceBase.hxx"

namespace mfront {

  // forward declaration
  struct LibraryDescription;
  namespace generic_parallel::material_property {
    struct AbstractBackend;
  }

  /*!
   * \brief the `generic-parallel` interface for material properties, built
   * on the `generic` interface: the code evaluating a material property at
   * a point is written by `GenericMaterialPropertyInterfaceBase` (see
   * `writePointEvaluation`), the backends writing the batch function and its
   * kernels.
   */
  struct MFRONT_VISIBILITY_EXPORT GenericParallelMaterialPropertyInterface
      : public GenericMaterialPropertyInterfaceBase {
    //! \return the interface name
    static std::string getName();
    //! \brief default constructor
    GenericParallelMaterialPropertyInterface();
    /*!
     * \brief append the generated material property to the given library
     * description. This method allows to choose to which library is attached
     * the material property (which is not the case of the
     * `getTargetsDescription` method).
     * \param[in] d: targets description
     * \param[in] l: library description
     * \param[in] mpd: material property description
     */
    void getLibraryDescription(
        TargetsDescription& d,
        LibraryDescription&,
        const MaterialPropertyDescription&) const override;
    /*!
     * \return the name of the generated function
     * \param[in] mpd : material property description
     */
    [[nodiscard]] std::string getFunctionName(
        const MaterialPropertyDescription&) const override;
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
    //
    [[nodiscard]] TypesDescription getTypesDescription() const override;
    [[nodiscard]] std::vector<std::string> getInterfaceHeaderFiles()
        const override;
    [[nodiscard]] virtual std::string getInterfaceInternalNamespace() const;
    [[nodiscard]] std::string getInterfaceNameInCamelCase() const override;
    [[nodiscard]] std::string getInterfaceNameInUpperCase() const override;
    [[nodiscard]] std::string getOutOfBoundsPolicyEnumerationPrefix()
        const override;
    //! \brief destructor
    ~GenericParallelMaterialPropertyInterface() override;

   protected:
    //
    using AbstractBackend =
        ::mfront::generic_parallel::material_property::AbstractBackend;

    [[nodiscard]] std::string getInterfaceName() const override;

    //! \return the name of the header file
    [[nodiscard]] std::string getHeaderFileName(
        const std::string&) const override;
    //! \return the name of the source file
    [[nodiscard]] std::string getSourceFileName(
        const std::string&) const override;
    /*!
     * \brief generate the output files
     * \param[in] mpd: material property description
     * \param[in] fd: mfront file description
     */
    void writeHeaderFile(const MaterialPropertyDescription&,
                         const FileDescription&) const override;
    /*!
     * \brief generate the output files
     * \param[in] mpd: material property description
     * \param[in] fd:  mfront file description
     */
    void writeSrcFile(const MaterialPropertyDescription&,
                      const FileDescription&) const override;
    //! \brief backend to be used
    std::unique_ptr<const AbstractBackend> backend;

  };  // end of MfrontGenericParallelMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICPARALLELMATERIALPROPERTYINTERFACEBASE_HXX */
