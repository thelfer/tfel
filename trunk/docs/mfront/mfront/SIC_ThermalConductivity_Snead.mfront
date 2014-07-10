@Parser MaterialLaw;
@Law    SIC_THERMALCONDUCTIVITY_SNEAD;
@Author Éric Brunon;
@Date   2007-12-06;

@Description{
 Loi de la publication JNM ajustée sur les mesures du SPUA 

 Publication JNM :                                      

 Journal of Nuclear Materials 371 ( 2007 ) 329-377
 Handbook of SiC properties for fuel performance modeling
 L.L. SNEAD et al.
 Page 337 - Equations (12) - Figure 7

 Mesures SPUA :

 Caractérisation des matériaux inertes pour FUTURIX/MI
 DEC/SPUA/LTEC/NT 05-044 ind 0 de Aout 2005
 F. AUDUBERT
 §4.2 page 22 - Figure 18
}

// changing the name of output
@Output k;

// input of the law
@Input T;
T.setGlossaryName("Temperature");

// temperature bounds
@PhysicalBounds T in [0:*[;

@Function{
  static const double a =  0.781151102452916 ;
  static const double b =  1.05E-5 ;
  static const double c = -3.00E-4 ;
  k = a/(b*T+c);
} // end of function
