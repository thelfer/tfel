/*!
 * \file   mfront/include/MFront/BehaviourDSLBase.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTPARSERBASE_HXX
#define LIB_MFRONTPARSERBASE_HXX

#include "MFront/BehaviourDSLCommon.hxx"

namespace mfront {

  template <typename Child>
  struct BehaviourDSLBase : public BehaviourDSLCommon {
    /*!
     * \brief analyse a file
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     * the input file commands
     * \param[in] s : substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    void importFile(const std::string&,
                    const std::vector<std::string>&,
                    const std::map<std::string, std::string>&) override;
    /*!
     * \brief analyse the specified string.
     * \param[in] s : analyse a string
     */
    void analyseString(const std::string&) override;
    /*!
     * \brief return the list of keywords usable with this parser
     * \param[out] k : the list of keywords registred for this parser
     */
    void getKeywordsList(std::vector<std::string>&) const override;

    virtual void addHook(const std::string&, const Hook);

   protected:
    typedef void (Child::*MemberFuncPtr)();
    typedef std::map<std::string, MemberFuncPtr> CallBackContainer;

    BehaviourDSLBase();

    virtual void analyse();

    void registerDefaultCallBacks();

    void treatDisabledCallBack();

    void registerNewCallBack(const std::string&, const MemberFuncPtr);

    void disableCallBack(const std::string&);

    virtual ~BehaviourDSLBase();

    CallBackContainer callBacks;
  };

}  // end of namespace mfront

#include "MFront/BehaviourDSLBase.ixx"

#endif /* LIB_MFRONTPARSERBASE_HXX */
