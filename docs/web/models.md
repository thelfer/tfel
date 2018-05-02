% Models
% Helfer Thomas
% October 6, 2014

\newcommand{\paren}[1]{\left(#1\right)}
\newcommand{\deriv}[2]{\frac{\partial #1}{\partial #2}}
\newcommand{\bts}[1]{\left.#1\right|_{t}}
\newcommand{\mts}[1]{\left.#1\right|_{t+\theta\,\Delta\,t}}
\newcommand{\ets}[1]{\left.#1\right|_{t+\Delta\,t}}

Models describe the evolution of some state variables of the material
during a time step.

To illustrate how a model can be implemented using `MFront`, let us
consider a simple solid swelling model which describes the evolution
of the swelling \(s\) as a function of the porosity \(f\) and the
burn-up \(\tau\):

\[
\deriv{s}{\tau}=C_{1}\,\exp\paren{C_{2}-f}
\]

A simple semi-implicit scheme can be used to integrate this equation
upon a time step:

\[
\Delta\,s=C_{1}\,\exp\paren{C_{2}-\mts{f}}\,\Delta\,\tau
\]

which can be  also written as:

\[
\ets{s}=\bts{s}+C_{1}\,\exp\paren{C_{2}-\mts{f}}\,\paren{\ets{\tau}-\bts{\tau}}
\]

This equation is the basis of the `MFront` implementation of the
model:

~~~~ {#UPuCSolidSwellingModel .cpp .numberLines}
@Parser   Model;
@Model    SolidSwellingModel;
@Material UPuC;
@Author   Helfer Thomas;
@Date     06 december 2007;

@Output s;
s.setGlossaryName("SolidSwelling");
s.setDefaultInitialValue(0.);
s.setDepth(1);

@Input Bu;
Bu.setGlossaryName("BurnUp");
Bu.setDepth(1);

@Input f;
f.setGlossaryName("Porosity");
f.setDepth(1);

@Function compute
{
  const real coef1 = 8.e-3;
  const real coef2 = 4.e-2;
  const real f_    = 0.5*(f+f_1);
  s = s_1 + coef1*exp(coef2-f_)*(Bu-Bu_1);
} // end of function compute
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<!-- Local IspellDict: english -->
