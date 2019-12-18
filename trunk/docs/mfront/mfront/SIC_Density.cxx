@Parser MaterialLaw;
@Law    SIC_DENSITY;
@Author Éric Brunon;
@Date   2007-12-06;

@Description{
  Origine inconnue
}

// changing the name of output
@Output r;

// input of the law
@Input p;
p.setGlossaryName("Porosity");

// porosity bounds
@PhysicalBounds p in [0:1];

@Function{
  static constexpr double a =  3215. ;
  r=a*(1.-p);
} // end of function
