/*! 
 * \file  mfront/include/MFront/MTestTypes.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTTYPES_H_
#define _LIB_MFRONT_MTESTTYPES_H_ 

#include<map>
#include<string>
#include"TFEL/Utilities/SmartPtr.hxx"

namespace mfront
{
  
  //! a simple alias
  typedef double real;
  
  //! forward declaration
  struct MTestEvolution;
  // ! a simple alias
  typedef tfel::utilities::shared_ptr<MTestEvolution> MTestEvolutionPtr;
  // ! a simple alias
  typedef std::map<std::string,MTestEvolutionPtr> MTestEvolutionManager;

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
      CONSISTENTTANGENTOPERATOR,
      // This one is mostly for the umat behaviour
      ELASTICSTIFNESSFROMMATERIALPROPERTIES,
      UNSPECIFIEDSTIFFNESSMATRIXTYPE
    }; // end of struct StiffnessMatrixType
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTTYPES_H */

