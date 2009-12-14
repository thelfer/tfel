/*!
 * \file   getFunction.c
 * \brief  ths file implements some C wrappers around the dlsym
 * system call.
 * \author Helfer Thomas
 * \date   18 december 2008
 */

#include<stdlib.h>
#include<string.h>
#include<dlfcn.h>

#include"System/getFunction.h"

int tfel_getCastemFunctionNumberOfVariables(void *lib,
					    const char * const name){
  unsigned short * n;
  size_t len = strlen(name);
  char *myname = (char *) malloc((len+7u)*sizeof(char));
  if(myname==0){
    return -1;
  }
  strncpy(myname,name,len);
  myname[len]='_';
  myname[len+1u]='n';
  myname[len+2u]='a';
  myname[len+3u]='r';
  myname[len+4u]='g';
  myname[len+5u]='s';
  myname[len+6u]='\0';
  n= (unsigned short *) dlsym(lib,myname);
  free(myname);
  if(n==0){
    return -1;
  }  
  return *n;
} 

int tfel_getUnsignedShort(void *lib,
			  const char * const name){
  unsigned short * n;
  n= (unsigned short *) dlsym(lib,name);
  if(n==0){
    return -1;
  }  
  return *n;
} 

char **
tfel_getArrayOfStrings(void *lib,
		       const char * const name)
{
  char ** v;
  v = (char **) dlsym(lib,name);
  return v;
}

int *
tfel_getArrayOfInt(void *lib,
		   const char * const name)
{
  int * v;
  v = (int *) dlsym(lib,name);
  return v;
}

char **
tfel_getCastemFunctionVariables(void *lib,
				const char * const name)
{
  char ** v;
  size_t len = strlen(name);
  char *myname = (char *) malloc((len+6u)*sizeof(char));
  if(myname==0){
    return 0;
  }
  strncpy(myname,name,len);
  myname[len]='_';
  myname[len+1u]='a';
  myname[len+2u]='r';
  myname[len+3u]='g';
  myname[len+4u]='s';
  myname[len+5u]='\0';
  v = (char **) dlsym(lib,myname);
  free(myname);
  return v;
}

double (*tfel_getCastemFunction(void *lib,const char * const name))(const double*const){
  return (double (*)(const double*const)) dlsym(lib,name);
} 

double (*tfel_getCFunction0(void *lib,const char * const name))(void){
  return (double (*)(void)) dlsym(lib,name);
} 


double (*tfel_getCFunction1(void *lib,const char * const name))(double){
  return (double (*)(double)) dlsym(lib,name);
}


double (*tfel_getCFunction2(void *lib,const char * const name))(double,double){
  return (double (*)(double,double)) dlsym(lib,name);
}


double (*tfel_getCFunction3(void *lib,const char * const name))(double,double,
								double){
  return (double (*)(double,double,double)) dlsym(lib,name);
}


double (*tfel_getCFunction4(void *lib,const char * const name))(double,double,
								double,double){
  return (double (*)(double,double,double,double)) dlsym(lib,name);
}


double (*tfel_getCFunction5(void *lib,const char * const name))(double,double,
								double,double,
								double){
  return (double (*)(double,double,double,double,
		     double)) dlsym(lib,name);
}

double (*tfel_getCFunction6(void *lib,const char * const name))(double,double,
								double,double,
								double,double){
  return (double (*)(double,double,double,double,
		     double,double)) dlsym(lib,name);
}


double (*tfel_getCFunction7(void *lib,const char * const name))(double,double,
								double,double,
								double,double,
								double){
  return (double (*)(double,double,double,double,
		     double,double,double)) dlsym(lib,name);
}


double (*tfel_getCFunction8(void *lib,const char * const name))(double,double,
								double,double,
								double,double,
								double,double){
  return (double (*)(double,double,double,double,
		     double,double,double,double)) dlsym(lib,name);
}


double (*tfel_getCFunction9(void *lib,const char * const name))(double,double,
								double,double,
								double,double,
								double,double,
								double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double)) dlsym(lib,name);
}


double (*tfel_getCFunction10(void *lib,const char * const name))(double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double,double)) dlsym(lib,name);
}


double (*tfel_getCFunction11(void *lib,const char * const name))(double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double,
								 double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double,double,double)) dlsym(lib,name);
}


double (*tfel_getCFunction12(void *lib,const char * const name))(double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double,double,double,double)) dlsym(lib,name);
}


double (*tfel_getCFunction13(void *lib,const char * const name))(double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double,
								 double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double,double,double,double,
		     double)) dlsym(lib,name);
}


double (*tfel_getCFunction14(void *lib,const char * const name))(double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double,double,double,double,
		     double,double)) dlsym(lib,name);
}


double (*tfel_getCFunction15(void *lib,const char * const name))(double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double,
								 double,double,
								 double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double,double,double,double,
		     double,double,double)) dlsym(lib,name);
}

double (*tfel_getFortranFunction0(void *lib,const char * const name))(void){
  return (double (*)(void)) dlsym(lib,name);
} 


double (*tfel_getFortranFunction1(void *lib,const char * const name))(const double* const){
  return (double (*)(const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction2(void *lib,const char * const name))(const double* const,const double* const){
  return (double (*)(const double* const,const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction3(void *lib,const char * const name))(const double* const,const double* const,
								      const double* const){
  return (double (*)(const double* const,const double* const,const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction4(void *lib,const char * const name))(const double* const,const double* const,
								      const double* const,const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction5(void *lib,const char * const name))(const double* const,const double* const,
								      const double* const,const double* const,
								      const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const)) dlsym(lib,name);
}

double (*tfel_getFortranFunction6(void *lib,const char * const name))(const double* const,const double* const,
								      const double* const,const double* const,
								      const double* const,const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction7(void *lib,const char * const name))(const double* const,const double* const,
								      const double* const,const double* const,
								      const double* const,const double* const,
								      const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction8(void *lib,const char * const name))(const double* const,const double* const,
								      const double* const,const double* const,
								      const double* const,const double* const,
								      const double* const,const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction9(void *lib,const char * const name))(const double* const,const double* const,
								      const double* const,const double* const,
								      const double* const,const double* const,
								      const double* const,const double* const,
								      const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const,
		     const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction10(void *lib,const char * const name))(const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction11(void *lib,const char * const name))(const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction12(void *lib,const char * const name))(const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction13(void *lib,const char * const name))(const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const,
		     const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction14(void *lib,const char * const name))(const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const)) dlsym(lib,name);
}


double (*tfel_getFortranFunction15(void *lib,const char * const name))(const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const,const double* const,
								       const double* const){
  return (double (*)(const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const,const double* const,
		     const double* const,const double* const,const double* const)) dlsym(lib,name);
}

