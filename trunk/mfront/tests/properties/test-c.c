/*! 
 * \file   test-c
 * \author Helfer Thomas
 * \date   16 jan 2010
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#include"VanadiumAlloy_YoungModulus_SRMA.hxx"
#include"VanadiumAlloy_PoissonRatio_SRMA.hxx"
#include"VanadiumAlloy_ThermalExpansion_SRMA.hxx"
#include"VanadiumAlloy_ThermalConductivity_SRMA.hxx"
#include"VanadiumAlloy_SpecificHeat_SRMA.hxx"

int main(void){
  
  const double T = 900.;
  const double y = 127.8e9 * (1.-7.825e-5*(T-293.15));
  const double n = 0.3272  * (1.-3.056e-5*(T-293.15));
  assert(fabs(VanadiumAlloy_YoungModulus_SRMA(900.)-y)<1.e-14*y);
  assert(fabs(VanadiumAlloy_PoissonRatio_SRMA(900.)-n)<1.e-14*n);

  return EXIT_SUCCESS;
}
