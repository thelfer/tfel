/*! 
 * \file  ModelDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 janv. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/ModelDescription.hxx"

namespace mfront
{

  ModelDescription::Function::Function() = default;
  ModelDescription::Function::Function(const ModelDescription::Function&) = default;
  ModelDescription::Function::Function(ModelDescription::Function&&) = default;
  ModelDescription::Function&
  ModelDescription::Function::operator=(const ModelDescription::Function&) = default;
  ModelDescription::Function&
  ModelDescription::Function::operator=(ModelDescription::Function&&) = default;
  ModelDescription::Function::~Function() = default;

  ModelDescription::ModelDescription() = default;
  ModelDescription::ModelDescription(const ModelDescription&) = default;
  ModelDescription::ModelDescription(ModelDescription&&) = default;
  ModelDescription&
  ModelDescription::operator=(const ModelDescription&) = default;
  ModelDescription&
  ModelDescription::operator=(ModelDescription&&) = default;
  ModelDescription::~ModelDescription() = default;
  
} // end of namespace mfront
