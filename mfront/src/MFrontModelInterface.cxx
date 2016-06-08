/*!
 * \file   MFrontModelInterface.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   03 juin 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include "MFront/MFrontModelInterface.hxx"

namespace mfront{

  std::string MFrontModelInterface::getName(void){
    return "mfront";
  } // end of MFrontModelInterface::getName

  MFrontModelInterface::MFrontModelInterface() = default;

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontModelInterface::treatKeyword(const std::string&,
				     TokensContainer::const_iterator p,
				     const TokensContainer::const_iterator)
  {
    return {false,p};
  } // end of MFrontModelInterface::treatKeyword
    
  void MFrontModelInterface::declareReservedNames(std::set<std::string>& names)
  {
    names.insert({"inputs"});
  } // end of MFrontModelInterface::declareReservedNames

  void MFrontModelInterface::writeOutputFiles(const FileDescription&  fd,
					      const ModelDescription& md)
  {
    //    std::cout << "cn : " << md.className << std::endl;
    // std::string material;
    // std::string library;
    // std::string className;
    // for(const auto& v:){}
  } // end of MFrontModelInterface::writeOutputFiles

  void MFrontModelInterface::getTargetsDescription(TargetsDescription& td,
						   const ModelDescription& md)
  {
    
  } // end of MFrontModelInterface::getTargetsDescription

  MFrontModelInterface::~MFrontModelInterface() = default;
  
} // end of namespace mfront
