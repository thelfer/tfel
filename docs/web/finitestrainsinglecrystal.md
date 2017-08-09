% A framework to implement finite strain single crystal behaviors, introducing the `FiniteStrainSingleCrystal` brick
% Thomas Helfer
% 27/03/2017

\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathrm{T}}}}
\newcommand{\inverse}[1]{{#1^{\mathrm{-1}}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\epsilonth}{\epsilon^{\mathrm{th}}}
\newcommand{\tepsilonto}{\underline{\epsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\underline{\epsilon}^{\mathrm{el}}}
\newcommand{\tepsilonth}{\underline{\epsilon}^{\mathrm{th}}}
\newcommand{\tepsilonvis}{\underline{\epsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonel}{\underline{\dot{\epsilon}}^{\mathrm{el}}}
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
- `BCC`: body centered cubic structure.
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
<1,-1,0>{1,1,1}
~~~~

Thus, an example of the usage of the `@SlidingSystem` keyword is:

~~~~{.cpp}
@SlidingSystem <1,-1,0>{1,1,1};
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
$ mfront-query --slip-systems SlipSystemGenerationTest.mfront
- <1,-1,0>{1,1,1}: [0,1,-1](1,1,1) [1,0,-1](1,1,1) [1,-1,0](1,1,1) [0,1,1](1,1,-1) [1,0,1](1,1,-1) [1,-1,0](1,1,-1) [0,1,-1](1,-1,-1) [1,0,1](1,-1,-1) [1,1,0](1,-1,-1) [0,1,1](1,-1,1) [1,0,-1](1,-1,1) [1,1,0](1,-1,1)
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
$ mfront-query --slip-systems-by-index SlipSystemGenerationTest.mfront
- 0: [0,1,-1](1,1,1)
- 1: [1,0,-1](1,1,1)
- 2: [1,-1,0](1,1,1)
- 3: [0,1,1](1,1,-1)
- 4: [1,0,1](1,1,-1)
- 5: [1,-1,0](1,1,-1)
- 6: [0,1,-1](1,-1,-1)
- 7: [1,0,1](1,-1,-1)
- 8: [1,1,0](1,-1,-1)
- 9: [0,1,1](1,-1,1)
- 10: [1,0,-1](1,-1,1)
- 11: [1,1,0](1,-1,1)
~~~~

#### Getting information about the orientation tensors

Concerning the orientation tensors, the following queries are
available:

- `--orientation-tensors`: list all the orientation tensors, sorted by
  family".
- `--orientation-tensors-by-index`: list all the orientation tensors.
- `--orientation-tensors-by-slip-system`: list all the orientation
  tensors.

#### Getting information about the Schmid factors

Concerning the Schmid factors, the following queries are
available:

- `--schmid-factors`: list all the Schmid factors, sorted by
  family".
- `--schmid-factors-by-index`: list all the Schmid factors.

~~~~{.sh}
$ mfront-query --schmid-factors-by-index='<1,1,1>' SlipSystemGenerationTest.mfront
~~~~

**Note** In this example, the direction must be surrounded by simple
quote to avoid interpretation by the shell.

### Definition of the interaction matrix

#### Structure of the interaction matrix

Since many interactions between slip systems are equivalent, the
interaction matrix only contains a few independent coefficients.

Using `mfront-query`, one may have access to the whole matrix using
the `--interaction-matrix` query:

~~~~{.sh}
$ mfront-query --interaction-matrix SlipSystemGenerationTest.mfront 
| 0 1 2 2 3 4 5 6 5 6 4 3 |
| 1 0 2 2 6 5 4 3 4 3 5 6 |
| 2 2 0 1 5 6 3 4 6 5 3 4 |
| 2 2 1 0 4 3 6 5 3 4 6 5 |
| 3 4 5 6 0 1 2 2 6 5 4 3 |
| 6 5 4 3 1 0 2 2 3 4 5 6 |
| 5 6 3 4 2 2 0 1 5 6 3 4 |
| 4 3 6 5 2 2 1 0 4 3 6 5 |
| 5 6 4 3 4 3 5 6 0 1 2 2 |
| 4 3 5 6 5 6 4 3 1 0 2 2 |
| 6 5 3 4 6 5 3 4 2 2 0 1 |
| 3 4 6 5 3 4 6 5 2 2 1 0 |
~~~~

Here only \(7\) coefficients are needed. The number corresponding to a
pair of slip systems defines the rank of the interaction.

The `mfront-query` also provides the `--interaction-matrix-structure`
query which displays the number of independent coefficients and sorts
the pair of slip systems by rank:

~~~~{.sh}
$ mfront-query --interaction-matrix-structure SlipSystemGenerationTest.mfront 
- number of independent coefficients: 7
- rank 0: ([0,1,-1](1,1,1):[0,1,-1](1,1,1)) ([1,0,-1](1,1,1):[1,0,-1](1,1,1)) ([1,-1,0](1,1,1):[1,-1,0](1,1,1)) ([0,1,1](1,1,-1):[0,1,1](1,1,-1)) ([1,0,1](1,1,-1):[1,0,1](1,1,-1)) ([1,-1,0](1,1,-1):[1,-1,0](1,1,-1)) ([0,1,-1](1,-1,-1):[0,1,-1](1,-1,-1)) ([1,0,1](1,-1,-1):[1,0,1](1,-1,-1)) ([1,1,0](1,-1,-1):[1,1,0](1,-1,-1)) ([0,1,1](1,-1,1):[0,1,1](1,-1,1)) ([1,0,-1](1,-1,1):[1,0,-1](1,-1,1)) ([1,1,0](1,-1,1):[1,1,0](1,-1,1))
- rank 1: ([0,1,-1](1,1,1):[1,0,-1](1,1,1)) ([0,1,-1](1,1,1):[1,-1,0](1,1,1)) ([1,0,-1](1,1,1):[0,1,-1](1,1,1)) ([1,0,-1](1,1,1):[1,-1,0](1,1,1)) ([1,-1,0](1,1,1):[0,1,-1](1,1,1)) ([1,-1,0](1,1,1):[1,0,-1](1,1,1)) ([0,1,1](1,1,-1):[1,0,1](1,1,-1)) ([0,1,1](1,1,-1):[1,-1,0](1,1,-1)) ([1,0,1](1,1,-1):[0,1,1](1,1,-1)) ([1,0,1](1,1,-1):[1,-1,0](1,1,-1)) ([1,-1,0](1,1,-1):[0,1,1](1,1,-1)) ([1,-1,0](1,1,-1):[1,0,1](1,1,-1)) ([0,1,-1](1,-1,-1):[1,0,1](1,-1,-1)) ([0,1,-1](1,-1,-1):[1,1,0](1,-1,-1)) ([1,0,1](1,-1,-1):[0,1,-1](1,-1,-1)) ([1,0,1](1,-1,-1):[1,1,0](1,-1,-1)) ([1,1,0](1,-1,-1):[0,1,-1](1,-1,-1)) ([1,1,0](1,-1,-1):[1,0,1](1,-1,-1)) ([0,1,1](1,-1,1):[1,0,-1](1,-1,1)) ([0,1,1](1,-1,1):[1,1,0](1,-1,1)) ([1,0,-1](1,-1,1):[0,1,1](1,-1,1)) ([1,0,-1](1,-1,1):[1,1,0](1,-1,1)) ([1,1,0](1,-1,1):[0,1,1](1,-1,1)) ([1,1,0](1,-1,1):[1,0,-1](1,-1,1))
.....
~~~~

In this example, the rank \(0\) contains all the interactions of a
slip system with itself.

#### Definition of the interaction matrix

The interaction matrix is defined through the `@InteractionMatrix`
keyword.

##### Explicit definition

##### Definition using a model

# A generic framework for finite strain single crystal plasticity

Les notions principales liées au formalisme en grandes déformations
sont présentées. Cette partie s'appuie sur les références
\cite{ForestMMC} et \cite{Han12}.

Le vecteur \(\vec{X}\) désigne les coordonnées d'un point dans la
configuration initiale \(\mathit{C}_0\) et le vecteur \(\vec{x}\)
désigne les coordonnées dans la configuration finale
\(\mathit{C}\). Le gradient de la transformation s'écrit :
\begin{equation} \tns{F} = \Frac{\partial \vec{x}}{\partial \vec{X}}
\]

Pour la plasticité cristalline, la décomposition multiplicative du
gradient de la transformation \(\tns{F}\) est adoptée selon
\cite{Mandel73} :

\[
\tns{F} = \tns{F}_{e}\,\cdot\, \tns{F}_{p}
\]

avec la partie plastique \(\tns{F}_{p}\) pour l'écoulement plastique
induit par les glissements entre différents systèmes de glissement, et
la partie élastique \(\tns{F}_{e}\) pour la déformation élastique pure
et la rotation de corps rigide du réseau cristallin. De plus, la
déformation élastique de Green-Lagrange est définie par :

\[
\tepsilonel_{\mathrm{GL}} =\Frac{1}{2} \left( \transpose{\tns{F}_{e}} \tns{F}_{e} - \tenseur{I} \right)
\]

Le gradient du vecteur vitesse \(\tns{L}\) est défini par :

\[
\tns{L} = \vec{\nabla} \vec{v} = \dot{\tns{F}} \inverse{\tns{F}}
\]

où \(\vec{v}\) est le vecteur vitesse. Compte tenu de la décomposition
multiplicative du gradient de transformation, le gradient du vecteur
vitesse \(\tns{L}\) s'exprime :

\[
\tns{L} = \dot{\tns{F}} \inverse{\tns{F}} = \dot{\tns{F}_{e}} \inverse{\tns{F}_{e}} +  \tns{F}_{e} \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} \inverse{\tns{F}_{e}} = \tns{L}^e + \tns{L}^p
\]

avec \(\tns{L}^e=\dot{\tns{F}_{e}} \inverse{\tns{F}_{e}}\) et
\(\tns{L}^p=\tns{F}_{e} \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}}
\inverse{\tns{F}_{e}}\).

Pour la plasticité cristalline, \(\dot{\tns{F}_{p}}
\inverse{\tns{F}_{p}}\) peut être déterminé sur les 12 systèmes de
glissement des matériaux CFC de type \(\{111\}\left<110\right>\) par :

\[
\dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} = \sum \limits_{s=1}^{12} \dot{\gamma}^s \tns{N}^s
\] {#eq:PP}

avec le tenseur d'orientation \(\tns{N}^s\) pour le système de
glissement \(s\) et le taux de glissement \(\dot{\gamma}^s\) pour le
système de glissement \(s\).

Par rapport à la description des contraintes, plusieurs notions sont utilisées : 
\begin{enumerate}
\item le tenseur des contraintes de Cauchy \(\tsigma\), défini dans la configuration actuelle, symétrique ;
\item le second tenseur de Piola-Kirchhoff \(\tenseur{S}^e\), défini dans la configuration intermédiaire, symétrique : 
\[
\tenseur{S}^e=J_E \inverse{\tns{F}_{e}} \tsigma \inverseT{\tns{F}_{e}} \rm{, avec} \ J_E = \det{\tns{F}_{e}}=\Frac{V}{V_i}=\Frac{\rho_i}{\rho}
\]
avec \(V\) le volume du monocristal dans la configuration actuelle, \(V_i\) le volume du monocristal dans la configuration intermédiaire, \(\rho\) la masse volumique du monocristal dans la configuration actuelle et \(\rho_i\) la masse volumique dans la configuration intermédiaire.
\item le tenseur de Mandel, défini dans la configuration intermédiaire, non symétrique : 
\[
\tns{M} = J_E \transpose{\tns{F}_{e}} \tsigma \inverseT{\tns{F}_{e}} = \transpose{\tns{F}_{e}} \tns{F}_{e} \tenseur{S}^e= \tenseur{C}^e \tenseur{S}^e
\]

avec \(\tenseur{C}^e\) le tenseur des dilatations élastiques
Cauchy-Green droit.
\end{enumerate}

Dans la suite, la formulation thermodynamique est présentée pour le
monocristal. Le deuxième principe de la thermodynamique dans sa forme
locale, qui est connu sous le nom d'*Inégalité de
Clausius-Duhem* s'écrit :

\[
-\rho (\dot e - T \dot s) + \tsigma\,\colon\, \tenseur{D} - \Frac{\vec{q}}{T} \cdot \vec{\nabla} T \geqslant 0
\] {#eq:Clausius-Duhem}

avec \(\rho\) la masse volumique du monocristal dans la configuration
actuelle, \(e\) la densité d'énergie interne, \(T\) la température,
\(s\) la densité d'entropie, \(\tenseur{D}\) le tenseur vitesse de
déformation
\(\tenseur{D} = \Frac{1}{2}\paren{\tns{L} + \transpose{\tns{L}}}\)
et \(\vec{q}\) le flux de chaleur. Pour le
monocristal, la puissance des efforts intérieurs est:

\[
\begin{aligned}
\Frac{1}{\rho} \tsigma\,\colon\, \tenseur{D} &=& \Frac{1}{\rho}\tsigma\,\colon\, \tns{L} = \Frac{1}{\rho}\tsigma\,\colon\, \dot{\tns{F}} \inverse{\tns{F}} \\
&=& \Frac{1}{\rho}\tsigma\,\colon\, (\dot{\tns{F}_{e}} \inverse{\tns{F}_{e}}+\tns{F}_{e} \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} \inverse{\tns{F}_{e}}) \\ 
&=& \Frac{1}{\rho}\tsigma\,\colon\, (\dot{\tns{F}_{e}} \inverse{\tns{F}_{e}}) + \Frac{1}{\rho}\tsigma\,\colon\, (\tns{F}_{e} \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} \inverse{\tns{F}_{e}})
\end{aligned}
\] {#eq:power_internal_forces_0}
									
Dans l'équation [@eq:power_internal_forces_0], comme \(\tsigma\)
est un tenseur symétrique, on peut donc remplacer \(\tenseur{D}\) par
\(\tns{L}\). Dans l'équation [@eq:power_internal_forces_1], tenant
compte de la relation entre le second tenseur de Piola-Kirchhoff
\(\tenseur{S}^e\) et le tenseur de Cauchy \(\tsigma\), on a

\[
\begin{aligned}
\frac{1}{\rho} \tsigma\,\colon\, \tenseur{D}
&=& \frac{1}{\rho_i} (\tns{F}_{e} \tenseur{S}^e \transpose{\tns{F}_{e}})\,\colon\, (\dot{\tns{F}_{e}} \inverse{\tns{F}_{e}}) + \frac {1}{\rho_i} (\tns{F}_{e} \tenseur{S}^e \transpose{\tns{F}_{e}})\,\colon\, (\tns{F}_{e} \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} \inverse{\tns{F}_{e}}) \\
&=& \frac{1}{\rho_i} \tenseur{S}^e\,\colon\, (\transpose{\tns{F}_{e}} \dot{\tns{F}_{e}}) + \frac{1}{\rho_i} (\transpose{\tns{F}_{e}} \tns{F}_{e} \tenseur{S}^e)\,\colon\, (\dot{\tns{F}_{p}} \inverse{\tns{F}_{p}})
\end{aligned}									
\]

Comme le second tenseur de Piola-Kirchhoff est symétrique, on a :

\[
\begin{aligned}
\frac{1}{\rho} \tsigma\,\colon\, \tenseur{D}
&=& \frac{1}{\rho_i} \tenseur{S}^e\,\colon\, \Frac{1}{2}\left( \transpose{\tns{F}_{e}} \dot{\tns{F}_{e}} + \transpose{\dot{\tns{F}_{e}}} \tns{F}_{e}\right)+ \frac{1}{\rho_i} (\transpose{\tns{F}_{e}} \tns{F}_{e} \tenseur{S}^e)\,\colon\, (\dot{\tns{F}_{p}} \inverse{\tns{F}_{p}}) \\
&=& \frac{1}{\rho_i} \tenseur{S}^e\,\colon\, \tepsilonel_{\mathrm{GL}}+ \frac{1}{\rho_i} \tns{M}\,\colon\, (\dot{\tns{F}_{p}} \inverse{\tns{F}_{p}})
\end{aligned}
\] {#eq:power_internal_forces}

Pour le monocristal sans écrouissage, la densité d'énergie interne
\(e\) est une fonction de la déformation élastique de Green-Lagrange
\(\tepsilonel_{\mathrm{GL}}\) et de la densité d'entropie \(s\) :

\[
e=e \left( \tepsilonel_{\mathrm{GL}},s \right)
\]

la densité d'énergie libre \(\psi\) est une fonction de
\(\tepsilonel_{\mathrm{GL}}\) et de la température \(T\) :

\[
\psi=\psi \left( \tepsilonel_{\mathrm{GL}},T \right)
\]

On sait que la relation entre la densité d'énergie libre \(\psi\) et
la densité d'énergie interne \(e\) s'écrit :

\[
\psi=e-Ts
\] {#eq:internal_and_free_E}

Compte tenu des équations [@eq:power_internal_forces] et
[@eq:internal_and_free_E], l'inégalité de Clausisu-Duhem devient :

\[
\rho \left(  \frac{\tenseur{S}^{e}}{\rho_i} - \frac{\partial \psi}{\partial \tepsilonel_{\mathrm{GL}}} \right)\,\colon\, \tdepsilonel_{\mathrm{GL}} - \rho \left( s + \frac{\partial \psi}{\partial T}\right) \dot T +\frac{\rho}{\rho_i} \tns{M}\,\colon\, \left( \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} \right) - \frac{\vec{q}}{T} \cdot \vec{\nabla} T \geqslant 0
\] {#eq:Clausius-Duhem_crystal}

où on a la dissipation thermique \(D^{t\!h}\) :

\[
D^{t\!h} = - \frac{\vec{q}}{T} \cdot \vec{\nabla} T
\]

et la dissipation intrinsèque \(D^i\):

\[
D^i = \rho \left(  \frac{\tenseur{S}^{e}}{\rho_i} - \frac{\partial \psi}{\partial \tepsilonel_{\mathrm{GL}}} \right)\,\colon\, \tdepsilonel_{\mathrm{GL}} - \rho \left( s + \frac{\partial \psi}{\partial T}\right) \dot T +\frac{\rho}{\rho_i} \tns{M}\,\colon\, \left( \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} \right)
\]

Considérons un processus réversible tel que \(\dot T = 0\),
\(\dot{\tns{F}_{p}} \inverse{\tns{F}_{p}}=0\) et \(\vec{\nabla} T =
0\). Pour le processus réversible, l'inégalité
\ref{eq:Clausius-Duhem_crystal} réduit à l'égalité, puisqu'il n'y a
pas de dissipation. Par conséquent, on obtient

\[
\tenseur{S}^e = \rho_i \frac{\partial \psi}{\partial \tepsilonel_{\mathrm{GL}}}
\]

De même, considérons un autre processus réversible tel que
\(\tdepsilonel_{\mathrm{GL}}=0\), \(\dot{\tns{F}_{p}}
\inverse{\tns{F}_{p}}=0\) et \(\vec{\nabla} T = 0\), on a

\[
s=-\frac{\partial \psi}{\partial T}
\]

Enfin, la dissipation intrinsèque se réduit à :

\[
D^i = \frac{\rho}{\rho_i} \tns{M}\,\colon\, \left( \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} \right)
\] {#eq:intrinsic_dissipation}

Dans l'équation [@eq:intrinsic_dissipation], \(\rho / \rho_i\)
vient du fait que l'inégalité \ref{eq:Clausius-Duhem} est écrite pour
une volume \(dV\) dans la configuration actuelle et que \(\tns{M} :
\left( \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} \right)\) est pour une
volume \(dV_i\) dans la configuration intermédiaire. En raison de la
conservation de masse, on a

\[
dV_i = \frac{\rho}{\rho_i} dV
\]

Dans le cadre de matériaux standards généralisés, un potentiel convexe
\(\Omega= \Omega\left( \tns{M} \right)\) existe et permet de définir
l'évolution de \(\dot{\tns{F}_{p}} \inverse{\tns{F}_{p}}\) :

\[
\dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} = \frac{\partial \Omega}{\partial \tns{M}}
\]

Selon les équations [@eq:power_internal_forces] et
[@eq:intrinsic_dissipation], le second tenseur de Piola-Kirchhoff
\(\tenseur{S}^e\) est utilisé pour la loi d'élasticité et le tenseur
de Mandel \(\tns{M}\) pour la loi d'écoulement. Vu que la dissipation
décrite par \(\tns{M}\,\colon\, \left( \dot{\tns{F}_{p}}
\inverse{\tns{F}_{p}} \right)\) est liée au glissement des
dislocations dans le monocristal, on a

\[
\tns{M}\,\colon\, \left( \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} \right) = \sum_{s=1}^{12} \dot \gamma^s \tau^s
\] {#eq:plastic_power_1}

où \(\tau^s\) est la cission résolue du système de glissement
\(s\). Compte tenu l'équation [@eq:PP], on a

\[
\tns{M}\,\colon\, \left( \dot{\tns{F}_{p}} \inverse{\tns{F}_{p}} \right) = \tns{M}\,\colon\, \left(\sum_{s=1}^{12} \dot \gamma^s \tns{N}^s \right) = \sum_{s=1}^{12} \dot \gamma^s \tns{M}\,\colon\, \tns{N}^s
\] {#eq:plastic_power_2}

Si on compare l'équation [@eq:plastic_power_1] et l'équation
[@eq:plastic_power_2], la cission résolue \(\tau^s\) du système de
glissement \(s\) s'écrit donc :

\[
\tau^s = \tns{M}\,\colon\, \tns{N}^s
\]


# The `FiniteStrainSingleCrystal` brick

The previous framework allows the implementation of a wide range of
finite strain single crystals that differ from their flow rules and
the constitutive equations used to describe their internal state
variables' evolution.

The `FiniteStrainSingleCrystal` brick hides all the details related to
this framework and simplifies the declaration of the slip systems and
the interaction matrix.

## Variables automatically defined

The plastic slip \(g\) are automatically defined.

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
\(\tns{F}_{e}\) en fin de pas de temps en fin de pas de
temps\textasciitilde{}: \[
\tns{F}_{e}=\tns{F}_{e}^{\mathrm{tr}}\,\star\,\paren{\Delta\,\tns{F}_{p}}^{-1}
\] où \(\star\) représente le produit des représentations matricielles de
\(\tns{F}_{e}^{\mathrm{tr}}\) et \(\paren{\Delta\,\tns{F}_{p}}^{-1}\).

L'équation associée à l'incrément de déformation élastique impose
qu'elle soit égale au tenseur de Green-Lagrange
\(\tepsilonel_{\mathrm{GL}}\) associé à la partie élastique
\(\tns{F}_{e}\) du gradient de la transformation\textasciitilde{}:
\[
\bts{\tepsilonel}+\Delta\,\bts{\tepsilonel}-
\tepsilonel_{\mathrm{GL}}\paren{\tns{F}_{e}}=0
\]

Le calcul des termes \(\deriv{f_{\tepsilonel}}{\Delta\,g_{i}}\) est assez
technique et devra se faire en plusieurs étapes\textasciitilde{}: \[
\deriv{\tenseur{\varepsilon_{\mathrm{GL}}}}{\Delta\,g_{i}}=
\Frac{1}{2}\,\deriv{\tenseur{C}_{e}}{\Delta\,g_{i}}=
\Frac{1}{2}\,\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}\,.\,
\deriv{\tns{F}_{e}}{\Delta\,g_{i}}
\]

La classe `t2tost2` fournit une méthode statique permettant de calculer la
dérivée du tenseur de Cauchy élastique \(\tenseur{C}_{e}\) par rapport au
gradient de la transformation élastique \(F_{e}\).

Il faut alors calculer le terme \(\deriv{\tns{F}_{e}}{\Delta\,g_{i}}\). Le
tenseur \(\tns{F}_{e}\) est le produit \(\star\) des représentation
matricielle de deux tenseurs\textasciitilde{}:
\(\tns{F}_{e}^{\mathrm{tr}}\) et le tenseur
\(\paren{\Delta\,\tns{F}_{p}}^{-1}\).

Cette démarche est maintenant appliquée au calcul de la dérivée du
tenseur \(\tns{F}_{e}\)\textasciitilde{}: \[
\deriv{\tns{F}_{e}}{\paren{\Delta\,\tns{F}_{p}}^{-1}} =
\partial^{r}_{\star}\paren{\tns{F}_{e}^{\mathrm{tr}}}
\]

En négligeant la correction plastique, la dérivée
\(\deriv{\paren{\Delta\,\tns{F}_{p}}^{-1}}{\Delta\,g_{i}}\) s'écrit
simplement\textasciitilde{}: \[
\deriv{\paren{\Delta\,\tns{F}_{p}}^{-1}}{\Delta\,g_{i}} = -\tns{\mu}_{i}
\]

Nous obtenons finalement l'expression la dérivée
\(\deriv{f}{\Delta\,g_{i}}\)\textasciitilde{}: \[
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

La contrainte de Kirchhoff \(\tenseur{\tau}\) se calcule par le transport
dans la configuration finale du second tenseur de Piola-Kirchhoff
calculé dans la configuration intermédiaire. \[
\tenseur{\tau} = \tns{F}_{e}\,\star\,\tenseur{S}\,\star\,\transpose{\tns{F}_{e}}
\] où \(\star\) représente le produit matriciel.

TFEL propose la méthode computePushForwardDerivative permettant de
relier la dérivée \(\deriv{\tenseur{\tau}}{\tns{F}_{e}}\) à la dérivée du
second tenseur de Piola-Kirchhoff \(\deriv{\tenseur{S}}{\tns{F}_{e}}\).
Cette dérivée se calcule par dérivation en chaînes\textsubscript{: \[
\deriv{\tenseur{S}}{\tns{F}_{e}}=\deriv{\tenseur{S}}{\tepsilonel_{\mathrm{GL}}}\deriv{\tepsilonel_{\mathrm{GL}}}{\tenseur{C}_{e}}\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}
\] soit}: \[
\deriv{\tenseur{S}}{\tns{F}_{e}}=\Frac{1}{2}\,D\,\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}
\]

La dérivée \(\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}\) se calcule par la
méthode statique dCdF de la classe t2tost2.

Il nous fait maintenant calculer le terme
\(\deriv{\tns{F}_{e}}{\Delta\,\tns{F}}\).

Nous savons que\textasciitilde{}: \[
\tns{F}_{e}=\tns{F}_{e}^{\mathrm{tr}}\,\star\,\paren{\Delta\,\tns{F}_{p}}^{-1}
\] Par dérivation, nous obtenons\textasciitilde{}: \[
\deriv{\tns{F}_{e}}{\Delta\,\tns{F}}=
\partial^{l}_{\star}\paren{\paren{\Delta\,\tns{F}_{p}}^{-1}}\,\deriv{\tns{F}_{e}^{\mathrm{tr}}}{\Delta\,\tns{F}}+
\partial^{r}_{\star}\paren{\tns{F}_{e}^{\mathrm{tr}}}\,\deriv{\paren{\Delta\,\tns{F}_{p}}^{-1}}{\Delta\,\tns{F}}
\]

Le calcul du terme \(\deriv{\tns{F}_{e}^{\mathrm{tr}}}{\Delta\,\tns{F}}\)
est aisé\textasciitilde{}: \[
\deriv{\tns{F}_{e}^{\mathrm{tr}}}{\Delta\,\tns{F}}=
\partial^{l}_{\star}\paren{\bts{\tns{F}_e}}
\]

Pour le calcul de la dérivée
\(\deriv{\paren{\Delta\,\tns{F}_{p}}^{-1}}{\Delta\,\tns{F}}\), nous
pouvons nous baser sur l'approximation faite lors de l'intégration. Nous
avons alors directement\textasciitilde{}: \[
\deriv{\paren{\Delta\,\tns{F}_{p}}^{-1}}{\Delta\,\tns{F}}=
-\sum_{i=1}^{12}\tns{\mu}_{i}\,\otimes\,\deriv{\Delta\,g_{i}}{\Delta\,\tns{F}}
\]

Le calcul de \(\deriv{\Delta\,g_{i}}{\Delta\,\tns{F}}\) représente la
seule vraie difficulté. Son calcul peut se faire en généralisant les
techniques utilisées en petites déformations.

Pour toute variation de \(\delta\,\Delta\,\tns{F}\) de \(\Delta\,\tns{F}\),
la variation du système implicite \(\delta\,F\) est
nulle\textasciitilde{}: \[
\delta\,F\paren{\Delta\,Y\paren{\delta\,\Delta\,\tns{F}},\delta\,\Delta\,\tns{F}}
=J\,\delta\,\Delta\,Y+
\left.\deriv{F}{\Delta\,\tns{F}}\right|_{\paren{\Delta\,\tepsilonel,\Delta\,g_{i}}}\,\delta\,\Delta\,\tns{F}=0
\]

Nous avons explicitement spécifié, en utilisant la notation classique de
la thermodynamique, que le calcul du terme
\(\left.\deriv{F}{\Delta\,\tns{F}}\right|_{\paren{\Delta\,\tepsilonel,\Delta\,g_{i}}}\)
se faisait en supposant que \(\Delta\,\tepsilonel\) et \(\Delta\,g_{i}\)
étaient des variables indépendantes.

Il est donc nécessaire de calculer la dérivée
\(\left.\deriv{F}{\Delta\,\tns{F}}\right|_{\paren{\Delta\,\tepsilonel,\Delta\,g_{i}}}\)
dont la seule composante non nulle est
\(\left.\deriv{f_{\tepsilonel}}{\Delta\,\tns{F}}\right|_{\paren{\Delta\,\tepsilonel,\Delta\,g_{i}}}\)\textasciitilde{}:
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

Finalement, \(\deriv{F}{\Delta\,\tns{F}}\) s'écrit\textasciitilde{}: \[
\deriv{f_{\tepsilonel}}{\Delta\,\tns{F}}=\deriv{\tenseur{C}_{e}}{\tns{F}_{e}}\,.\,
\partial^{l}_{\star}\paren{\paren{\Delta\,\tns{F}_{p}}^{-1}}\,.\,
\partial^{l}_{\star}\paren{\bts{\tns{F}_{e}}}
\]

La dérivée \(\deriv{\Delta\,g_{i}}{\Delta\,\tns{F}}\) est
alors\textasciitilde{}: \[
\deriv{\Delta\,g_{i}}{\Delta\,\tns{F}}=-J^{-1}_{\Delta\,g_{i}}|\deriv{f_{\tepsilonel}}{\Delta\,\tns{F}}
\]

## Code blocks automatically declared

The `ComputeFinalStress` code block is automatically declared.

The `TangentOperator` code block is automatically declared.

## Variables automatically computed

The residual \(f_{\tepsilonel}\) associated with the elastic strain
\(\tepsilonel\) is automatically implemented.

The Mandel stress \(\tns{M}\) is automatically computed before the
integrator code block. Its value is stored in the variables `M`.  If
the algorithm requires an analytic jacobian, the derivative
\(\deriv{\tns{M}}{\Delta\,\tepsilonel}\) is also computed and stored
in the variable `dM_ddeel` respectively.

# References

<!-- Local IspellDict: english -->
