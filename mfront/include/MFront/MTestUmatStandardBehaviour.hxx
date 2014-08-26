/*! 
 * \file   MTestUmatStandardBehaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  18 november 2013
 */

#ifndef _LIB_MFRONT_MTESTSTANDARDUMATBEHAVIOUR_H_
#define _LIB_MFRONT_MTESTSTANDARDUMATBEHAVIOUR_H_ 

#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MFront/MTestUmatBehaviourBase.hxx"

namespace mfront
{

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL MTestUmatStandardBehaviour
    : public MTestUmatBehaviourBase
  {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    MTestUmatStandardBehaviour(const tfel::material::ModellingHypothesis::Hypothesis,
			       const std::string&,
			       const std::string&);
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual MTestStiffnessMatrixType::mtype
    getDefaultStiffnessMatrixType(void) const;
    /*!
     * \brief allocate internal workspace
     * \param[in] h : modelling hypothesis
     */
    virtual void
    allocate(const tfel::material::ModellingHypothesis::Hypothesis);
    /*!
     * destructor
     */
    ~MTestUmatStandardBehaviour();
  protected:
    //! the umat fonction
    tfel::system::UMATFctPtr fct;
    //! local stifness matrix
    mutable tfel::math::matrix<real> D;
    //! temporary vector
    mutable tfel::math::vector<real> iv;
  }; // end of struct MTestBehaviour
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTUMATSTANDARDBEHAVIOUR_H */

