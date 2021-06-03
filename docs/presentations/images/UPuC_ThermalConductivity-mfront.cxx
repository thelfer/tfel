@Parser MaterialLaw;
@Law ThermalConductivity;
@Material UPuC;
@Author Thomas Helfer;

@Output k;  //< changing the name of output

@Input T, p, Bu;                   //< inputs of the law
T.setGlossaryName("Temperature");  //< pleiades name
p.setGlossaryName("Porosity");     //< pleiades name
Bu.setGlossaryName("BurnUp");      //< pleiades name

@PhysicalBounds T in [0:*[;  //< temperature physical bounds
@Bounds T in [0:2573.15];    //< temperature bounds
@PhysicalBounds p in [0:1];  //< porosity physical bounds
@PhysicalBounds Bu in [0:*[; //< burn-up  physicalbounds

@Function{
  if (T <= 773.15) {
    k = (8.14e-6 * T - 0.010096882) * T + 19.65063040915;
  } else {
    k = (-1.88e-6 * T + 0.009737044) * T + 10.2405949657;
  }
  k *= (1. - p) / (1. + 2. * p);
  k *= 1. - (0.02 * Bu);
} // end of function
