/*!
 * \file   TensorConvertion.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   14 oct. 2015
 */

#include<iostream>

static int index(const unsigned short i,
		 const unsigned short j){
  if((i==j)&&(i<3)){
    return i;
  } else if((i==0)&&(j==1)){
    return 3;
  } else if((i==1)&&(j==0)){
    return 4;
  }
  return -1;
}

int main(){
  // CIJKN=1/J(K_{IJKL}*F_{NL}
  // CIJKM=1/J(K_{IJKL}*F_{ML} (N=>M)
  // CIJKM=1/J(K_{IJKN}*F_{MN} (L=>N)
  // CIJKL=1/J(K_{IJKN}*F_{LN} (M=>L)
  for(unsigned short i=0;i!=3;++i){
    for(unsigned short j=0;j!=3;++j){
      const auto ri = index(i,j);
      if(ri!=-1){
	for(unsigned short k=0;k!=3;++k){
	  for(unsigned short l=0;l!=3;++l){
	    const auto rj = index(k,l);
	    if(rj!=-1){
	      std::cout << "C(" << ri << "," << rj << ") = 1/J*(";
	      bool first = true;
	      for(unsigned short n=0;n!=3;++n){
		const auto rkn = index(k,n);
		const auto rln = index(l,n);
		if(!((rkn==-1)||(rln==-1))){
		  if(!first){
		    std::cout << "+";
		  }
		  std::cout << "K(" << ri << "," << rkn << ")*F(" << rln << ")";
		  first=false;
		}
	      }
	      std::cout << ");\n";
	    }
	  }
	}
      }
    }
  }
  return EXIT_SUCCESS;
}










