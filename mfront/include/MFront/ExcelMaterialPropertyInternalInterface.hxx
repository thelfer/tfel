/*!
 * \file   mfront/include/MFront/ExcelMaterialPropertyInternalInterface.hxx
 * \brief  This file declares the ExcelMaterialPropertyInternalInterface class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTEXCELLAWINTERNALINTERFACE_H_
#define LIB_MFRONT_MFRONTEXCELLAWINTERNALINTERFACE_H_

#include "MFront/CMaterialPropertyInterface.hxx"

namespace mfront {

  /*!
   * An interface used to generate the dll to be called by excel.
   * This interface is called by the Excel interface: the Excel
   * generates a VBA file who refers to the library that is generated
   * by this interface.
   * The only difference between this interface and the `C` interface
   * is the calling convention used by 32 bits windows: the `C`
   * interface uses the `__cdecl` calling convention whereas the
   * `Excel` interface uses the `__stdcall` one.
   */
  struct ExcelMaterialPropertyInternalInterface
      : public CMaterialPropertyInterface {
    /*!
     * \return the name of this interface
     */
    static std::string getName(void);
    //! constructor
    ExcelMaterialPropertyInternalInterface();
    //! destructor
    virtual ~ExcelMaterialPropertyInternalInterface();

   private:
    virtual std::string getCallingConvention(void) const override;
    /*!
     * \return the name of the generated library
     * \param[in] l: library name (given by the `@Library` keyword)
     * \param[in] m: material name (given by the `@Material` keyword)
     */
    virtual std::string getGeneratedLibraryName(
        const std::string&, const std::string&) const override;
    /*!
     * \brief write the header preprocessor directives
     */
    virtual void writeHeaderPreprocessorDirectives(
        const MaterialPropertyDescription&) override;
    /*!
     * \param[in] m: material name
     * \param[in] c: class name
     */
    virtual std::string getHeaderFileName(const std::string&,
                                          const std::string&) const override;
    /*!
     * \param[in] m: material name
     * \param[in] c: class name
     */
    virtual std::string getSrcFileName(const std::string&,
                                       const std::string&) const override;
  };  // end of MfrontExcelLawInternalInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTEXCELLAWINTERNALINTERFACE_H_ */
