/*! 
 * \file  OutOfBoundsPolicy.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jan 2010
 */

#ifndef   _LIB_TFEL_MATERIALLAW_OUTOFBOUNDSPOLICY_H_
#define   _LIB_TFEL_MATERIALLAW_OUTOFBOUNDSPOLICY_H_ 

namespace tfel{
  
  namespace materiallaw {

    enum OutOfBoundsPolicy{
      Warning,
      Strict,
      None
    }; // end of enum OutOfBoundsPolicy

  } // end of namespace materiallaw
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIALLAW_OUTOFBOUNDSPOLICY_H */

