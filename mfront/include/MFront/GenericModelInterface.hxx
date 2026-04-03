/*!
 * \file   mfront/include/MFront/GenericModelInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28/04/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICMODELINTERFACE_HXX
#define LIB_MFRONT_GENERICMODELINTERFACE_HXX

#include "MFront/AbstractModelInterface.hxx"

namespace mfront {

  /*!
   * \brief a generic interface for point-wise models implemented using the
   * `Model` domain specific language.
   *
   * This interface is based on the on-the-fly conversion of the
   * model description in a behaviour description and using the interface for
   * behaviours.
   */
  struct GenericModelInterface final : public AbstractModelInterface {
    //! \return the name of the interface
    static std::string getName();
    //! \return the name of the generated library
    std::string getLibraryName(const ModelDescription&) const;
    //! \brief constructor
    GenericModelInterface();
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void declareReservedNames(std::set<std::string>&) override;
    void writeOutputFiles(const FileDescription&,
                          const ModelDescription&) override;
    void getTargetsDescription(TargetsDescription&,
                               const ModelDescription&) override;
    //! \brief destructor
    ~GenericModelInterface() override;

   protected:
    /*!
     * \brief generate the header file
     * \param fd: file description
     * \param md: model description
     */
    virtual void writeHeaderFile(const FileDescription&,
                                 const ModelDescription&);
    /*!
     * \brief generate the source file
     * \param fd: file description
     * \param md: model description
     */
    virtual void writeSourceFile(const FileDescription&,
                                 const ModelDescription&);

  };  // end of struct GenericModelInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICMODELINTERFACE_HXX */
