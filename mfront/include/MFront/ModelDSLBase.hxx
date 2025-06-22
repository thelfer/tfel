/*!
 * \file   mfront/include/MFront/ModelDSLBase.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04 jun 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTMODELPARSERBASE_HXX
#define LIB_MFRONTMODELPARSERBASE_HXX

#include <map>
#include <string>
#include "MFront/DSLBase.hxx"
#include "MFront/ModelDSLCommon.hxx"

namespace mfront {

  template <typename Child>
  struct ModelDSLBase : public ModelDSLCommon {
    /*!
     * \brief return the list of keywords usable with this parser
     * \param[out] k : the list of keywords registred for this parser
     */
    void getKeywordsList(std::vector<std::string>&) const override;
    /*!
     * \brief analyse a file. This method is called only once, for the
     * main mfront file. The imported files are treated by the import
     * method.
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     * the input file commandes (those commands are given through the
     * --@?? option of the command line
     * \param[in] s : substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    void analyseFile(const std::string&,
                     const std::vector<std::string>&,
                     const std::map<std::string, std::string>&) override;
    /*!
     * \brief analyse the specified string.
     * \param[in] s : analyse a string
     */
    void analyseString(const std::string&) override;
    /*!
     * \brief import a file
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     * the input file commands
     */
    void importFile(const std::string&,
                    const std::vector<std::string>&,
                    const std::map<std::string, std::string>&) override;

   protected:
    typedef void (Child::*MemberFuncPtr)();
    typedef std::map<std::string, MemberFuncPtr> CallBackContainer;
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    ModelDSLBase(const DSLOptions&);

    virtual void analyse();

    virtual void registerDefaultCallBacks();

    virtual void registerNewCallBack(const std::string&, const MemberFuncPtr);

    CallBackContainer callBacks;

    std::set<std::string> registredKeyWords;
  };

}  // end of namespace mfront

#include "MFront/ModelDSLBase.ixx"

#endif /* LIB_MFRONTMODELPARSERBASE_HXX */
