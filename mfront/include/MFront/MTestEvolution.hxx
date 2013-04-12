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

#include"MFront/MTestTypes.hxx"

namespace mfront
{
  
  /*!
   * Base class for evolutions of external variables
   */
  struct MTestEvolution
  {
    /*!
     * \return the value of the evolution
     * at the given time
     */
    virtual real
    operator()(const real) const = 0;
    //! destructor
    virtual ~MTestEvolution();
  };

  /*!
   * a constant evolution
   */
  struct MTestConstantEvolution
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
    //! destructor
    virtual ~MTestConstantEvolution();
  protected:
    const real value;
  };

  /*!
   * a linear per interval evolution
   */
  struct MTestLPIEvolution
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
    //! destructor
    virtual ~MTestLPIEvolution();
  private:
    std::map<real,real> values;
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTEVOLUTION_H */

