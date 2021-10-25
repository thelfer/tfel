@Parser MaterialLaw;
@Law SIC_YOUNGMODULUS_SNEAD;
@Author Thomas Helfer;
@Date 2007 - 12 - 06;

@Description {
  Journal of Nuclear Materials 371 ( 2007 ) 329-377
  Handbook of SiC properties for fuel performance modeling
  L.L. SNEAD et al.
  Pages 339 et 340 - équations (17) et (18)
}

// changing the name of output
@Output E;

// input of the law
@Input T, p;

// variables bounds
@PhysicalBounds T in [0:*[;
@PhysicalBounds p in [0:1];

@Function{
  const real E0 = 460.00E9;
  const real B = 0.04E9;
  const real T0 = 962.00;
  const real C = 3.57;
  E = (E0 - (B * T * exp(-T0 / T))) * exp(-C * p);
} // end of function
