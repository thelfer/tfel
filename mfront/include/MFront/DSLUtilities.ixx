/*!
 * \file   MFront/DSLUtilities.ixx
 * \brief
 * \author Thomas Helfer
 * \date   23/04/2025
 */

#ifndef LIB_MFRONT_DSLUTILITIES_IXX
#define LIB_MFRONT_DSLUTILITIES_IXX

namespace mfront {

  template <typename T>
  void read(T& r,
            tfel::utilities::CxxTokenizer::const_iterator& p,
            const tfel::utilities::CxxTokenizer::const_iterator pe) {
    r = read<T>(p, pe);
  }  // end of read

}  // end of namespace mfront

#endif /* LIB_MFRONT_DSLUTILITIES_IXX */
