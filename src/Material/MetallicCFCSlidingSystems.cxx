/*! 
 * \file  MetallicCFCSlidingSystems.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 21 mars 2013
 */

#include"TFEL/Material/MetallicCFCSlidingSystems.hxx"

namespace tfel
{

  namespace material
  {

    MetallicCFCSlidingSystems<1u>&
    MetallicCFCSlidingSystems<1u>::getMetallicCFCSlidingSystems()
    {
      static MetallicCFCSlidingSystems s;
      return s;
    }

    MetallicCFCSlidingSystems<2u>&
    MetallicCFCSlidingSystems<2u>::getMetallicCFCSlidingSystems()
    {
      static MetallicCFCSlidingSystems s;
      return s;
    }

    MetallicCFCSlidingSystems<3u>&
    MetallicCFCSlidingSystems<3u>::getMetallicCFCSlidingSystems()
    {
      static MetallicCFCSlidingSystems s;
      return s;
    }

    MetallicCFCSlidingSystems<1u>::MetallicCFCSlidingSystems()
    {}

    MetallicCFCSlidingSystems<2u>::MetallicCFCSlidingSystems()
    {}

    MetallicCFCSlidingSystems<3u>::MetallicCFCSlidingSystems()
    {}

  }
}
