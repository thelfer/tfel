/*! 
 * \file  mfront/include/MTest/CastemEvolution.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MTESTCASTEMEVOLUTION_H_
#define _LIB_MFRONT_MTESTCASTEMEVOLUTION_H_ 

#include<map>
#include<string>
#include<vector>

#include<memory>
#include"TFEL/System/ExternalLibraryManager.hxx"

#include"MTest/MTestConfig.hxx"
#include"MTest/MTestTypes.hxx"
#include"MTest/Evolution.hxx"

namespace mfront
{
  
  /*!
   * An evolution defined by a castem fonction
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT CastemEvolution
    : public Evolution
  {
    /*!
     * \brief constructor
     * \param[in] l    : library
     * \param[in] f    : function
     * \param[in] evm_ : externally defined evolutions
     * \param[in] vm_  : externally defined values 
     */
    CastemEvolution(const std::string&,
			 const std::string&,
			 const std::shared_ptr<std::map<std::string,
								    std::shared_ptr<Evolution> > >&);
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
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t  : time
     * \param[in] v  : value
     */
    virtual void setValue(const real,
			  const real);
    //! destructor
    virtual ~CastemEvolution();
  protected:
    //! externally defined evolutions
    std::shared_ptr<std::map<std::string,
					 std::shared_ptr<Evolution> > > evm;
    //! the castem function
    tfel::system::CastemFunctionPtr f;
    //! names of the variables
    std::vector<std::string> vnames;
    //! arguments send to the Cast3M function
    mutable std::vector<real> args;
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTCASTEMEVOLUTION_H */

