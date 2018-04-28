% How to implement an isotropic plastic behaviour with isotropic linear hardening in MFront
% Thomas Helfer, Lorenzo Riparbelli, Ioannis Christovasilis
% 24/04/2018

\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}

\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonp}{\tenseur{\varepsilon}^{\mathrm{p}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}

This article shows how to implement an orthotropic plastic behaviour
with isotropic linear hardening in `MFront`.

# Description of the behaviour

The behaviour is described by a standard decomposition of the strain
\(\tepsilonto\) in an elastic and a plastic component, respectively
denoted \(\tepsilonel\) and \(\tepsilonp\):

\[
\tepsilonto=\tepsilonel+\tepsilonp
\]

## Elastic behaviour

The stress \(\tsigma\) is related to the the elastic strain
\(\tepsilonel\) by a the standard Hooke behaviour:

\[
\tsigma = \lambda\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\,\tepsilonel
\]

## Plastic flow

The plastic part of the behaviour is described by the following yield
surface:
\[
f\paren{\sigmaeq,p} = \sigmaeq-\sigma_{0}-R\,p
\]

where \(\sigmaeq\) is the Hill stress defined below, \(p\) is the
equivalent plastic strain, \(\sigma_{0}\) is the yield stress and
\(R\) is the hardening slope.

The von Mises stress is defined by:
\[
\sigmaeq=\sqrt{\Frac{3}{2}\,\tenseur{s}\,\colon\,\tenseur{s}}=\sqrt{3\,J_{2}}
\]
where:

- \(\tenseur{s}\) is the deviatoric stress defined as follows:
\[
\tenseur{s}=\tsigma-\Frac{1}{3}\,\trace{\tsigma}\,\tenseur{I}
\]
- \(J_{2}\) is the second invariant of \(\tenseur{s}\).

The previous expression can be rewritten by introducing a fourth order
tensor called \(\tenseurq{M}\):
\[
\sigmaeq=\sqrt{\sigma\,\colon\,\tenseurq{M}\,\colon\,\tsigma}
\]

The tensor \(\tenseurq{M}\) is given by:
\[
\tenseurq{M}=\Frac{3}{2}\,\paren{\tenseurq{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}}
\]

The plastic flow is assumed to be associated, so the flow direction
\(\tenseur{n}\) is given by \(\deriv{f}{\tsigma}\):

\[
\tenseur{n} = \deriv{f}{\tsigma} = \Frac{3}{2\,\sigmaeq}\,\tenseur{s}
\]

# Integration algorithm

The previous constitutive equations will be integrated using a
standard implicit scheme.



~~~~{.cpp}
@DSL DefaultDSL;
@Behaviour IsotropicLinearHardeningPlasticity;
@Author Thomas Helfer;
@Date   14/10/2016;
@Description{
  An implicit implementation of a simple
  isotropic plasticity behaviour with
  isotropic linear hardening.

  The yield surface is defined by:
  "\["
  "  f(\sigmaeq,p) = \sigmaeq-s_{0}-H\,p"
  "\]"
}

@StateVariable StrainStensor eel;
eel.setGlossaryName("ElasticStrain");
@StateVariable strain p;
p.setGlossaryName("EquivalentPlasticStrain");

@Parameter young =  70.e9;
young.setGlossaryName("YoungModulus");
@Parameter nu    =   0.34;
nu.setGlossaryName("PoissonRatio");
@Parameter H     =  10.e9;
H.setEntryName("HardeningSlope");
@Parameter s0    =  300.e6;
s0.setGlossaryName("YieldStress");

/*!
 * computation of the prediction operator: we only provide the elastic
 * operator.
 *
 * We could also provide a tangent operator, but this would mean
 * saving an auxiliary state variable stating if a plastic loading
 * occured at the previous time step.
 */
@PredictionOperator{
  const auto lambda = computeLambda(young,nu);
  const auto mu     = computeMu(young,nu);
  Dt = lambda*Stensor4::IxI()+2*mu*Stensor4::Id();
}

/*!
 * behaviour integration using a fully implicit Euler-backwark scheme.
 */
@ProvidesSymmetricTangentOperator;
@Integrator{
  const auto lambda = computeLambda(young,nu);
  const auto mu     = computeMu(young,nu);
  eel += deto;
  const auto se     = 2*mu*deviator(eel);
  const auto seq_e  = sigmaeq(se);
  const auto b      = seq_e-s0-H*p>stress{0};
  if(b){
    const auto iseq_e = 1/seq_e;
    const auto n      = eval(3*se/(2*seq_e));
    const auto cste   = 1/(H+3*mu);
    dp   = (seq_e-s0-H*p)*cste;
    eel -= dp*n;
    if(computeTangentOperator_){
      if(smt==CONSISTENTTANGENTOPERATOR){
	Dt = (lambda*Stensor4::IxI()+2*mu*Stensor4::Id()
	      -4*mu*mu*(dp*iseq_e*(Stensor4::M()-(n^n))+cste*(n^n)));
      } else {
	Dt = lambda*Stensor4::IxI()+2*mu*Stensor4::Id();
      }
    }
  } else {
    if(computeTangentOperator_){
      Dt = lambda*Stensor4::IxI()+2*mu*Stensor4::Id();
    }
  }
  sig = lambda*trace(eel)*Stensor::Id()+2*mu*eel;
}
~~~~