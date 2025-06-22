/*!
 * \file   MFrontTemplateGenerationOptions.hxx
 * \brief
 * \author Thomas Helfer
 * \date   07/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTTEMPLATEGENERATIONOPTIONS_HXX
#define LIB_MFRONT_MFRONTTEMPLATEGENERATIONOPTIONS_HXX

#include <vector>
#include <string>

namespace mfront {

  /*!
   * \brief structure holding various options used for the generation
   * of code templates.
   */
  struct MFrontTemplateGenerationOptions {
    //! \brief use unicode symbols
    bool useUnicodeSymbols = true;
    /*!
     * \brief name of the code blocks for which values of external state
     * variables at the middle of the time step must be computed.
     */
    std::vector<std::string> evs_mts;
  };  // end of MFrontTemplateGenerationOptions

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTTEMPLATEGENERATIONOPTIONS_HXX */
