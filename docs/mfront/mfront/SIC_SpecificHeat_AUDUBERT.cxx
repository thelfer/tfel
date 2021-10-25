@Parser MaterialLaw;
@Law SIC_SPECIFICHEAT_AUDUBERT;
@Author Thomas Helfer;
@Date 2007 - 12 - 06;

@Description {
  Caractérisation des matériaux inertes pour FUTURIX / MI DEC / SPUA / LTEC /
          NT 05 -
      044 ind 0 de Aout 2005 F.AUDUBERT
 §4.2 page 19 - Figure 13 c
}

// changing the name of output
@Output C;

// input of the law
@Input T;
T.setGlossaryName("Temperature");

// temperature bounds
@PhysicalBounds T in [0:*[;

@Function{
  static constexpr double a = 48.54704;
  static constexpr double b = 0.00292;
  static constexpr double c = -21.09629;
  static constexpr double d = -0.0034;
  static constexpr double e = 4.0E-2;
  C = (a + b * T + c * exp(d * T)) / e;
} // end of function
