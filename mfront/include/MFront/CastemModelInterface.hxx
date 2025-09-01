/*!
 * \file   mfront/include/MFront/CastemModelInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03/07/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEMMODELINTERFACE_HXX
#define LIB_MFRONT_CASTEMMODELINTERFACE_HXX

#include "MFront/AbstractModelInterface.hxx"

namespace mfront {

  /*!
   * \brief an interface for point-wise models implemented using the
   * `Model` domain specific language based for the `Cast3M` solver.
   *
   * This interface is based on the on-the-fly conversion of the
   * model description in a behaviour description and using the interface for
   * behaviours.
   */
  struct CastemModelInterface final : public AbstractModelInterface {
    //! \return the name of the interface
    static std::string getName();
    //! \return the name of the generated library
    std::string getLibraryName(const ModelDescription &) const;
    //! \brief constructor
    CastemModelInterface();
    //
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string &,
        const std::vector<std::string> &,
        tokens_iterator,
        const tokens_iterator) override;
    void declareReservedNames(std::set<std::string> &) override;
    void writeOutputFiles(const FileDescription &,
                          const ModelDescription &) override;
    void getTargetsDescription(TargetsDescription &,
                               const ModelDescription &) override;
    //! \brief destructor
    ~CastemModelInterface() override;

   protected:
    /*!
     *\brief write the arguments of the `UMAT` function
     * \param[in] os: output stream
     */
    static void writeUMATFunctionArguments(std::ostream &);
    /*!
     * \brief generate the header file
     * \param fd: file description
     * \param md: model description
     */
    virtual void writeHeaderFile(const FileDescription &,
                                 const ModelDescription &);
    /*!
     * \brief generate the source file
     * \param fd: file description
     * \param md: model description
     */
    virtual void writeSourceFile(const FileDescription &,
                                 const ModelDescription &);

  };  // end of struct CastemModelInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_CASTEMMODELINTERFACE_HXX */
