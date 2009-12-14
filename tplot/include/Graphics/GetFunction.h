/*
 * \file   GetFunction.h
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 déc 2008
 */

#ifndef _LIB_TFEL_GRAPHICS_GETFUNCTION_H_
#define _LIB_TFEL_GRAPHICS_GETFUNCTION_H_

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

  int checkCastemFunction(void *,
			  const char * const,
			  const unsigned short);

  double (*getCastemFunction(void *,const char * const))(const double*const);

  double (*getCFunction0(void*,const char * const))(void);

  double (*getCFunction1(void*,const char * const))(double);

  double (*getCFunction2(void*,const char * const))(double,double);

  double (*getCFunction3(void*,const char * const))(double,double,
						    double);

  double (*getCFunction4(void*,const char * const))(double,double,
						    double,double);

  double (*getCFunction5(void*,const char * const))(double,double,
						    double,double,
						    double);

  double (*getCFunction6(void*,const char * const))(double,double,
						    double,double,
						    double,double);

  double (*getCFunction7(void*,const char * const))(double,double,
						    double,double,
						    double,double,
						    double);

  double (*getCFunction8(void*,const char * const))(double,double,
						    double,double,
						    double,double,
						    double,double);

  double (*getCFunction9(void*,const char * const))(double,double,
						    double,double,
						    double,double,
						    double,double,
						    double);

  double (*getCFunction10(void*,const char * const))(double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double);

  double (*getCFunction11(void*,const char * const))(double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double,
						     double);

  double (*getCFunction12(void*,const char * const))(double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double);

  double (*getCFunction13(void*,const char * const))(double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double,
						     double);

  double (*getCFunction14(void*,const char * const))(double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double);

  double (*getCFunction15(void*,const char * const))(double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double,
						     double,double,
						     double);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif 	/* _LIB_TFEL_GRAPHICS_GETFUNCTION_H_ */
