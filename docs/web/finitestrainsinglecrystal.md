% A framework to implement finite strain single crystal behaviors, introducing the `FiniteStrainSingleCrystal` brick
% Thomas Helfer
% 27/03/2017

\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathrm{T}}}}
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
\newcommand{\talpha}{\underline{\alpha}}
\newcommand{\tdalpha}{\underline{\dot{\alpha}}}
\newcommand{\txi}{\underline{\xi}}
\newcommand{\tdxi}{\underline{\dot{\xi}}}
\newcommand{\tDq}{\underline{\mathbf{D}}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\nom}[1]{\textsc{#1}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

This page describes a general framework to implement finite strain
single crystal behaviors.

The first section is dedicated to the description of the thermodynamic
bases of the framework presented, based on the work of Xhu Han (see
@Han2012).

The second part describes the implicit scheme used to integrate this
behaviour and the computation of the consistent tangent operator. This
part is based on the `Code_Aster` documentation (@edf_lois_2011).

The third part details the `MFront` implementation of this behaviour.

The last part introduces the `FiniteStrainSingleCrystal` brick which
hides all the details described in the previous sections.

# Thermodynamical basis

# Description of the implicit scheme

# Implementation in MFront

## Description of the slip systems and of the interaction matrix

This section shows how to define:

- the crystal structure.
- the slip systems.
- the interaction matrix.

### Crystal structure

The crystal structure must be defined through the `@CrystalStructure`
keyword. This keyword is followed by one of the following value:

- `Cubic`: cubic structure.
- `FCC`: face centered cubic structure.
- `HCP`: hexagonal closed-packed structures.

~~~~{.cpp}
@CrystalStructure FCC;
~~~~

### Definition of the slip systems

A single slip systems family can be defined by one of the following
keywords: `@SlidingSystem`, `@GlidingSystem` or `@SlipSystem`. Those
keywords are followed by the definition of one family of slip systems.

Several slip systems families can be defined by one of the following
keywords: `@SlidingSystems`, `@GlidingSystems` or
`@SlipSystems`. Those keywords are followed by an array giving the
definition of the families of slip systems.

For `Cubic` and `FCC` crystal structures, a slip system is defined by
its plane and its Burgers'vector as follows:

~~~~{.cpp}
{1,-1,0}<1,1,1>
~~~~

Thus, an example of the usage of the `@SlidingSystem` keyword is:

~~~~{.cpp}
@SlidingSystem {1,-1,0}<1,1,1>;
~~~~

For a given slip systems family, a set of slip systems are generated
by symmetry using code extracted from the `NUMODIS` code. The previous
example defines \(12\) slip systems.

Once the sliding system families has been defined, a static integer
variable called `Nss` is available which contains the total number of
slip systems defined. In the previous example, `Nss` value is \(12\).

This value can be used to define additional state variables:

~~~~{.cpp}
//! equivalent plastic strain on each slip system
@AuxiliaryStateVariable real p[Nss];
~~~~

For post-processing reasons, one needs to know in which order the slip
systems are defined. This question is treated in the next paragraph.

Also, note that for the \(i^{\text{th}}\) slip systems family, a
static integer variable beginning by `Nss` and followed by the value
of \(i\) is defined. In the previous example, a unique slip systems
family is defined and a variable `Nss0` is made available. Of course,
in this case `Nss0` is equal to `Nss`. Those variables may be useful
to define variables specific to a slip system family.

#### Getting information about the generated slip systems

The list of the generated slip systems can be retrieved using
`mfront-query`.

Consider the following example:

~~~~{.cpp}
@DSL       ImplicitFiniteStrain;
@Behaviour SlipSystemGenerationTest;
@Brick     FiniteStrainSingleCrystal;
@CrystalStructure FCC;
@SlidingSystem <1,-1,0>{1,1,1};
~~~~

If the previous code is saved a file called
`SlipSystemGenerationTest.mfront`, one may use `mfront-query` as
follows:

~~~~{.sh}
$ mfront-query --slip-systems
- (1,-1,0)[1,1,1]: (0,1,1)[1,1,-1] (0,1,1)[1,-1,1] (0,1,-1)[1,1,1] (0,1,-1)[1,-1,-1] (1,0,1)[1,1,-1] (1,0,1)[1,-1,-1] (1,0,-1)[1,1,1] (1,0,-1)[1,-1,1] (1,1,0)[1,-1,-1] (1,1,0)[1,-1,1] (1,-1,0)[1,1,1] (1,-1,0)[1,1,-1]
~~~~

The output shows that \(12\) slip systems were generated. All those
systems are equivalent by symmetry.

Concerning slip systems, the following queries are available:

~~~~{.sh}
$ mfront-query --help-behaviour-queries-list |grep slip
--slip-systems          : list all the slip systems, sorted by family
--slip-systems-by-index : list all the slip systems
~~~~

The `--slip-systems-by-index` query gives the index associated to a
given slip system, which is helpfull for postprocessing purposes. For
this example:

~~~~{.sh}
$ mfront-query --slip-systems-by-index
- 0: (0,1,1)[1,1,-1]
- 1: (0,1,1)[1,-1,1]
- 2: (0,1,-1)[1,1,1]
- 3: (0,1,-1)[1,-1,-1]
- 4: (1,0,1)[1,1,-1]
- 5: (1,0,1)[1,-1,-1]
- 6: (1,0,-1)[1,1,1]
- 7: (1,0,-1)[1,-1,1]
- 8: (1,1,0)[1,-1,-1]
- 9: (1,1,0)[1,-1,1]
- 10: (1,-1,0)[1,1,1]
- 11: (1,-1,0)[1,1,-1]
~~~~

### Definition of the interaction matrix

## Implementation of the implicit system

Le second tenseur de Piola-Kirchhoff est défini par la loi élastique de
Saint-Venant Kirchhoff\textasciitilde{}: \[
\tenseur{S}=\tenseurq{D}\,.\,\paren{\bts{\tepsilonel}+\Delta\,\tepsilonel}
\]

Nous calculons (une approximation) de l'inverse de l'incrément de la
partie plastique du gradient de la transformation\textasciitilde{}: \[
\paren{\Delta\,\tns{F}_{p}}^{-1} = \tns{I}-\sum_{i=1}^{12}\Delta\,g_{i}\,\tns{\mu}_{i}
\]

Nous calculons la partie élastique de gradient de la transformation
$\tns{F}_{e}$ en fin de pas de temps en fin de pas de
temps\textasciitilde{}: \[
\tns{F}_{e}=\tns{F}_{e}^{\mathrm{tr}}\,\star\,\paren{\Delta\,\tns{F}_{p}}^{-1}
\] où $\star$ représente le produit des représentations matricielles de
$\tns{F}_{e}^{\mathrm{tr}}$ et $\paren{\Delta\,\tns{F}_{p}}^{-1}$.

L'équation associée à l'incrément de déformation élastique impose
qu'elle soit égale au tenseur de Green-Lagrange
$\tepsilonel_{\mathrm{GL}}$ associé à la partie élastique
$\tns{F}_{e}$ du gradient de la transformation\textasciitilde{}:
\[
\bts{\tepsilonel}+\Delta\,\bts{\tepsilonel}-
\tepsilonel_{\mathrm{GL}}\paren{\tns{F}_{e}}=0
\]

Le calcul des termes $\deriv{f_{\tepsilonel}}{\Delta\,g_{i}}$ est assez
technique et devra se faire en plusieurs étapes\textasciitilde{}: \[
\deriv{\tenseur{\varepsilon_{\mathrm{GL}}}}{\Delta\,g_{i}}=
\Frac{1}{2}\,\deriv{\tenseur{C}_{e}}{\Delta\,g_{i}}=
\Frac{1}{2}\,\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}\,.\,
\deriv{\tns{F}_{e}}{\Delta\,g_{i}}
\]

La classe t2tost2 fournit une méthode statique permettant de calculer la
dérivée du tenseur de Cauchy élastique $\tenseur{C}_{e}$ par rapport au
gradient de la transformation élastique $F_{e}$.

Il faut alors calculer le terme $\deriv{\tns{F}_{e}}{\Delta\,g_{i}}$. Le
tenseur $\tns{F}_{e}$ est le produit $\star$ des représentation
matricielle de deux tenseurs\textasciitilde{}:
$\tns{F}_{e}^{\mathrm{tr}}$ et le tenseur
$\paren{\Delta\,\tns{F}_{p}}^{-1}$.

Cette démarche est maintenant appliquée au calcul de la dérivée du
tenseur $\tns{F}_{e}$\textasciitilde{}: \[
\deriv{\tns{F}_{e}}{\paren{\Delta\,\tns{F}_{p}}^{-1}} =
\partial^{r}_{\star}\paren{\tns{F}_{e}^{\mathrm{tr}}}
\]

En négligeant la correction plastique, la dérivée
$\deriv{\paren{\Delta\,\tns{F}_{p}}^{-1}}{\Delta\,g_{i}}$ s'écrit
simplement\textasciitilde{}: \[
\deriv{\paren{\Delta\,\tns{F}_{p}}^{-1}}{\Delta\,g_{i}} = -\tns{\mu}_{i}
\]

Nous obtenons finalement l'expression la dérivée
$\deriv{f}{\Delta\,g_{i}}$\textasciitilde{}: \[
\deriv{f_{\tepsilonel}}{\Delta\,g_{i}}=
-\Frac{1}{2}\,\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}\,.\,
\partial^{r}_{\star}\paren{\tns{F}_{e}^{\mathrm{tr}}}\,.\,
\tns{\mu}_{i}
\]

## Implementation of the consistent tangent operator

Le point de départ du calcul est la relation\textasciitilde{}: \[
\deriv{\tenseur{\tau}}{\Delta\,\tns{F}}=\deriv{\tenseur{\tau}}{\tns{F}_{e}}\deriv{\tns{F}_{e}}{\Delta\,\tns{F}}
\]

Nous calculons maintenant chacun des termes du produit.

La contrainte de Kirchhoff $\tenseur{\tau}$ se calcule par le transport
dans la configuration finale du second tenseur de Piola-Kirchhoff
calculé dans la configuration intermédiaire. \[
\tenseur{\tau} = \tns{F}_{e}\,\star\,\tenseur{S}\,\star\,\transpose{\tns{F}_{e}}
\] où $\star$ représente le produit matriciel.

TFEL propose la méthode computePushForwardDerivative permettant de
relier la dérivée $\deriv{\tenseur{\tau}}{\tns{F}_{e}}$ à la dérivée du
second tenseur de Piola-Kirchhoff $\deriv{\tenseur{S}}{\tns{F}_{e}}$.
Cette dérivée se calcule par dérivation en chaînes\textsubscript{: \[
\deriv{\tenseur{S}}{\tns{F}_{e}}=\deriv{\tenseur{S}}{\tepsilonel_{\mathrm{GL}}}\deriv{\tepsilonel_{\mathrm{GL}}}{\tenseur{C}_{e}}\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}
\] soit}: \[
\deriv{\tenseur{S}}{\tns{F}_{e}}=\Frac{1}{2}\,D\,\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}
\]

La dérivée $\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}$ se calcule par la
méthode statique dCdF de la classe t2tost2.

Il nous fait maintenant calculer le terme
$\deriv{\tns{F}_{e}}{\Delta\,\tns{F}}$.

Nous savons que\textasciitilde{}: \[
\tns{F}_{e}=\tns{F}_{e}^{\mathrm{tr}}\,\star\,\paren{\Delta\,\tns{F}_{p}}^{-1}
\] Par dérivation, nous obtenons\textasciitilde{}: \[
\deriv{\tns{F}_{e}}{\Delta\,\tns{F}}=
\partial^{l}_{\star}\paren{\paren{\Delta\,\tns{F}_{p}}^{-1}}\,\deriv{\tns{F}_{e}^{\mathrm{tr}}}{\Delta\,\tns{F}}+
\partial^{r}_{\star}\paren{\tns{F}_{e}^{\mathrm{tr}}}\,\deriv{\paren{\Delta\,\tns{F}_{p}}^{-1}}{\Delta\,\tns{F}}
\]

Le calcul du terme $\deriv{\tns{F}_{e}^{\mathrm{tr}}}{\Delta\,\tns{F}}$
est aisé\textasciitilde{}: \[
\deriv{\tns{F}_{e}^{\mathrm{tr}}}{\Delta\,\tns{F}}=
\partial^{l}_{\star}\paren{\bts{\tns{F}_e}}
\]

Pour le calcul de la dérivée
$\deriv{\paren{\Delta\,\tns{F}_{p}}^{-1}}{\Delta\,\tns{F}}$, nous
pouvons nous baser sur l'approximation faite lors de l'intégration. Nous
avons alors directement\textasciitilde{}: \[
\deriv{\paren{\Delta\,\tns{F}_{p}}^{-1}}{\Delta\,\tns{F}}=
-\sum_{i=1}^{12}\tns{\mu}_{i}\,\otimes\,\deriv{\Delta\,g_{i}}{\Delta\,\tns{F}}
\]

Le calcul de $\deriv{\Delta\,g_{i}}{\Delta\,\tns{F}}$ représente la
seule vraie difficulté. Son calcul peut se faire en généralisant les
techniques utilisées en petites déformations.

Pour toute variation de $\delta\,\Delta\,\tns{F}$ de $\Delta\,\tns{F}$,
la variation du système implicite $\delta\,F$ est
nulle\textasciitilde{}: \[
\delta\,F\paren{\Delta\,Y\paren{\delta\,\Delta\,\tns{F}},\delta\,\Delta\,\tns{F}}
=J\,\delta\,\Delta\,Y+
\left.\deriv{F}{\Delta\,\tns{F}}\right|_{\paren{\Delta\,\tepsilonel,\Delta\,g_{i}}}\,\delta\,\Delta\,\tns{F}=0
\]

Nous avons explicitement spécifié, en utilisant la notation classique de
la thermodynamique, que le calcul du terme
$\left.\deriv{F}{\Delta\,\tns{F}}\right|_{\paren{\Delta\,\tepsilonel,\Delta\,g_{i}}}$
se faisait en supposant que $\Delta\,\tepsilonel$ et $\Delta\,g_{i}$
étaient des variables indépendantes.

Il est donc nécessaire de calculer la dérivée
$\left.\deriv{F}{\Delta\,\tns{F}}\right|_{\paren{\Delta\,\tepsilonel,\Delta\,g_{i}}}$
dont la seule composante non nulle est
$\left.\deriv{f_{\tepsilonel}}{\Delta\,\tns{F}}\right|_{\paren{\Delta\,\tepsilonel,\Delta\,g_{i}}}$\textasciitilde{}:
\[
\left.\deriv{f_{\tepsilonel}}{\Delta\,\tns{F}}\right|_{\paren{\Delta\,\tepsilonel,\Delta\,g_{i}}}=
-\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}\,.\,
\deriv{\tns{F}_{e}}{\Delta\,\tns{F}}=
-\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}\,.\,
\deriv{}{\Delta\,\tns{F}}\paren{\tns{F}_{e}^{\mathrm{tr}}\,\star\,\paren{\Delta\,\tns{F}_{p}}^{-1}}
\]

\[
\deriv{}{\Delta\,\tns{F}}\paren{\tns{F}_{e}^{\mathrm{tr}}\,\star\,\paren{\Delta\,\tns{F}_{p}}^{-1}}=
\partial^{l}_{\star}\paren{\paren{\Delta\,\tns{F}_{p}}^{-1}}\,.\,
\deriv{\tns{F}_{e}^{\mathrm{tr}}}{\Delta\,\tns{F}}=
\partial^{l}_{\star}\paren{\paren{\Delta\,\tns{F}_{p}}^{-1}}\,.\,
\partial^{l}_{\star}\paren{\bts{\tns{F}_{e}}}
\]

Finalement, $\deriv{F}{\Delta\,\tns{F}}$ s'écrit\textasciitilde{}: \[
\deriv{f_{\tepsilonel}}{\Delta\,\tns{F}}=\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}\,.\,
\partial^{l}_{\star}\paren{\paren{\Delta\,\tns{F}_{p}}^{-1}}\,.\,
\partial^{l}_{\star}\paren{\bts{\tns{F}_{e}}}
\]

La dérivée $\deriv{\Delta\,g_{i}}{\Delta\,\tns{F}}$ est
alors\textasciitilde{}: \[
\deriv{\Delta\,g_{i}}{\Delta\,\tns{F}}=-J^{-1}_{\Delta\,g_{i}}|\deriv{f_{\tepsilonel}}{\Delta\,\tns{F}}
\]

# The `FiniteStrainSingleCrystal` brick

The previous framework allows the implementation of a wide range of
finite strain single crystals that differ from their flow rules and
the constitutive equations used to describe their internal state
variables' evolution.

The `FiniteStrainSingleCrystal` brick hides all the details related to
this framework and simplifies the declaration of the slip systems and
the interaction matrix.

## Specific keywords

### The `@SlidingSystem` and `@SlidingSystems` keywords

### The `@InteractionMatrix` keyword

## Variables automatically defined

The plastic slip \(g\) are automatically defined.

## Code blocks automatically declared

The `ComputeFinalStress` code block is automatically declared.

The `TangentOperator` code block is automatically declared.

# Variables automatically computed

The residual \(f_{\tepsilonel}\) associated with the elastic strain
\(\tepsilonel\) is automatically implemented.

The Mandel stress \(\tns{M}\) is automatically computed before the
integrator code block. Its value is stored in the variables `M`.  If
the algorithm requires an analytic jacobian, the derivative
\(\deriv{\tns{M}}{\Delta\,\tepsilonel}\) is also computed and stored
in the variable `dM_ddeel` respectively.

# References

<!-- Local IspellDict: english -->
