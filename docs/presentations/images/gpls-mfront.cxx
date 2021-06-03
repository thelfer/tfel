@Parser RungeKutta;
@Behaviour GPLS;
@Author Helfer Thomas;
@Algorithm rk54;
@Date 27 / 03 / 09;

@Includes {
#include <TFEL/Material/Lame.hxx>
#include <TFEL/Material/Hill.hxx>
}

@Epsilon 1.e-6;

@OrthotropicBehaviour;
@RequireStiffnessTensor;

@StateVar real p;      /* Equivalent viscoplastic strain */
@StateVar Stensor evp; /* Viscoplastic strain            */
