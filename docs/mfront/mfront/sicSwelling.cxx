@Parser PleiadesModel;
@Model SiCSwellingModelTest;
@Author Helfer Thomas;
@Date 05 Déc.2007;

@LocalParameter TabString domains;
domains.setDefaultValue("MATRIX", "CLADDING");

@BindDomainToParameter "domains";

@StaticVar double Phi0 = 0.3396e25;

@Output s;
s.setGlossaryName("SolidSwelling");
s.setDepth(1);

@Input f;
f.setGlossaryName("Fluence");
f.setDepth(1);

@Input T;
T.setGlossaryName("Temperature");
T.setDepth(1);

@LoadingVariable n;
n.setEntryName("NeutronFlux");
n.setDepth(1);

@Function compute {
  const double theta = 0.5 * (T_1 + T) - 273.15;
  const double Phi = 0.5 * (f_1 + f);
  double swellingSaturation;
  double incr;
  // the original law is given in %,
  // so values of swellingSaturation are here divided by 100.
  if ((25 <= theta) && (theta <= 800.)) {
    swellingSaturation = -3.3283e-5 * theta + 3.1133e-2;
  } else if ((800. < theta) && (theta <= 1000.)) {
    swellingSaturation = -1.3528e-5 * theta + 1.5329e-2;
  } else if ((1000. < theta) && (theta <= 1500.)) {
    swellingSaturation = 0.18e-2;
  } else {
    ostringstream msg;
    msg << "SICSolidSwellingModel::computeSolidSwellingIncrement : ";
    msg << "temperature is out of bounds (" << theta << ")";
    throw(Pleiades::PExceptions::PleiadesError(msg.str()));
  }
  incr = swellingSaturation / Phi0;
  incr *= exp(-Phi / Phi0);
  // terme supplémentaire à plus haute température
  if ((1000. < theta) && (theta <= 1250.)) {
    incr += 1.297e-28 * ((theta - 1000.) / 250.);
  }
  if (theta >= 1250.) {
    incr += 1.297e-28;
  }
  incr *= 0.5 * (n + n_1) * dt;
  s = s_1 + incr;
}  // end of function compute
