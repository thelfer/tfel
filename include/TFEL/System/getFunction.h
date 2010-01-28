/*!
 * \file   getFunction.h
 * \brief  this file declares some C wrappers around the dlsym
 * system call.
 * \author Helfer Thomas
 * \date   13 déc 2008
 */

#ifndef _LIB_TFEL_GETFUNCTION_H_
#define _LIB_TFEL_GETFUNCTION_H_

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

  /*!
   * \brief this function get if the number of argument of the
   * specified function
   *
   * This function search a symbol called name+'_nargs' and interpret
   * it as and unsigned short. The value of that unsigned short
   * compared to the number given in argument.
   *
   * The name 'castem' comes from an extension we wrote for that
   * finite element code that uses function with the following
   * prototype double (*)(const double* const);
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return int, the number of variables (-1 if on error).
   */
  int tfel_getCastemFunctionNumberOfVariables(void *,
					      const char * const);

  int tfel_getUnsignedShort(void *,
			    const char * const);


  char **
  tfel_getArrayOfStrings(void *,
			 const char * const);

  int *
  tfel_getArrayOfInt(void *,
		     const char * const);

  char **
  tfel_getCastemFunctionVariables(void *,
				  const char * const);

  
  /*!
   * \brief this function returns a function of type
   * double (*)(const double * const)
   *
   * The name 'castem' comes from an extension we wrote for that
   * finite element code that uses function with the following
   * prototype double (*)(const double* const);
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCastemFunction(void *,const char * const))(const double*const);

  /*!
   * \brief this function returns a function of type
   * double (*)(void)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction0(void*,const char * const))(void);

  /*!
   * \brief this function returns a function of type
   * double (*)(double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction1(void*,const char * const))(double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction2(void*,const char * const))(double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction3(void*,const char * const))(double,double,
							 double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction4(void*,const char * const))(double,double,
							 double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction5(void*,const char * const))(double,double,
							 double,double,
							 double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction6(void*,const char * const))(double,double,
							 double,double,
							 double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction7(void*,const char * const))(double,double,
							 double,double,
							 double,double,
							 double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction8(void*,const char * const))(double,double,
							 double,double,
							 double,double,
							 double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction9(void*,const char * const))(double,double,
							 double,double,
							 double,double,
							 double,double,
							 double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double,double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction10(void*,const char * const))(double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,
   *          double,double,double,double,
   *          double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction11(void*,const char * const))(double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double,
							  double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,
   *          double,double,double,double,
   *          double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction12(void*,const char * const))(double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,
   *          double,double,double,double,
   *          double,double,double,double,
   *          double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction13(void*,const char * const))(double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double,
							  double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,
   *          double,double,double,double,
   *          double,double,double,double,
   *          double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction14(void*,const char * const))(double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,
   *          double,double,double,double,
   *          double,double,double,double,
   *          double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getCFunction15(void*,const char * const))(double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double,
							  double,double,
							  double);

  /*!
   * \brief this function returns a function of type
   * double (*)(void)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction0(void*,const char * const))(void);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction1(void*,const char * const))(const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction2(void*,const char * const))(const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction3(void*,const char * const))(const double* const,const double* const,
							       const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction4(void*,const char * const))(const double* const,const double* const,
							       const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction5(void*,const char * const))(const double* const,const double* const,
							       const double* const,const double* const,
							       const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction6(void*,const char * const))(const double* const,const double* const,
							       const double* const,const double* const,
							       const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction7(void*,const char * const))(const double* const,const double* const,
							       const double* const,const double* const,
							       const double* const,const double* const,
							       const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction8(void*,const char * const))(const double* const,const double* const,
							       const double* const,const double* const,
							       const double* const,const double* const,
							       const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction9(void*,const char * const))(const double* const,const double* const,
							       const double* const,const double* const,
							       const double* const,const double* const,
							       const double* const,const double* const,
							       const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction10(void*,const char * const))(const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction11(void*,const char * const))(const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction12(void*,const char * const))(const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction13(void*,const char * const))(const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction14(void*,const char * const))(const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param void *, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (*tfel_getFortranFunction15(void*,const char * const))(const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const,const double* const,
								const double* const);
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif 	/* _LIB_TFEL_GETFUNCTION_H_ */
