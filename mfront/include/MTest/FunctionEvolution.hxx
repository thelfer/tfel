/*! 
 * \file  mfront/include/MTest/FunctionEvolution.hxx
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

#ifndef LIB_MFRONT_MTESTFUNCTIONEVOLUTION_H_
#define LIB_MFRONT_MTESTFUNCTIONEVOLUTION_H_ 

#include"TFEL/Math/Evaluator.hxx"

#include"MTest/MTestConfig.hxx"
#include"MTest/MTestTypes.hxx"
#include"MTest/Evolution.hxx"

namespace mfront
{

  struct MFRONT_MTEST_VISIBILITY_EXPORT FunctionEvolution
    : public Evolution
  {
    /*!
     * constructor
     * \param[in] : 
     */
    FunctionEvolution(const std::string&,
			   const std::shared_ptr<std::map<std::string,
							  std::shared_ptr<Evolution> > >&);
    /*!
     * \return the value of the evolution
     * at the given time
     */
    virtual real
    operator()(const real) const override;
    /*!
     * \return true if the evolution is constant
     */
    virtual bool
    isConstant(void) const override;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t  : time
     * \param[in] v  : value
     */
    virtual void setValue(const real,
			  const real) override;
    //! destructor
    virtual ~FunctionEvolution();
  private:
    //! externally defined evolutions
    std::shared_ptr<std::map<std::string,
					 std::shared_ptr<Evolution> > > evm;
    //! Evaluator
    mutable tfel::math::Evaluator f;
  };

} // end of namespace mfront

#endif /* LIB_MFRONT_MTESTFUNCTIONEVOLUTION_H_ */

