@Parser PleiadesModel;
@Model  NbZrCSwellingModelTest;
@Author Helfer Thomas;
@Date   23 Nov 2007;

@LocalParameter TabString domains;
domains.setDefaultValue("MATRIX","RING");

@BindDomainToParameter "domains";

@Output s;
s.setGlossaryName("SolidSwelling");
s.setDepth(1);

@LoadingVariable dpaEv;
dpaEv.setEntryName("DisplacementPerAtome");
dpaEv.setDepth(1);

@Input T;
T.setGlossaryName("Temperature");
T.setDepth(1);

@Function compute
{
  const double ddpa = dpaEv-dpaEv_1;
  const double dpa  = 0.5*(dpaEv_1+dpaEv);
  const double tmp  = max(400.,370.56*dpa);
  const double theta = 0.5*(T_1+T)-273.15;
  s = s_1 + 1.824e-3*exp(-(theta-800.)*(theta-800.)/tmp)*ddpa;
} // end of function compute
