/*! 
 * \file  MTestTypes.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTTYPES_H_
#define _LIB_MFRONT_MTESTTYPES_H_ 

namespace mfront
{
  
  //! a simple alias
  typedef double real;

      /*!
     * \brief possible type of stiffness matrix
     */
  struct MTestStiffnessMatrixType
  {
    //! list of possible matrix type
    enum mtype{
      NOSTIFFNESS,
      ELASTIC,
      SECANTOPERATOR,
      TANGENTOPERATOR,
      CONSISTANTTANGENTOPERATOR,
      // This one is mostly for the umat behaviour
      ELASTICSTIFNESSFROMMATERIALPROPERTIES,
      UNSPECIFIEDSTIFFNESSMATRIXTYPE
    }; // end of struct StiffnessMatrixType
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTTYPES_H */

