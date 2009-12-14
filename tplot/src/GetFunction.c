/*!
 * \file   GetFunction.c
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   18 déc 2008
 */

#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Graphics/GetFunction.h"

int checkCastemFunction(void *lib,
			const char * const name,
			const unsigned short nbr){
  size_t len = strlen(name);
  char *myname = (char *) malloc((len+7u)*sizeof(char));
  if(myname==0){
    return -2;
  }
  strncpy(myname,name,len);
  myname[len]='_';
  myname[len+1u]='n';
  myname[len+2u]='a';
  myname[len+3u]='r';
  myname[len+4u]='g';
  myname[len+5u]='s';
  myname[len+6u]='\0';
  unsigned short * n= (unsigned short *) dlsym(lib,myname);
  free(myname);
  if(nbr!=*n){
    return -1;
  }
  return 0;
} 

double (*getCastemFunction(void *lib,const char * const name))(const double*const){
  return (double (*)(const double*const)) dlsym(lib,name);
} 

double (*getCFunction0(void *lib,const char * const name))(void){
  return (double (*)(void)) dlsym(lib,name);
} 


double (*getCFunction1(void *lib,const char * const name))(double){
  return (double (*)(double)) dlsym(lib,name);
}


double (*getCFunction2(void *lib,const char * const name))(double,double){
  return (double (*)(double,double)) dlsym(lib,name);
}


double (*getCFunction3(void *lib,const char * const name))(double,double,
							   double){
  return (double (*)(double,double,double)) dlsym(lib,name);
}


double (*getCFunction4(void *lib,const char * const name))(double,double,
							   double,double){
  return (double (*)(double,double,double,double)) dlsym(lib,name);
}


double (*getCFunction5(void *lib,const char * const name))(double,double,
							   double,double,
							   double){
  return (double (*)(double,double,double,double,
		     double)) dlsym(lib,name);
}


double (*getCFunction6(void *lib,const char * const name))(double,double,
							   double,double,
							   double,double){
  return (double (*)(double,double,double,double,
		     double,double)) dlsym(lib,name);
}


double (*getCFunction7(void *lib,const char * const name))(double,double,
							   double,double,
							   double,double,
							   double){
  return (double (*)(double,double,double,double,
		     double,double,double)) dlsym(lib,name);
}


double (*getCFunction8(void *lib,const char * const name))(double,double,
							   double,double,
							   double,double,
							   double,double){
  return (double (*)(double,double,double,double,
		     double,double,double,double)) dlsym(lib,name);
}


double (*getCFunction9(void *lib,const char * const name))(double,double,
							   double,double,
							   double,double,
							   double,double,
							   double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double)) dlsym(lib,name);
}


double (*getCFunction10(void *lib,const char * const name))(double,double,
							    double,double,
							    double,double,
							    double,double,
							    double,double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double,double)) dlsym(lib,name);
}


double (*getCFunction11(void *lib,const char * const name))(double,double,
							    double,double,
							    double,double,
							    double,double,
							    double,double,
							    double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double,double,double)) dlsym(lib,name);
}


double (*getCFunction12(void *lib,const char * const name))(double,double,
							    double,double,
							    double,double,
							    double,double,
							    double,double,
							    double,double){
  return (double (*)(double,double,double,double,
		     double,double,double,double,
		     double,double,double,double)) dlsym(lib,name);
}


double (*getCFunction13(void *lib,const char * const name))(double,double,
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


double (*getCFunction14(void *lib,const char * const name))(double,double,
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


double (*getCFunction15(void *lib,const char * const name))(double,double,
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

