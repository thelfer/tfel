/*! 
 * \file  MTestCastemEvolution.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTCASTEMEVOLUTION_H_
#define _LIB_MFRONT_MTESTCASTEMEVOLUTION_H_ 

#include<map>
#include<string>
#include<vector>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"

#include"MFront/MTestTypes.hxx"
#include"MFront/MTestEvolution.hxx"

namespace mfront
{
  
  /*!
   * An evolution defined by a castem fonction
   */
  struct MTestCastemEvolution
    : public MTestEvolution
  {
    /*!
     * \brief constructor
     * \param[in] l    : library
     * \param[in] f    : function
     * \param[in] evm_ : externally defined evolutions
     * \param[in] vm_  : externally defined values 
     */
    MTestCastemEvolution(const std::string&,
			 const std::string&,
			 const tfel::utilities::shared_ptr<std::map<std::string,
								    tfel::utilities::shared_ptr<MTestEvolution> > >&);
    /*!
     * \return the value of the CastemEvolution
     * at the given time
     */
    virtual real
    operator()(const real) const;
    /*!
     * \return true if the evolution
     * is constant
     */
    virtual bool isConstant(void) const;
    //! destructor
    virtual ~MTestCastemEvolution();
  protected:
    //! externally defined evolutions
    tfel::utilities::shared_ptr<std::map<std::string,
					 tfel::utilities::shared_ptr<MTestEvolution> > > evm;
    //! the castem function
    tfel::system::CastemFunctionPtr f;
    //! names of the variables
    std::vector<std::string> vnames;
    //! arguments send to the Cast3M function
    mutable std::vector<real> args;
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTCASTEMEVOLUTION_H */

