@Parser MaterialLaw;
@Law    SIC_THERMALEXPANSION_SNEAD;
@Author Éric Brunon;
@Date   2007-12-06;

@Description{
 Journal of Nuclear Materials 371 ( 2007 ) 329-377
 Handbook of SiC properties for fuel performance modeling
 L.L. SNEAD et al.
 Pages 338 et 339 - Équation (16)

 Attention : L équation (16) donne le coefficient de
             dilatation instantanée. La loi donnée ici
             donne la dilatation moyenne par rapport à une
             température de référence de 20°C
}

// changing the name of output
@Output a;

// input of the law
@Input T;
T.setGlossaryName("Temperature");

// temperature bounds
@PhysicalBounds T in [0:*[;

@Function{

  // coefficients constants
  static constexpr double T0  = 293.15;
  static constexpr double T02 = 85936.9225;
  static constexpr double T03 = 25192408.830875;
  static constexpr double T04 = 7385154648.77100;
  static constexpr double T1  = 1273.;
  static constexpr double T12 = 1620529.;
  static constexpr double T13 = 2062933417.;
  static constexpr double T14 = 2626114239841.;
  static constexpr double a0 =  -1.8276;
  static constexpr double a1 =   0.0178;
  static constexpr double a2 =  -1.5544e-5;
  static constexpr double a3 =   4.5246e-9;
  static constexpr double a4 =   5.0e-6;
  static constexpr double M1_3 = 1./3.;

  const double T2 = T*T;
  const double T3 = T2*T;
  const double T4 = T3*T;

  double tmp;  
  if (T<T1){
    tmp  = a0*(T-T0)+0.5*a1*(T2-T02);
    tmp += M1_3*a2*(T3-T03)+0.25*a3*(T4-T04);
    tmp *= 1e-6;
    a=(exp(tmp)-1)/(T-T0);
  } else {
    tmp  = a0*(T1-T0)+0.5*a1*(T12-T02);
    tmp += M1_3*a2*(T13-T03)+0.25*a3*(T14-T04);
    tmp *= 1e-6;
    tmp += a4*(T-T1);
    a= (exp(tmp)-1.)/(T-T0);
  }
} // end of function
