% Material properties in MFront
% Helfer Thomas
% August 15, 2014

Material properties generally depend on some state variables which
describes the current thermodynamical state of the material.

# A first example: uranium dioxide Young modulus

A possible implementation of the Young modulus of uranium dioxide is:

~~~~ {#UO2YoungModulus .cpp .numberLines}
@Parser MaterialLaw;          // treating a material property
@Material UO2;                // material name
@Law YoungModulus_Martin1989; // name of the material property 
@Author   T. Helfer;  // author name
@Date     04/04/2014; // implentation date
@Description          // detailled description
{
  The elastic constants of polycrystalline UO2 and
  (U, Pu) mixed oxides: a review and recommendations
  Martin, DG
  High Temperatures. High Pressures, 1989
}

@Output E;                    // output of the material property
E.setGlossaryName("YoungModulus");
@Input T,f;                   // inputs of the material property
T.setGlossaryName("Temperature");
f.setGlossaryName("Porosity");

@PhysicalBounds T in [0:*[;    // Temperature is positive
@PhysicalBounds f in [0:1.];   // Porosity is positive and lower than one
@Bounds T in [273.15:2610.15]; // Validity range

@Function                     // implementation body
{
  E = 2.2693e11*(1.-2.5*f)*(1-6.786e-05*T-4.23e-08*T*T);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The code computing the Young modulus, introduced by the `@Function`
keyword on line 24, is reasonably closed to the mathematical
expression of the material property:
\[
E(T,f)=2.2693\,10^{11}\,(1-2.5\,f)\,(1-6.786\,10^{-5}\,T-4.23\,10^{-8}\,T^{2})
\]

<!-- Local IspellDict: english -->
