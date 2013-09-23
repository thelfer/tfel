/*! 
 * \file  MTestEvolution.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTEVOLUTION_H_
#define _LIB_MFRONT_MTESTEVOLUTION_H_ 

#include<map>
#include<vector>

#include"MFront/MTestConfig.hxx"
#include"MFront/MTestTypes.hxx"

namespace mfront
{
  
  /*!
   * Base class for evolutions of external variables
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTestEvolution
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
    virtual ~MTestEvolution();
  };

  /*!
   * a constant evolution
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTestConstantEvolution
    : public MTestEvolution
  {
    /*!
     * constructor
     * \param[in] v : value of the evolution
     */
    MTestConstantEvolution(const real);
    /*!
     * \return the value of the evolution
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
    virtual void
    setValue(const real,
	     const real);
    //! destructor
    virtual ~MTestConstantEvolution();
  protected:
    const real value;
  };

  /*!
   * a linear per interval evolution
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTestLPIEvolution
    : public MTestEvolution
  {
    /*!
     * constructor
     * \param[in] t : times
     * \param[in] v : values
     */
    MTestLPIEvolution(const std::vector<real>&,
		      const std::vector<real>&);
    /*!
     * \return the value of the evolution
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
    virtual ~MTestLPIEvolution();
  private:
    std::map<real,real> values;
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTEVOLUTION_H */

