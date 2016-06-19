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

#include "MFront/TargetsDescription.hxx"
#include "MFront/MFrontModelInterface.hxx"

namespace mfront{

  static std::string
  getSrcFileName(const ModelDescription& md){
    if(md.className.empty()){
      throw("getSrcFileName: no class name defined");
    }
    return md.className;
  } // end of getSrcFileName

  static std::string
  getHeaderFileName(const ModelDescription& md){
    return "MFront/"+getSrcFileName(md);
  } // end of getSrcFileName

  
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
    
  void MFrontModelInterface::declareReservedNames(std::set<std::string>&)
  {
    //    names.insert({"inputs"});
  } // end of MFrontModelInterface::declareReservedNames

  void MFrontModelInterface::writeOutputFiles(const FileDescription&,
					      const ModelDescription&)
  {
  } // end of MFrontModelInterface::writeOutputFiles

  void MFrontModelInterface::getTargetsDescription(TargetsDescription& td,
						   const ModelDescription& md)
  {
    const auto lib = "MFrontModel";
    insert_if(td[lib].ldflags,"-lm");
    insert_if(td[lib].sources,getSrcFileName(md)+".cxx");
    insert_if(td.headers,"MFront/"+getHeaderFileName(md)+".hxx");
    insert_if(td[lib].epts,md.className);
  } // end of MFrontModelInterface::getTargetsDescription

  MFrontModelInterface::~MFrontModelInterface() = default;
  
} // end of namespace mfront
