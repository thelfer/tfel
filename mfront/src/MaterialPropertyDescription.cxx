/*! 
 * \file  MaterialPropertyDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mars 2014
 */

#include"MFront/MaterialPropertyDescription.hxx"

namespace mfront
{

  std::vector<std::string>
  MaterialPropertyDescription::getInputVariableGlossaryNames(void)
  {
    return this->inputs.getGlossaryNames(this->glossaryNames,
					 this->entryNames);
  } // end of MaterialPropertyDescription::getInputVariableGlossaryNames

} // end of namespace mfront
