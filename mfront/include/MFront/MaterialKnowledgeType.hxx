/*!
 * \file   MaterialKnowledgeType.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24 avril 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MATERIALKNOWLEDGETYPE_HXX
#define LIB_MFRONT_MATERIALKNOWLEDGETYPE_HXX

namespace mfront {

  /*!
   * \brief a simple structure describing the type of material
   * knowledge treated.
   */
  enum MaterialKnowledgeType {
    MATERIALPROPERTY = 0,
    BEHAVIOUR = 1,
    MODEL = 2
  };  // end of enum MaterialKnowledgeType

}  // end of namespace mfront

#endif /* LIB_MFRONT_MATERIALKNOWLEDGETYPE_HXX */
