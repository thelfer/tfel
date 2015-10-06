/*!
 * \file   BehaviourIntegrationErrorReport.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   05 oct. 2015
 */

#ifndef _LIB_TFEL_MATERIAL_BEHAVIOURINTEGRATIONERRORREPORT_HXX_
#define _LIB_TFEL_MATERIAL_BEHAVIOURINTEGRATIONERRORREPORT_HXX_

#include<string>
#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace material{

    /*!
     * a structure used to report the behaviour
     * integration status
     */
    struct TFELMATERIAL_VISIBILITY_EXPORT BehaviourIntegrationErrorReport{
      //! List of supported errors 
      enum Status{
	INTERFACEEXCEPTION,
	OUTOFBOUNDSEXCEPTION,
	DIVERGENCEEXCEPTION,
	MATERIALEXCEPTION,
	TFELEXCEPTION,
	STANDARDEXCEPTION,
	UNKNOWNEXCEPTION
      }; // end of enum status
      //! integration status
      Status s;
      //! error description
      std::string e;
    }; // end of struct BehaviourIntegrationErrorReport

    /*!
     * \structure in charge of handling behaviour integration error
     * reports in a multi-threaded environment.
     */
    struct TFELMATERIAL_VISIBILITY_EXPORT BehaviourIntegrationErrorReportHandler
    {
      /*!
       * \return the integration error report associated to the
       * current thread
       */
      BehaviourIntegrationErrorReport&
      getBehaviourIntegrationErrorReport();
      //! constructor
      BehaviourIntegrationErrorReportHandler();
      //! destructor
      ~BehaviourIntegrationErrorReportHandler();
    private:
      //! pimpl design patern
      struct impl;
      //! internal structure
      impl *imp;
    }; // end of struct BehaviourIntegrationErrorReportHandler
    
  } // end of namespace material
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_BEHAVIOURINTEGRATIONERRORREPORT_HXX_ */
