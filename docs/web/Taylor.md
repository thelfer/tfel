% Implémentation du schéma d'homogénéisation de Taylor pour des comportements non-linéaires 
% Helfer Thomas/Martin Antoine
% February 6, 2025

\newcommand{\D}{\mathrm{d}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\tns}[1]{\underset{\tilde{}}{\mathbf{#1}}}
\newcommand{\transpose}[1]{#1^{\mathop{T}}}

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

# Le schéma d'homogénéisation de Taylor

Le schéma de Taylor/Voigt fait l'hypothèse d'une déformation uniforme $\tenseur E$
dans toutes les phases. La contrainte effective $\tenseur \Sigma$, définie comme la moyenne des contraintes
dans le volume élémentaire représentatif (VER), est ainsi égale à:
  \begin{aligned}
    \tenseur \Sigma   &= \sum_{i=1}^{N}f_i\,\tsigma^i (\tenseur E) \\
  \end{aligned}
où $N$ est le nombre de phases, $f_i$ est la fraction volumique de la phase $i$ et $\tsigma^i$ est la contrainte dans la phase $i$ (donnée par une loi de comportement non-linéaire).
Par ailleurs, l'opérateur tangent macroscopique est égal à:
  \begin{aligned}
    \dfrac{\D \tenseur \Sigma}{\D \tenseur E}   &= \sum_{i=1}^{N}f_i\,\dfrac{\D \tsigma^i}{\D \varepsilon^i}(\tenseur E) \\
  \end{aligned}

# Implémentation dans MFront

Il est possible d'implémenter ce schéma d'homogénéisation à travers un `Behaviour`.
Ce `Behaviour` doit faire appel aux lois de comportement de chaque phase.
Ainsi, une solution intéressante consiste à utiliser une `BehaviourVariable`.

## Création des lois de comportement sur chaque phase avec `@BehaviourVariable`

La première étape consiste à créer des variables qui permettront de calculer, sur chaque phase
la contrainte locale en fonction de la déformation locale, et également l'opérateur tangent local en fonction
de la déformation locale. En somme, il s'agit de pouvoir appeler facilement un `Behaviour` local dans le `Behaviour`
macroscopique. Ces `Behaviour` doivent être implémentés au préalable dans des fichiers `.mfront`
auxiliaires.

Pour l'exemple, et pour simplifier, on supposera que l'on a 2 phases dont le comportement
est élastoplastique avec une critère de Von Mises à écrouissage isotrope linéaire. Les modules d'écrouissage et limites
d'élasticité sont différents entre les phases. Ce comportement est implémenté dans le fichier
`Plasticity.mfront`, avec comme paramètres, la limite d'élasticité
et le module d'écrouissage.

~~~~ {#Plasticity .cpp .numberLines}
@DSL IsotropicPlasticMisesFlow;
@Behaviour Plasticity;
@Epsilon 1e-14;
@UseQt true;

@MaterialProperty stress H;
@MaterialProperty stress s0;

@FlowRule {
  f = seq - H * p - s0;
  df_dseq = 1;
  df_dp = -H;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ensuite, dans le fichier `Taylor.mfront`,
on travaillera avec le `DSL` `Default` :

~~~~ {#Taylor .cpp .numberLines}
@DSL Default;
@Behaviour Taylor;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La création de `BehaviourVariable` se fait de la manière suivante :

~~~~ {#Taylor .cpp .numberLines}
@Includes{
#include "MFront/GenericBehaviour/BehaviourData.hxx"
}

@BehaviourVariable b1 {
file: "Plasticity.mfront",
variables_suffix: "1",
external_names_prefix: "FirstPhase",
shared_external_state_variables: {".+"}
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


- La ligne $2$ permet d'utiliser le mot clé `@BehaviourVariable`.
- le bloc `@BehaviourVariable` crée
une variable nommée `b1`, représentant notre loi de comportement sur la phase $1$.
- le fichier implémentant la loi de comportement de la phase $1$ est précisé par la ligne $6$.
- en ce qui concerne les variables de la loi de comportement, celles-ci
seront reprises avec le nom utilisé par le `Behaviour` implémenté,
en ajoutant un suffixe, ici $1$ pour la phase $1$. On pourra par exemple utiliser
`eto1`, la déformation totale de la phase $1$, ou encore `sig1` pour la
contrainte, et c'est la même chose pour les autres variables, par exemple
`eel1` ou `p1` pour la déformation plastique cumulée.
- le nom des variables à l'extérieur, c'est-à-dire par exemple dans
le code aux éléments finis appelant `MFront`, se verra ajouté le préfixe
`FirstPhase`, comme précisé par la ligne $8$.

Pour plus de détails sur l'utilisation de `@BehaviourVariable`,
on se reportera à la [documentation de BehaviourVariable](behaviour-variable.html)

Bien sûr, il faudra créer autant de `@BehaviourVariable`
qu'il y a de phases.

## Intégration des lois de comportement et implémentation du schéma de Taylor

Après avoir définis les `BehaviourVariable`, on peut
implémenter le bloc `@Integrator`. Avant ce bloc,
On précise que l'opérateur tangent sera calculé à l'intérieur du
bloc `@Integrator`, étant donné sa forme très simple,
dans le cas du schéma de Taylor. Ceci se fait via `@ProvidesTangentOperator`.

~~~~ {#Integrator .cpp .numberLines}
@ProvidesTangentOperator;

@Integrator{
  b1.deto=deto;
  
  constexpr auto b1_smflag = TangentOperatorTraits<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>::STANDARDTANGENTOPERATOR;
  const auto r1 = b1.integrate(b1_smflag,CONSISTENTTANGENTOPERATOR);
  StiffnessTensor Dt1 = b1.getTangentOperator();
 
  b2.deto=deto;
  
  constexpr auto b2_smflag = TangentOperatorTraits<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>::STANDARDTANGENTOPERATOR;
  const auto r2 = b2.integrate(b2_smflag,CONSISTENTTANGENTOPERATOR);
  StiffnessTensor Dt2 = b2.getTangentOperator();
  
  updateAuxiliaryStateVariables(b1);
  updateAuxiliaryStateVariables(b2);

  sig = f * sig1 + (1 - f) * sig2;
  if (computeTangentOperator_) {
    Dt = f * Dt1 + (1 - f) * Dt2;
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On peut voir que l'on commence par intégrer les lois de comportement
sur chaque phase.
Regardons la phase $1$ :

- la ligne $4$ précise l'incrément de déformation sur la phase $1$,
en l'occurrence identique à l'incrément de la déformation
macroscopique `deto`. Par ailleurs, `b1.eto` est automatiquement initialisé à `eto1`
avant l'intégration du comportement `b1`. Ceci est automatique, et correspond
à l'appel de `initialize(b1)` avant d'intégrer.
- l'intégration du comportement de la phase $1$ se fait via la ligne
$7$ avec la méthode `integrate` qui prend deux arguments (voir la documentation
de `@BehaviourVariable` pour plus de précisions. `r1` est un booléen qui
vaut $1$ si l'intégration s'est bien déroulée.
- on récupère l'opérateur tangent via la méthode `getTangentOperator`.

Une fois l'intégration des lois de comportement effectuée
sur chaque phase, il est nécessaire de mettre à jour les variables
auxiliaires associées à chaque `@BehaviourVariable`. Ceci est
réalisé aux lignes $16$ et $17$ avec la fonction `updateAuxiliaryStateVariables`.
Cela permet de mettre à jour `eto1` (resp. `sig1`) à la déformation (resp. contrainte)
obtenue en fin d'intégration
du comportement `b1`, et de même pour les autres variables
comme `eel1` ou `p1`. Il en est de même pour la phase 2.

Enfin, le calcul de la contrainte macroscopique est donné
par la loi de Taylor (moyenne des contraintes locales)
à la ligne $19$, et le calcul de l'opérateur tangent
est également donné par la loi de Taylor, aux lignes suivantes.

A noter que la propriété suivante avait été définie avant
le bloc d'intégration :

~~~~ {#fraction .cpp .numberLines}
@MaterialProperty real f;
f.setEntryName("FirstPhaseFraction");
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Résultats

On utilise ici MTest pour effectuer des
simulations sur point matériel, en simulant un test de traction
uniforme.

Le fichier MTest
(nommé ici `Taylor.mtest`) se présente de la façon suivante :

~~~~ {#Taylor_mtest .mtest .numberLines}
@ModellingHypothesis 'Tridimensional';
@Behaviour<Generic> 'src/libBehaviour.so' 'Taylor';
@MaterialProperty<constant> 'FirstPhaseYoungModulus'     150.e9;
@MaterialProperty<constant> 'FirstPhasePoissonRatio'       0.35;
@MaterialProperty<constant> 'H1'                50.e9;
@MaterialProperty<constant> 's01'               200.e6;
@MaterialProperty<constant> 'SecondPhaseYoungModulus'     90.e9;
@MaterialProperty<constant> 'SecondPhasePoissonRatio'       0.3;
@MaterialProperty<constant> 'H2'                30.e9;
@MaterialProperty<constant> 's02'               50.e6;
@MaterialProperty<constant> 'FirstPhaseFraction' 0.1;
@ExternalStateVariable 'Temperature' 293.15;
@ImposedStrain 'EXX' {0 : 0, 1 : 3e-3};
@Times {0, 1 in 200};
~~~~~~~~~~~~~~

La contrainte axiale macroscopique, ainsi que la contrainte axiale dans chaque phase sont
représentées en fonction de la déformation axiale macroscopique (uniforme dans le VER):

![Contraintes locales et macroscopique en fonction de la déformation macroscopique, traction uniaxiale, schéma de Taylor](./img/Taylor_test.png)

On peut voir, comme attendu, que la contrainte macroscopique
est une moyenne des contraintes dans les phases. Quand la limite
d'élasticité la plus petite est atteinte, la phase correspondante
plastifie. Le module tangent macroscopique s'en trouve diminué, ce
qui se répercute sur la contrainte macroscopique.
L'autre phase plastifie plus tard. Même si son évolution reste
élastique, on peut voir que sa contrainte axiale est également
impactée par la plastification de l'autre phase, à cause des déformations
non axiales plastiques apparues dans cette dernière.

<!-- Local IspellDict: english -->
