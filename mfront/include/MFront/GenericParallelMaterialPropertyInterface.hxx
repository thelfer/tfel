/*!
 * \file   mfront/include/MFront/GenericParallelMaterialPropertyInterface.hxx
 * \brief  This file declares the GenericParallelMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   06 mai 2008
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
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  // forward declaration
  struct LibraryDescription;
  namespace generic_parallel::material_property {
    struct AbstractBackend;
  }

  /*!
   * \brief a base class for material properties' interfaces such as the
   * `cyrano` and `generic` interfaces.
   */
  struct MFRONT_VISIBILITY_EXPORT GenericParallelMaterialPropertyInterface
      : public AbstractMaterialPropertyInterface {
    //! \brief list of types used by the interface
    struct TypesDescription {
      std::string real_type;
      std::string integer_type;
      std::string out_of_bounds_policy_type;
      std::string output_status_type;
    };
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
    virtual void getLibraryDescription(
        TargetsDescription& d,
        LibraryDescription&,
        const MaterialPropertyDescription&) const;
    /*!
     * \return the name of the generated function
     * \param[in] mpd : material property description
     */
    virtual std::string getFunctionName(
        const MaterialPropertyDescription&) const;
    //
    void setOptions(const DataMap&) override;
    void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void writeOutputFiles(const MaterialPropertyDescription&,
                          const FileDescription&) const override;
    //
    virtual TypesDescription getTypesDescription() const;
    virtual std::vector<std::string> getInterfaceHeaderFiles() const;
    virtual std::string getInterfaceInternalNamespace() const;
    virtual std::string getInterfaceNameInCamelCase() const;
    virtual std::string getInterfaceNameInUpperCase() const;
    virtual std::string getOutOfBoundsPolicyEnumerationPrefix() const;
    //! \brief destructor
    ~GenericParallelMaterialPropertyInterface() override;

   protected:
    //
    using AbstractBackend =
        ::mfront::generic_parallel::material_property::AbstractBackend;

    virtual std::string getInterfaceName() const;

    //! \return the name of the header file
    virtual std::string getHeaderFileName(const std::string&) const;
    //! \return the name of the source file
    virtual std::string getSourceFileName(const std::string&) const;
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

    //! \brief backend to be used
    const std::unique_ptr<const AbstractBackend> backend;

  };  // end of MfrontGenericParallelMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICPARALLELMATERIALPROPERTYINTERFACEBASE_HXX */
