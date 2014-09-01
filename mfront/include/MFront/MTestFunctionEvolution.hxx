/*! 
 * \file  mfront/include/MFront/MTestFunctionEvolution.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MTESTFUNCTIONEVOLUTION_H_
#define _LIB_MFRONT_MTESTFUNCTIONEVOLUTION_H_ 

#include"TFEL/Math/Evaluator.hxx"

#include"MFront/MTestConfig.hxx"
#include"MFront/MTestTypes.hxx"
#include"MFront/MTestEvolution.hxx"

namespace mfront
{

  struct MFRONT_MTEST_VISIBILITY_EXPORT MTestFunctionEvolution
    : public MTestEvolution
  {
    /*!
     * constructor
     * \param[in] : 
     */
    MTestFunctionEvolution(const std::string&,
			   const tfel::utilities::shared_ptr<std::map<std::string,
								      tfel::utilities::shared_ptr<MTestEvolution> > >&);
    /*!
     * \return the value of the evolution
     * at the given time
     */
    virtual real
    operator()(const real) const;
    /*!
     * \return true if the evolution is constant
     */
    virtual bool
    isConstant(void) const;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t  : time
     * \param[in] v  : value
     */
    virtual void setValue(const real,
			  const real);
    //! destructor
    virtual ~MTestFunctionEvolution();
  private:
    //! externally defined evolutions
    tfel::utilities::shared_ptr<std::map<std::string,
					 tfel::utilities::shared_ptr<MTestEvolution> > > evm;
    //! Evaluator
    mutable tfel::math::Evaluator f;
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTFUNCTIONEVOLUTION_H */

