/*! 
 * \file  mfront/include/MFront/UMATFiniteStrainMTestFileGenerator.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juil. 2013
 */

#ifndef _LIB_UMATFINITESTRAINMTESTFILEGENERATOR_H_
#define _LIB_UMATFINITESTRAINMTESTFILEGENERATOR_H_ 

#include"MFront/MFrontConfig.hxx"
#include"MFront/MTestFileGeneratorBase.hxx"

namespace mfront
{

  struct MTESTFILEGENERATOR_VISIBILITY_EXPORT UMATFiniteStrainMTestFileGenerator
    : public mfront::MTestFileGeneratorBase
  {
    /*!
     * constructor
     * \param[in] i : interface
     * \param[in] l : library
     * \param[in] b : behaviour
     */
    UMATFiniteStrainMTestFileGenerator(const std::string&,
				      const std::string&,
				      const std::string&);
    /*!
     * set the deformation gradient at the beginning of the time step
     * \param[in] F : deformation gradient
     */
    virtual void setDeformationGradientAtTheBeginningOfTheStimeStep(const double* const);
    /*!
     * set the deformation gradient at the end of the time step
     * \param[in] F : deformation gradient
     */
    virtual void setDeformationGradientAtTheEndOfTheStimeStep(const double* const);
    /*!
     * set stresses
     * \param[in] s : stresses
     */
    virtual void setStressTensor(const double* const);
    /*!
     * destructor
     */
    virtual ~UMATFiniteStrainMTestFileGenerator();
  protected:
    /*!
     * write behaviour declaration
     * \param[in] os : output stream
     */
    virtual void
    writeBehaviourDeclaration(std::ostream&) const;
    /*!
     * write driving variables
     * \param[in] os : output stream
     */
    virtual void
    writeDrivingVariables(std::ostream&) const;
    //! interface name
    std::string interface;
    //! library name
    std::string library;
    //! behaviour name
    std::string behaviour;
    //! deformation gradient at the beginning of the time step
    double F0[36];
    //! deformation gradient at the end of the time step
    double F1[36];
    //! stress increment
    double stress[6];
  }; // end of struct UMATFiniteStrainMTestFileGenerator

} // end of namespace mfront

#endif /* _LIB_UMATFINITESTRAINMTESTFILEGENERATOR_H */

