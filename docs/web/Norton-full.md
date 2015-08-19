\newcommand{\tensor}[1]{\underline{#1}}
\newcommand{\tensorq}[1]{\underline{\mathbf{#1}}}
\newcommand{\ust}[1]{\underset{\tilde{}}{\mathbf{#1}}}
\newcommand{\transpose}[1]{#1^{\mathop{T}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\epsilonth}{\epsilon^{\mathrm{th}}}
\newcommand{\tepsilonto}{\underline{\epsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\underline{\epsilon}^{\mathrm{el}}}
\newcommand{\tepsilonth}{\underline{\epsilon}^{\mathrm{th}}}
\newcommand{\tepsilonvis}{\underline{\epsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonvis}{\underline{\dot{\epsilon}}^{\mathrm{vis}}}
\newcommand{\tepsilonp}{\underline{\epsilon}^{\mathrm{p}}}
\newcommand{\tdepsilonp}{\underline{\dot{\epsilon}}^{\mathrm{p}}}
\newcommand{\trace}[1]{\mathrm{tr}\paren{#1}}
\newcommand{\Frac}[2]{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\dtot}{\mathrm{d}}
\newcommand{\paren}[1]{\left(#1\right)}
\newcommand{\bts}[1]{\left.#1\right|_{t}}
\newcommand{\mts}[1]{\left.#1\right|_{t+\theta\,\Delta\,t}}
\newcommand{\ets}[1]{\left.#1\right|_{t+\Delta\,t}}

# Norton behaviour description

* file   : Norton.mfront
* author : Helfer Thomas
* date   : 23 / 11 / 06

This viscoplastic behaviour is fully determined by the evolution 
of the equivalent viscoplastic strain \(p\) as a function \(f\) 
of the Von Mises stress \(\sigmaeq\) : 
\[\dot{p}=f\paren{\sigmaeq}=A\,\sigmaeq^{E}\] 


where : 

- \(A\) is the Norton coefficient . 
- \(E\) is the Norton exponent . 

\(A\) and \(E\) are declared as material properties . 


### List of supported Hypotheses

* AxisymmetricalGeneralisedPlaneStrain
* Axisymmetrical
* PlaneStrain
* GeneralisedPlaneStrain
* Tridimensional

## Variables


###  Material properties

* YoungModulus:
	+ variable name: young
	+ variable type: stress
	+ description: the Young modulus of an isotropic material
* PoissonRatio:
	+ variable name: nu
	+ variable type: real
	+ description: the Poisson ratio of an isotropic material
* NortonCoefficient:
	+ variable name: A
	+ variable type: real
	+ description: The Norton coefficient
* NortonExponent:
	+ variable name: E
	+ variable type: real
	+ description: The Norton coefficient

###  State variables

* ElasticStrain:
	+ variable name: eel
	+ variable type: StrainStensor
	+ description: la déformation élastique
* EquivalentViscoplasticStrain:
	+ variable name: p
	+ variable type: strain
	+ description: la déformation viscoplastique équivalente


### Parameters 

###  Parameters

* theta:
	+ variable type: real
	+ default value: 0.5
* epsilon:
	+ variable type: real
	+ default value: 1e-08
* iterMax:
	+ variable type: ushort
	+ default value: 100

###  Local variables

* lambda:
	+ variable type: stress
* mu:
	+ variable type: stress
* T_:
	+ variable type: temperature
* f:
	+ variable type: DstrainDt
* df_dseq:
	+ variable type: DF_DSEQ_TYPE
* se:
	+ variable type: StressStensor
* seq:
	+ variable type: stress
* seq_e:
	+ variable type: stress
* n:
	+ variable type: StrainStensor

## Code documentation

### FlowRule description
The return-mapping algorithm used to integrate this behaviour
requires the definition of \(f\) and \(\deriv{f}{\sigmaeq}\) (see
@simo_computational_1998 and @helfer_generateur_2013 for
details).

We introduce an auxiliary variable called `tmp` to
limit the number of call to the `pow` function
