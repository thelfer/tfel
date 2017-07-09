/*!
 * \file   log1D.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   18 déc. 2015
 */

#include<iostream>
#include<cstdlib>

int main(){
  for(unsigned short i=0;i!=3;++i){
    for(unsigned short j=0;j!=3;++j){
      if((i==0)&&(j==0)){
	std::cout << "*DDSDDE     = ("; 
      } else {
	std::cout << "*(DDSDDE+"<< i+3*j << ") = ("; 
      }
      if(i==j){
	std::cout << "-STRESS[" << i<< "]+";
	std::cout << "K[" << i+3*j << "]/(STRAN[" << j << "]+DSTRAN[" << j << "]))";
	std::cout << "/(STRAN[" << i << "]+DSTRAN[" << i << "]);\n";
      } else {
	std::cout << "K[" << i+3*j << "]/((STRAN[" << j << "]+DSTRAN[" << j << "])*"
	  "(STRAN[" << i << "]+DSTRAN[" << i << "]));\n";
      }
    }
  }
  return EXIT_SUCCESS;
}
