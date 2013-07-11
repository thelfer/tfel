/*! 
 * \file  UMATSmallStrainMTestFileGenerator.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juil. 2013
 */

#ifndef _LIB_UMATSMALLSTRAINMTESTFILEGENERATOR_H_
#define _LIB_UMATSMALLSTRAINMTESTFILEGENERATOR_H_ 

#include"MFront/MFrontConfig.hxx"
#include"MFront/MTestFileGeneratorBase.hxx"

namespace mfront
{

  struct MTESTFILEGENERATOR_VISIBILITY_EXPORT UMATSmallStrainMTestFileGenerator
    : public mfront::MTestFileGeneratorBase
  {
    /*!
     * constructor
     * \param[in] i : interface
     * \param[in] l : library
     * \param[in] b : behaviour
     */
    UMATSmallStrainMTestFileGenerator(const std::string&,
				      const std::string&,
				      const std::string&);
    /*!
     * set the strain at the beginning of the time step
     * \param[in] e : strain
     */
    virtual void setStrainTensor(const double* const);
    /*!
     * set the strain increment
     * \param[in] e : strain increments
     */
    virtual void setStrainTensorIncrement(const double* const);
    /*!
     * set stresses
     * \param[in] s : stresses
     */
    virtual void setStressTensor(const double* const);
    /*!
     * destructor
     */
    virtual ~UMATSmallStrainMTestFileGenerator();
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
    //! strain
    double eto[6];
    //! strain increment
    double deto[6];
    //! stress increment
    double stress[6];
  }; // end of struct UMATSmallStrainMTestFileGenerator

} // end of namespace mfront

#endif /* _LIB_UMATSMALLSTRAINMTESTFILEGENERATOR_H */

