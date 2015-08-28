/*! 
 * \file  ModelData.cxx
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

#include"MFront/ModelData.hxx"

namespace mfront
{

  ModelData::Function::Function() = default;
  ModelData::Function::Function(const ModelData::Function&) = default;
  ModelData::Function::Function(ModelData::Function&&) = default;
  ModelData::Function&
  ModelData::Function::operator=(const ModelData::Function&) = default;
  ModelData::Function&
  ModelData::Function::operator=(ModelData::Function&&) = default;
  ModelData::Function::~Function() = default;

  ModelData::ModelData() = default;
  ModelData::ModelData(const ModelData&) = default;
  ModelData::ModelData(ModelData&&) = default;
  ModelData&
  ModelData::operator=(const ModelData&) = default;
  ModelData&
  ModelData::operator=(ModelData&&) = default;
  ModelData::~ModelData() = default;
  
} // end of namespace mfront
