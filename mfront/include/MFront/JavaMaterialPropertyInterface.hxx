/*!
 * \file   mfront/include/MFront/JavaMaterialPropertyInterface.hxx
 * \brief  This file declares the JavaMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_JAVAMATERIALPROPERTYINTERFACE_HXX
#define LIB_MFRONT_JAVAMATERIALPROPERTYINTERFACE_HXX

#include <string>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  struct JavaMaterialPropertyInterface
      : public AbstractMaterialPropertyInterface {
    //! \return the interface name
    static std::string getName();
    //! constructor
    JavaMaterialPropertyInterface();
    /*!
     * \param[in] k  : keyword treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] d   : target description
     * \param[in]  mpd : material property description
     */
    virtual void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeOutputFiles(const MaterialPropertyDescription&,
                                  const FileDescription&) const override;
    //! destructor
    virtual ~JavaMaterialPropertyInterface();

   private:
    //! package name
    std::string package;
  };  // end of JavaMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_JAVAMATERIALPROPERTYINTERFACE_HXX */
