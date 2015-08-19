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

## Source code

~~~~ {#Norton .cpp .numberLines}
@Parser IsotropicMisesCreep;
@Behaviour Norton;
@Author Helfer Thomas;
@Date 23/11/06;
@Description{
  This viscoplastic behaviour is fully determined by the evolution
  of the equivalent viscoplastic strain "\(p\)" as a function "\(f\)"
  of the Von Mises stress "\(\sigmaeq\)":
  "\["
  "\dot{p}=f\paren{\sigmaeq}=A\,\sigmaeq^{E}"
  "\]"
  where:

  - "\(A\)" is the Norton coefficient.
  - "\(E\)" is the Norton exponent.

  "\(A\)" and "\(E\)" are declared as material properties.
}

@UMATFiniteStrainStrategies[umat] {None,LogarithmicStrain1D};

//! The Norton coefficient
@MaterialProperty real A;
A.setEntryName("NortonCoefficient");

//! The Norton coefficient
@MaterialProperty real E;
E.setEntryName("NortonExponent");

@FlowRule{
  /*!
   * The return-mapping algorithm used to integrate this behaviour
   * requires the definition of \(f\) and \(\deriv{f}{\sigmaeq}\) (see
   * @simo_computational_1998 and @helfer_generateur_2013 for
   * details).
   * 
   * We introduce an auxiliary variable called `tmp` to
   * limit the number of call to the `pow` function
   */
  const real tmp = A*pow(seq,E-1);
  f       = tmp*seq;
  df_dseq = E*tmp;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
