/*! 
 * \file  MTestFunctionEvolution.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTFUNCTIONEVOLUTION_H_
#define _LIB_MFRONT_MTESTFUNCTIONEVOLUTION_H_ 

#include"TFEL/Math/Evaluator.hxx"

#include"MFront/MTestTypes.hxx"
#include"MFront/MTestEvolution.hxx"

namespace mfront
{

  struct MTestFunctionEvolution
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

