% Implémentation du schéma d'homogénéisation de Sachs pour des comportements non-linéaires 
% Helfer Thomas/Martin Antoine
% February 6, 2025

\newcommand{\D}{\mathrm{d}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\tns}[1]{\underset{\tilde{}}{\mathbf{#1}}}
\newcommand{\transpose}[1]{#1^{\mathop{T}}}

\newcommand{\tepsilon}{\tenseur{\varepsilon}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}

\newcommand{\tepsilonto}{\underline{\epsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\underline{\epsilon}^{\mathrm{el}}}

\newcommand{\tepsilonp}{\underline{\epsilon}^{\mathrm{p}}}
\newcommand{\tdepsilonp}{\underline{\dot{\epsilon}}^{\mathrm{p}}}

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

# Le schéma d'homogénéisation de Sachs

Le schéma de Sachs/Reuss fait l'hypothèse d'une contrainte uniforme $\tenseur \Sigma$
dans toutes les phases:
  \begin{aligned}
    \tsigma^i &= \tenseur \Sigma \\
  \end{aligned}
où $\tsigma^i$ est la contrainte dans la phase $i$.
La déformation effective $\tenseur E$ est définie comme la moyenne des déformations
dans le volume élémentaire représentatif (VER) :
  \begin{aligned}
    \tenseur E &= \sum_{i=1}^{N}f_i\,\tenseur \varepsilon^i \\
  \end{aligned}
où $N$ est le nombre de phases, $f_i$ est la fraction volumique de la phase $i$ et $\tenseur \varepsilon^i$ est la déformation dans la phase $i$ (donnée par une loi de comportement non-linéaire).
Par ailleurs, l'opérateur tangent macroscopique est égal à:
  \begin{aligned}
    \dfrac{\D \tenseur \Sigma}{\D \tenseur E}   &= \left(\deriv{\tenseur E}{\tenseur \Sigma}\right)^{-1}=\left(\sum_{i=1}^{N}f_i\,\deriv{\tepsilon^i}{\tenseur \Sigma}\right)^{-1}=\left(\sum_{i=1}^{N}f_i\,\left(\deriv{\tsigma^i}{\tepsilon^i}\right)^{-1}\right)^{-1}\\
  \end{aligned}
  
# Résolution du système non linéaire

On discrétise l'intervalle de temps considéré et on cherche à un instant donné les incréments des variables.
Le problème à résoudre est le suivant :

$\left\{
\begin{aligned}
&\Delta\tenseur \Sigma - \Delta\tsigma^i(\tepsilon^i+\theta\Delta\tepsilon^i) = 0 \qquad \forall i\\
&\Delta\tenseur E-\sum_{i=1}^{N}f_i\,\Delta\tepsilon^i = 0\\
  \end{aligned}\right.$

avec comme inconnues les incréments $\Delta\tepsilon^i$ et $\Delta \tenseur \Sigma$,
et $\theta$ peut être précisé par l'utilisateur.
On peut utiliser l'algorithme de Newton-Raphson pour résoudre ce sytème non-linéaire.
Dans MFront, le `DSL` `Implicit` permet de résoudre ce type d'équation
de manière simple. Il nous faut seulement préciser les résidus, ainsi que la jacobienne.
Les résidus sont

$\left\{
\begin{aligned}
&f_{\tepsilon^i} =\left(\Delta\tenseur \Sigma - \Delta\tsigma^i(\tepsilon^i+\theta\Delta\tepsilon^i)\right)/\sigma_{0} \qquad \forall i\\
&f_{\tenseur E} = \Delta\tenseur E-\sum_{i=1}^{N}f_i\,\Delta\tepsilon^i
  \end{aligned}\right.$
  
 où $\sigma_{0}$ est un paramètre qui permet de rendre homogènes en dimension et en ordre de grandeur les résidus. La jacobienne est donnée par:
  
 $\left\{
\begin{aligned}
&\deriv{f_{\tepsilon^i}}{\Delta\tepsilon^j} =- \delta_{ij}\frac{\theta}{\sigma_0}\deriv{\tsigma^j}{\tepsilon^j}(\tepsilon^j+\theta\Delta\tepsilon^j) \qquad \forall i,j\\
&\deriv{f_{\tepsilon^i}}{\Delta\tenseur \Sigma} = \tenseurq{I}/\sigma_{0} \qquad \forall i\\
&\deriv{f_{\tenseur E}}{\Delta\tepsilon^i} = -f_i\,\tenseurq{I} \qquad \forall i\\
&\deriv{f_{\tenseur E}}{\Delta\tenseur \Sigma} = \tenseurq{0}
  \end{aligned}\right.$

# Implémentation dans MFront

Comme pour le schéma de Taylor, nous allons utiliser les `BehaviourVariable`
pour l'intégration des comportements locaux.
L'intégration du comportement global, qui nécessite de résoudre une équation
non-linéaire, se fera avec le `DSL` `Implicit`.

L'étape de création des variables qui permettent d'intégrer le comportement sur chaque phase
se fait de la même manière que ce qui a été décrit pour le [schéma de Taylor](Taylor.html).
Les `Behaviour` doivent être implémentés au préalable dans des fichiers `.mfront`
auxiliaires.

Pour l'exemple, et pour simplifier, on supposera que l'on a 2 phases dont le comportement
est élastoplastique avec un critère de Von Mises à écrouissage isotrope linéaire. Les modules d'écrouissage et limites
d'élasticité sont différents entre les phases et le comportement est implémenté dans un fichier
`Plasticity.mfront`.

Dans le fichier `Sachs.mfront`,
on travaillera avec le `DSL` `Implicit`, avec un algorithme de Newton-Raphson :

~~~~ {#Sachs .cpp .numberLines}
@DSL Implicit;
@Behaviour Sachs;

@Algorithm NewtonRaphson;
@Epsilon 1.e-14;
@Theta 1;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On rappelle la syntaxe pour créer une `BehaviourVariable` :

~~~~ {#Sachs .cpp .numberLines}
@Includes{
#include "MFront/GenericBehaviour/BehaviourData.hxx"
}

@BehaviourVariable b1 {
file: "Plasticity.mfront",
variables_suffix: "1",
store_gradients: false,
external_names_prefix: "FirstPhase",
shared_external_state_variables: {".+"}
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A noter ici, on rajoute la ligne `store_gradients: false,` car on veut 
définir la variable `eto1` comme `StateVariable`. Dans le cas du schéma de Taylor,
l'option `store_gradients` était égale à `true`, ce qui avait le même
effet que d'écrire `@AuxiliaryStateVariable StrainStensor eto1;` ce que nous ne voulons pas ici.
Il en est de même pour la phase 2.

Les `StateVariable` sont l'incrément de contrainte macroscopique d'une part et les incréments de déformations locales d'autre part :

~~~~ {#State .cpp .numberLines}
@StateVariable StrainStensor eto2;
eto2.setEntryName("SecondPhaseTotalStrain");
@StateVariable StrainStensor eto1;
eto1.setEntryName("FirstPhaseTotalStrain");
@StateVariable StressStensor Sig;
Sig.setEntryName("MacroscopicStress");
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ayant besoin des inverses des opérateurs tangents de chaque phase
pour le calcul de l'opérateur tangent macroscopique,
on définit des variables locales :

~~~~ {#Local .cpp .numberLines}
@LocalVariable StiffnessTensor iDt1;
@LocalVariable StiffnessTensor iDt2;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'implémentation du bloc `@Integrator` est la suivante: 

~~~~ {#Integrator .cpp .numberLines}
@Integrator{
  initialize(b1);
  b1.deto=deto1;
  constexpr auto b1_smflag = TangentOperatorTraits<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>::STANDARDTANGENTOPERATOR;
  const auto r1 = b1.integrate(b1_smflag,CONSISTENTTANGENTOPERATOR);
  StiffnessTensor Dt1 = b1.getTangentOperator();
  iDt1= invert(Dt1);
  
  initialize(b2);
  b2.deto=deto2;
  constexpr auto b2_smflag = TangentOperatorTraits<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>::STANDARDTANGENTOPERATOR;
  const auto r2 = b2.integrate(b2_smflag,CONSISTENTTANGENTOPERATOR);
  StiffnessTensor Dt2 = b2.getTangentOperator();
  iDt2= invert(Dt2);
  
  auto dsig1 =b1.sig-sig1;
  auto dsig2 =b2.sig-sig2;
  
  feto1=(dSig-dsig1)/sig_0;
  feto2=(dSig-dsig2)/sig_0;
  fSig=deto-(1-f)*deto1-f*deto2;
  auto Id=st2tost2<3u,real>::Id();
  auto Null= {0,0,0,0,0,0,
  	      0,0,0,0,0,0,
    	      0,0,0,0,0,0,
  	      0,0,0,0,0,0,
  	      0,0,0,0,0,0,
       	      0,0,0,0,0,0};
  dfeto1_ddeto1 = -theta/sig_0*Dt1;
  dfeto1_ddeto2 = Null;
  dfeto1_ddSig = Id/sig_0;
  dfeto2_ddeto1 = Null;
  dfeto2_ddeto2 = -theta/sig_0*Dt2;
  dfeto2_ddSig = Id/sig_0;
  dfSig_ddeto1 = -(1-f)*Id;
  dfSig_ddeto2 = -f*Id;
  dfSig_ddSig = Null;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On peut voir que l'on commence par l'intégration des lois de comportement
sur chaque phase. Cependant, par rapport au schéma de Taylor,
on rajoute `initialize(b1)` et `initialize(b2)` avant l'intégration. En effet, ceci permet
d'initialiser `b1.eto` à `eto1` et `b2.eto` à `eto2`. Ceci était automatique
dans le cas du schéma de Taylor, mais ici, comme l'option `store_gradients`
est `false` pour les deux phases, ce n'est plus automatique.
Par ailleurs, le gradient `b1.deto` est initialisé à `deto1`, qui est l'incrément
de la `StateVariable` `eto1` (celui-ci sera mis à jour à chaque itération du Newton-Raphson).

On récupère au passage l'opérateur tangent sur chaque phase, et on sauvegarde
son inverse.

Ensuite, on indique les résidus et la jacobienne.
Pour définir les résidus `feto1` et `feto2`, on a besoin des
incréments de contrainte `dsig1` et `dsig2`, que l'on obtient
grâce aux contraintes locales en fin d'intégration, `b1.sig`
et `b2.sig`. Les contraintes `sig1` et `sig2` sont les valeurs
des contraintes en début d'intégration. Elles sont égales
aux contraintes en fin de pas de temps précédent. En effet,
le bloc `@UpdateAuxiliaryStateVariables` est appelé en fin de pas
de temps et `sig1` est automatiquement mis à jour à `b1.sig`,
et de même `sig2` est mis à jour à `b2.sig`.

Attention : si l'instruction `updateAuxiliaryStateVariables(b1);`
est donnée dans le bloc `@Integrator`, alors la mise à jour de
la contrainte `sig1` aura lieu non pas à la fin du pas de temps,
mais à la fin de chaque itération du Newton-Raphson, ce que nous ne voulons
pas ici.
 
A noter que les paramètres suivants avaient été définis avant
le bloc d'intégration :

~~~~ {#fraction .cpp .numberLines}
@Parameter stress sig_0=1e11;

@MaterialProperty real f;
f.setEntryName("FirstPhaseFraction");
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Enfin, la contrainte macroscopique est donnée par le bloc
suivant :

~~~~ {#FinalStress .cpp .numberLines}
@ComputeFinalStress{
sig = Sig;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

et l'opérateur tangent est implémenté dans le bloc
suivant :

~~~~ {#TangentOperator .cpp .numberLines}
@TangentOperator{
  Dt = invert(f*iDt1+(1-f)*iDt2);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

qui utilise la formule du schéma de Sachs exposé plus haut.

# Résultats

On utilise ici MTest pour effectuer des
simulations sur point matériel, en simulant un test de traction
uniforme.

Le fichier MTest
(nommé ici `Sachs.mtest`) se présente de la façon suivante :

~~~~ {#Sachs_mtest .mtest .numberLines}
@ModellingHypothesis 'Tridimensional';
@Behaviour<Generic> 'src/libBehaviour.so' 'Sachs';
@MaterialProperty<constant> 'FirstPhaseYoungModulus'     60.e9;
@MaterialProperty<constant> 'FirstPhasePoissonRatio'       0.3;
@MaterialProperty<constant> 'H1'                4.e9;
@MaterialProperty<constant> 's01'               60.e6;
@MaterialProperty<constant> 'SecondPhaseYoungModulus'     50.e9;
@MaterialProperty<constant> 'SecondPhasePoissonRatio'       0.3;
@MaterialProperty<constant> 'H2'                2.e9;
@MaterialProperty<constant> 's02'               50.e6;
@MaterialProperty<constant> 'PhaseFraction' 0.5;
@ExternalStateVariable 'Temperature' 293.15;
@ImposedStrain 'EXX' {0 : 0, 1 : 5e-3};
@Times {0.,1 in 50};
~~~~~~~~~~~~~~

La contrainte axiale (uniforme dans le VER) est représentée ci-dessous
en fonction de la déformation axiale macroscopique d'une part, en fonction
de la déformation de la phase 1 et en fonction de la déformation de la phase
2.

![Contrainte macroscopique en fonction des déformations locales et macroscopique, traction uniaxiale, schéma de Sachs](./img/Sachs_test.png)

On peut voir, comme attendu, que si l'on se place à un état de contrainte
donnée, la déformation macroscopique est une moyenne
des déformations locales.
Quand la contrainte macroscopique
atteint la limite d'élasticité d'une des phases, la phase en question plastifie,
et cela se répercute sur la déformation macroscopique.
La plastification de la seconde phase apparaît ensuite.


<!-- Local IspellDict: english -->
