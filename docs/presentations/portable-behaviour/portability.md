% Writing portable behaviour implementation with MFront
% Thomas Helfer
% 2014

\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\tns}[1]{\underset{\tilde{}}{\mathbf{#1}}}
\newcommand{\transpose}[1]{#1^{\mathop{T}}}

\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\sigmaz}{\sigma_{zz}}

\newcommand{\epsilonto}{\epsilon^{\mathrm{to}}}
\newcommand{\epsilonth}{\epsilon^{\mathrm{th}}}

\newcommand{\tepsilonto}{\underline{\epsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\underline{\epsilon}^{\mathrm{el}}}
\newcommand{\tepsilonth}{\underline{\epsilon}^{\mathrm{th}}}
\newcommand{\tdepsilonto}{\underline{\dot{\epsilon}}^{\mathrm{to}}}

\newcommand{\tepsilonvis}{\underline{\epsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonvis}{\underline{\dot{\epsilon}}^{\mathrm{vis}}}

\newcommand{\tepsilonp}{\underline{\epsilon}^{\mathrm{p}}}
\newcommand{\tdepsilonp}{\underline{\dot{\epsilon}}^{\mathrm{p}}}

\newcommand{\talpha}{\underline{\alpha}}
\newcommand{\tdalpha}{\underline{\dot{\alpha}}}
\newcommand{\txi}{\underline{\xi}}
\newcommand{\tdxi}{\underline{\dot{\xi}}}

\newcommand{\tDq}{\underline{\mathbf{D}}}
\newcommand{\trace}[1]{\mathrm{tr}\paren{#1}}
\newcommand{\Frac}[2]{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\dtot}{{\mathrm{d}}}
\newcommand{\paren}[1]{\left(#1\right)}
\newcommand{\nom}[1]{\textsc{#1}}
\newcommand{\bts}[1]{\left.#1\right|_{t}}
\newcommand{\mts}[1]{\left.#1\right|_{t+\theta\,\Delta\,t}}
\newcommand{\ets}[1]{\left.#1\right|_{t+\Delta\,t}}

# Current interfaces to mechanical behaviours

- `umat` for the [CEA](http://www.cea.fr/)'s
  [Cast3M](http://www-cast3m.cea.fr/) finite element solver
- `aster` for the [EDF](http://www.edf.com/)'s
  [Code-Aster](http://www.code-aster.org) finite element
  solver
- `cyrano` for the Cyrano3 fuel performance code.
- `zmat` for the [Zebulon](www.zset-software.com/products/zebulon/)
  finite elemnet solver developed by the
  [École des Mines ParisTech](http://www.mines-paristech.fr/)
  (France), [Onera](http://www.onera.fr),
  [NW Numerics & Modeling](http://www.nwnumerics.com), Inc (USA) and
  various academic partners.

# Supported modelling hypotheses

|   Solver  |                                                         Convention                                                 |
|:---------:|:------------------------------------------------------------------------------------------------------------------:|
|Cast3M     | Axisymmetrical generalised plane strain, plane strain, generalised plane strain, plane stress, axisymmetrical, tridimensional  |
|Code-Aster | Plane strain, plane stress (1), axisymmetrical, tridimensional                                                     |
|Zebulon    | \(1D\), \(2D\), \(3D\) (2)                                                                                         |
|Cyrano     | Axisymmetrical generalised plane strain, axisymmetrical generalised plane stress (3)                               |

(1) Can be supported by the behaviour or by the solver (De Borst)
(2) Modelling hypothesis is not accessible within the behaviour. Plane stress is supported
(3) The default.

# Plane stress

- Supported for plane stress and axisymmetrical generalised plane stress *shall* be explicit
- See the `MFront` documentation

# Prediction operator

|:---------:|:-------:|
|Cast3M     | No (1)  |
|Code-Aster | Elastic, secant, tangent |
|Zebulon    | No      |
|Cyrano     | No      |

(1) Expected in PLEIADES version (2014 or 2015 ?)

# Consistent tangent operator

|:---------:|:-------:|
|Cast3M     | No (1)  |
|Code-Aster | Elastic, secant, tangent, consistent tangent operator  |
|Zebulon    | Consistent tangent operator (2)  |
|Cyrano     | Consistent tangent operator      |

(1) Expected in PLEIADES version (2014 or 2015 ?)
(2) By default.

# Finite strains

![](img/LogarithmicStrainPlasticity-ssna303.svg)

- for standard laws, **use the Miehe logarithmic strains framework**:
    - available in Cast3M (through `MFront`), Code-Aster, ZeBuLoN
	- can easily inserted in Cyrano (paper under review)
- avoid using Cast3M's `PASAPAS` `GRANDS_DEPLACEMENTS` option

# Consistent tangent operator in finite strain

|:---------:|:-------:|
|Cast3M     | No  |
|Code-Aster | \(\deriv{\tenseur{\tau}}{\Delta\, \tns{F}}\) (1)  |
|Zebulon    | `DSIG_DD` (2)  |

**MFront generates convertion between consistent tangent operators :
  use the most natural one for your formalism**

(1) \(\Delta\, \tns{F} = \ets{\tns{F}}.\left(\bts{\tns{F}}\right)^{-1}\)
(2) The only finite strain formulation supported by `MFront` is the
`Updated_Lagrangian`.



# Orthotropy

![](img/pipe.svg "Recommended definition for the orthotropic axes")

- behaviour and orthotropic axes definitions **must be consistent**
- no general purpose solver allow uniform definition of the
  orthotropic axes for all modelling hypotheses
    - example of pipes

# Strains

|   Solver  |                                                         Convention                                                                            |
|:---------:|:---------------------------------------------------------------------------------------------------------------------------------------------:|
|TFEL       |\(\left(\epsilonto_{xx},\epsilonto_{yy},\epsilonto_{zz},\sqrt{2}\,\epsilonto_{xy},\sqrt{2}\,\epsilonto_{xy},\sqrt{2}\,\epsilonto_{yz}\right)\) |
|Cast3M     |\(\left(\epsilonto_{xx},\epsilonto_{yy},\epsilonto_{zz},2\,\epsilonto_{xy},2\,\epsilonto_{xy},2\,\epsilonto_{yz}\right)\)                      |
|Code-Aster |\(\left(\epsilonto_{xx},\epsilonto_{yy},\epsilonto_{zz},\sqrt{2}\,\epsilonto_{xy},\sqrt{2}\,\epsilonto_{xy},\sqrt{2}\,\epsilonto_{yz}\right)\) |
|Zebulon    |\(\left(\epsilonto_{xx},\epsilonto_{yy},\epsilonto_{zz},\sqrt{2}\,\epsilonto_{xy},\sqrt{2}\,\epsilonto_{yz},\sqrt{2}\,\epsilonto_{xz}\right)\) |

## 1D case


|   Solver  |                   Convention                                        |
|:---------:|:-------------------------------------------------------------------:|
|Cast3M     |\(\left(\epsilonto_{rr},\epsilonto_{zz},\epsilonto_{\theta}\right)\) |
|Code-Aster |\(\left(\epsilonto_{rr},\epsilonto_{zz},\epsilonto_{\theta}\right)\) |
|Cyrano3    |\(\left(\epsilonto_{rr},\epsilonto_{\theta},\epsilonto_{zz}\right)\) |

# Stresses

|   Solver  |                                                         Convention                                                    |
|:---------:|:---------------------------------------------------------------------------------------------------------------------:|
|TFEL       |\(\left(\sigma_{xx},\sigma_{yy},\sigma_{zz},\sqrt{2}\,\sigma_{xy},\sqrt{2}\,\sigma_{xy},\sqrt{2}\,\sigma_{yz}\right)\) |
|Cast3M     |\(\left(\sigma_{xx},\sigma_{yy},\sigma_{zz},\sigma_{xy},\sigma_{xy},\sigma_{yz}\right)\)                               |
|Code-Aster |\(\left(\sigma_{xx},\sigma_{yy},\sigma_{zz},\sqrt{2}\,\sigma_{xy},\sqrt{2}\,\sigma_{xy},\sqrt{2}\,\sigma_{yz}\right)\) |
|Zebulon    |\(\left(\sigma_{xx},\sigma_{yy},\sigma_{zz},\sqrt{2}\,\sigma_{xy},\sqrt{2}\,\sigma_{yz},\sqrt{2}\,\sigma_{xz}\right)\) |

|   Solver  |                   Convention                                        |
|:---------:|:-------------------------------------------------------------------:|
|Cast3M     |\(\left(\sigma_{rr},\sigma_{zz},\sigma_{\theta}\right)\)              |
|Code-Aster |\(\left(\sigma_{rr},\sigma_{zz},\sigma_{\theta}\right)\)              |
|Cyrano3    |\(\left(\sigma_{rr},\sigma_{\theta},\sigma_{zz}\right)\)              |

# Convertions

- Convertions are handled by the interface
- Internal state variables are stored using TFEL conventions
    - In the material frame (for orthotropic behaviours)
    - Except for ZeBuLoN

<!-- pandoc -s -f markdown+tex_math_single_backslash --bibliography=bibliography.bib --filter pandoc-citeproc --slide-level 1 --toc --mathjax -i -t revealjs -V theme:beige -H mysettings.css  portability.md -o portability.html -->
