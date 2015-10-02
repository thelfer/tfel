/*! 
 * \file  mfront/include/MTest/Evolution.hxx
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

#ifndef LIB_MFRONT_MTESTEVOLUTION_H_
#define LIB_MFRONT_MTESTEVOLUTION_H_ 

#include<map>
#include<vector>

#include"MTest/MTestConfig.hxx"
#include"MTest/MTestTypes.hxx"

namespace mfront
{
  
  /*!
   * Base class for evolutions of external variables
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT Evolution
  {
    /*!
     * \return the value of the evolution
     * at the given time
     */
    virtual real
    operator()(const real) const = 0;
    /*!
     * \return true if the evolution
     * is constant
     */
    virtual bool isConstant(void) const = 0;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t  : time
     * \param[in] v  : value
     * \note most evolution will throw an exception (for example, this
     * method does not makes sense for constant evolutions)
     */
    virtual void setValue(const real,
			  const real) = 0;
    //! destructor
    virtual ~Evolution();
  };

  /*!
   * a constant evolution
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT ConstantEvolution final
    : public Evolution
  {
    /*!
     * constructor
     * \param[in] v : value of the evolution
     */
    ConstantEvolution(const real);
    /*!
     * \return the value of the evolution
     * at the given time
     */
    virtual real
    operator()(const real) const override;
    /*!
     * \return true if the evolution
     * is constant
     */
    virtual bool isConstant(void) const override;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t  : time
     * \param[in] v  : value
     */
    virtual void
    setValue(const real,
	     const real) override;
    //! destructor
    virtual ~ConstantEvolution();
  protected:
    ConstantEvolution& operator=(const ConstantEvolution&) = delete;
    ConstantEvolution& operator=(ConstantEvolution&&) = delete;
    const real value;
  };

  /*!
   * a linear per interval evolution
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT LPIEvolution final
    : public Evolution
  {
    /*!
     * constructor
     * \param[in] t : times
     * \param[in] v : values
     */
    LPIEvolution(const std::vector<real>&,
		      const std::vector<real>&);
    /*!
     * \return the value of the evolution
     * at the given time
     */
    virtual real
    operator()(const real) const override;
    /*!
     * \return true if the evolution
     * is constant
     */
    virtual bool isConstant(void) const override;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t  : time
     * \param[in] v  : value
     */
    virtual void setValue(const real,
			  const real) override;
    //! destructor
    virtual ~LPIEvolution();
  private:
    std::map<real,real> values;
  };

} // end of namespace mfront

#endif /* LIB_MFRONT_MTESTEVOLUTION_H_ */

