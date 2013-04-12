/*! 
 * \file  ExternalFunctionsPrototypes.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 26 jan 2010
 */

#ifndef _LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_H_
#define _LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_H_ 

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef UNIX32
  typedef int    UMATIntegerType;
  typedef double UMATRealType;
  typedef int    AsterIntegerType;
  typedef double AsterRealType;
#endif 
#ifdef UNIX64
  typedef long   UMATIntegerType;
  typedef double UMATRealType;
  typedef long   AsterIntegerType;
  typedef double AsterRealType;
#endif

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#ifdef	__cplusplus

namespace tfel
{

  namespace system
  {

    extern "C" {

      //! a simple alias
      typedef double (*CastemFunctionPtr)(const double*);

      //! a simple alias
      typedef double (*CFunction0Ptr)(void);

      //! a simple alias
      typedef double (*CFunction1Ptr)(double);

      //! a simple alias
      typedef double (*CFunction2Ptr)(double,double);

      //! a simple alias
      typedef double (*CFunction3Ptr)(double,double,double);

      //! a simple alias
      typedef double (*CFunction4Ptr)(double,double,double,
				      double);

      //! a simple alias
      typedef double (*CFunction5Ptr)(double,double,double,
				      double,double);

      //! a simple alias
      typedef double (*CFunction6Ptr)(double,double,double,
				      double,double,double);

      //! a simple alias
      typedef double (*CFunction7Ptr)(double,double,double,
				      double,double,double,
				      double);

      //! a simple alias
      typedef double (*CFunction8Ptr)(double,double,double,
				      double,double,double,
				      double,double);

      //! a simple alias
      typedef double (*CFunction9Ptr)(double,double,double,
				      double,double,double,
				      double,double,double);

      //! a simple alias
      typedef double (*CFunction10Ptr)(double,double,double,
				       double,double,double,
				       double,double,double,
				       double);

      //! a simple alias
      typedef double (*CFunction11Ptr)(double,double,double,
				       double,double,double,
				       double,double,double,
				       double,double);

      //! a simple alias
      typedef double (*CFunction12Ptr)(double,double,double,
				       double,double,double,
				       double,double,double,
				       double,double,double);

      //! a simple alias
      typedef double (*CFunction13Ptr)(double,double,double,
				       double,double,double,
				       double,double,double,
				       double,double,double,
				       double);

      //! a simple alias
      typedef double (*CFunction14Ptr)(double,double,double,
				       double,double,double,
				       double,double,double,
				       double,double,double,
				       double,double);

      //! a simple alias
      typedef double (*CFunction15Ptr)(double,double,double,
				       double,double,double,
				       double,double,double,
				       double,double,double,double,
				       double,double);

      //! a simple alias
      typedef double (*FortranFunction0Ptr)(void);

      //! a simple alias
      typedef double (*FortranFunction1Ptr)(const double *);

      //! a simple alias
      typedef double (*FortranFunction2Ptr)(const double *,const double *);

      //! a simple alias
      typedef double (*FortranFunction3Ptr)(const double *,const double *,
					    const double *);

      //! a simple alias
      typedef double (*FortranFunction4Ptr)(const double *,const double *,
					    const double *,const double *);

      //! a simple alias
      typedef double (*FortranFunction5Ptr)(const double *,const double *,
					    const double *,const double *,
					    const double *);

      //! a simple alias
      typedef double (*FortranFunction6Ptr)(const double *,const double *,
					    const double *,const double *,
					    const double *,const double *);

      //! a simple alias
      typedef double (*FortranFunction7Ptr)(const double *,const double *,
					    const double *,const double *,
					    const double *,const double *,
					    const double *);

      //! a simple alias
      typedef double (*FortranFunction8Ptr)(const double *,const double *,
					    const double *,const double *,
					    const double *,const double *,
					    const double *,const double *);

      //! a simple alias
      typedef double (*FortranFunction9Ptr)(const double *,const double *,
					    const double *,const double *,
					    const double *,const double *,
					    const double *,const double *,
					    const double *);

      //! a simple alias
      typedef double (*FortranFunction10Ptr)(const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *);

      //! a simple alias
      typedef double (*FortranFunction11Ptr)(const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *);

      //! a simple alias
      typedef double (*FortranFunction12Ptr)(const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *);

      //! a simple alias
      typedef double (*FortranFunction13Ptr)(const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *);

      //! a simple alias
      typedef double (*FortranFunction14Ptr)(const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *);

      //! a simple alias
      typedef double (*FortranFunction15Ptr)(const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *,const double *,
					     const double *);

      /*!
       * a simple alias.
       * This is the prototype of the external functions used by the
       * pleiades version of the castem finite element solver
       */
      typedef void (*UMATFctPtr)(const UMATIntegerType  *const, /* nombre de composantes des contraintes  */
				 const UMATRealType *const, /* incrément de temps                     */
				 const UMATRealType *const, /* matrice de passage du repère élement
							       au repère global                       */
				 UMATRealType *const,       /* matrice tangente                       */
				 const UMATRealType *const, /* tenseur des déformations               */
				 const UMATRealType *const, /* tenseur des incréments de déformations */
				 const UMATRealType *const, /* température au début du pas            */
				 const UMATRealType *const, /* incrément de température               */
				 const UMATRealType *const, /* propriétés matériau                    */
				 const UMATIntegerType  *const, /* nombre de propriétés matériau          */
				 const UMATRealType *const, /* variables externes */
				 const UMATRealType *const, /* incréments des variables externes */
				 UMATRealType *const,       /* variables internes   */
				 const UMATIntegerType  *const, /* nombre de variables internes */
				 UMATRealType *const,       /* tenseur des contraintes */
				 const UMATIntegerType  *const, /* entier définissant le type de calcul  */
				 UMATIntegerType  *const);      /* code sortie */

      /*!
       * a simple alias.
       * This is the prototype of the external functions used by the
       * pleiades version of the castem finite element solver
       */
      typedef void (*AsterFctPtr)(AsterRealType *const,       /* stress                   */
				  AsterRealType *const,       /* internal state variables */
				  AsterRealType *const,       /* tangent operator         */
				  AsterRealType *const,
				  AsterRealType *const,
				  AsterRealType *const,
				  AsterRealType *const,
				  AsterRealType *const,
				  AsterRealType *const,
				  AsterRealType *const,
				  const AsterRealType *const, /* strain tensor    */
				  const AsterRealType *const, /* strain increment */
				  const AsterRealType *const,
				  const AsterRealType *const, /* time increment   */
				  const AsterRealType *const, /* temperature      */
				  const AsterRealType *const, /* temperature increment    */
				  const AsterRealType *const, /* external state variables */
				  const AsterRealType *const, /* external state variables increments   */
				  const char     *const,
				  const AsterIntegerType  *const,
				  const AsterIntegerType  *const,
				  const AsterIntegerType  *const, /* number of components of tensors       */
				  const AsterIntegerType  *const, /* number of internal state variables    */
				  const AsterRealType *const,     /* material properties                   */
				  const AsterIntegerType  *const, /* number of material properties         */
				  const AsterRealType *const,
				  const AsterRealType *const, /* rotation matrix                       */
				  AsterRealType *const,       /* estimation of the next time increment */
				  const AsterRealType *const,
				  const AsterRealType *const,
				  const AsterRealType *const,
				  const AsterIntegerType  *const,
				  const AsterIntegerType  *const,
				  const AsterIntegerType  *const,
				  const AsterIntegerType  *const,
				  const AsterIntegerType  *const,
				  AsterIntegerType  *const);

    }
    
  } // end of namespace system
    
} // end of namespace tfel

#endif /* __cplusplus */

#endif /* _LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_H */

