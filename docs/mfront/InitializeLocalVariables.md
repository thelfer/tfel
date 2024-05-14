The `@InitializeLocalVariables` introduces a code block meant to
initialize the local variables (see the `@LocalVariable` keyword).

## Example

~~~~{.cpp}
//! volumic fraction of each phase
@LocalVariable real  fv[Np];
//! interaction matrix
@LocalVariable tfel::math::tmatrix<Nss,Nss,real> mh;

@InitLocalVariables<Append>{
  // volumic fraction of each phase
  for(unsigned short i=0;i!=Np;++i){
    fv[i]=1.0/static_cast<real>(Np) ; 
  }
  // interaction matrix
  const real h1 = 1.;
  const real h2 = 1.;
  const real h3 = 1.;
  const real h4 = 1.;
  const real h5 = 1.;
  const real h6 = 1.;
  mh = InteractionMatrix::getInteractionMatrix(h1,h2,h3,
					       h4,h5,h6);
}
~~~~


