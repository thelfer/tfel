/*! 
 * \file  ExternalFunctionsPrototypes.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 26 jan 2010
 */

#ifndef _LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_H_
#define _LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_H_ 

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

    } // end of extern "C"
    
  } // end of namespace system

} // end of namespace tfel

#endif /* _LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_H */

