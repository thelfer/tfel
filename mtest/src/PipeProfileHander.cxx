/*!
 * \file   PipeProfileHander.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   14 déc. 2015
 */

#include"MTest/PipeProfile.hxx"
#include"MTest/PipeProfileHandler.hxx"

namespace mtest{

  PipeProfileHandler::PipeProfileHandler() = default;
  PipeProfileHandler::PipeProfileHandler(PipeProfileHandler&&) = default;
  PipeProfileHandler::PipeProfileHandler(const PipeProfileHandler&) = default;
  PipeProfileHandler&
  PipeProfileHandler::operator=(PipeProfileHandler&&) = default;
  PipeProfileHandler&
  PipeProfileHandler::operator=(const PipeProfileHandler&) = default;
  
} // end of namespace mtest
