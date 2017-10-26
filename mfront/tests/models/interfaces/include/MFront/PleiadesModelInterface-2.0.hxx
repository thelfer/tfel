/*!
 * \file   PleiadesModelInterface.hxx
 * \brief  This file declares the PleiadesModelInterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONT_PLEIADESMODELINTERFACE_H_
#define _LIB_MFRONT_PLEIADESMODELINTERFACE_H_

#include "MFront/AbstractModelInterface.hxx"
#include "MFront/FileDescription.hxx"

namespace mfront {

/*!
 * Implements a interface for the PLEIADES 2.0 kernel
 */
struct PleiadesModelInterface : public AbstractModelInterface {

  //!default constructor
  PleiadesModelInterface();

  //! \return the name of the interface
  static std::string getName(void);

  virtual std::pair<bool, TokensContainer::const_iterator> treatKeyword(
      const std::string&, const std::vector<std::string>&, TokensContainer::const_iterator,
      const TokensContainer::const_iterator) override;

  /*! declare reserved names. Those are variables used internally
  * that may conflicts with user variables
  * \param[in] n : reserved names
  */
  virtual void declareReservedNames(std::set<std::string>&) override;

  /*!
   * \brief write the output files
   * \param pdata : processing data
   * \param data  : PleiadesModel data
   */
  virtual void writeOutputFiles(const FileDescription&, const ModelDescription&) override;

  virtual void getTargetsDescription(TargetsDescription&, const ModelDescription&) override;

  virtual ~PleiadesModelInterface();
};
// end of class PleiadesModelInterface

}  // end of namespace mfront

#endif /* _LIB_MFRONTPLEIADESMODELINTERFACE_H */
