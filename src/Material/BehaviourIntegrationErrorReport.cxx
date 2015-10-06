/*!
 * \file   BehaviourIntegrationErrorReport.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   05 oct. 2015
 */

#include<vector>
#include<stdexcept>

#include"TFEL/Material/BehaviourIntegrationErrorReport.hxx"

namespace tfel{

  namespace material{

    struct BehaviourIntegrationErrorReportHandler::impl{
      BehaviourIntegrationErrorReport report;
    };
    
    BehaviourIntegrationErrorReportHandler::BehaviourIntegrationErrorReportHandler()
      : imp(new impl)
    {
      if(imp==0){
	throw(std::bad_alloc());
      }
    } // end of BehaviourIntegrationErrorReportHandler::BehaviourIntegrationErrorReportHandler()

    BehaviourIntegrationErrorReportHandler::~BehaviourIntegrationErrorReportHandler()
    {
      delete imp;
    } // end of BehaviourIntegrationErrorReportHandler::~BehaviourIntegrationErrorReportHandler()

    BehaviourIntegrationErrorReport&
    BehaviourIntegrationErrorReportHandler::getBehaviourIntegrationErrorReport()
    {
      return imp->report;
    } // end of BehaviourIntegrationErrorReportHandler::getBehaviourIntegrationErrorReportHandler()
    
  } // end of namespace material

} // end of namespace tfel
