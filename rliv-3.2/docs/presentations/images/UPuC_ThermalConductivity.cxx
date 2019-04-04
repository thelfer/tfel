#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

unsigned short
UPuC_ThermalConductivity_nargs = 3u;

double
UPuC_ThermalConductivity(const double * const castem_params)
{
  using namespace std;
  const double T = *(castem_params);
  const double p = *(castem_params+1u);
  const double Bu = *(castem_params+2u);
  double k;
  if( T <= 773.15){
    k = ( 8.14e-6 * T - 0.010096882) * T + 19.65063040915;
  } else{
    k = ( -1.88e-6 * T + 0.009737044) * T + 10.2405949657;
  }
  k *= ( 1. - p) /( 1. + 2. * p);
  k *=  1. -( 0.02 * Bu);
  return k;
} // end of UPuC_ThermalConductivity

#ifdef __cplusplus
} // end of extern "C"
#endif /* __cplusplus */
