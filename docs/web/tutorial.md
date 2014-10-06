% Introduction à MFront 
% Helfer Thomas; Jean-Michel Proix
% September 16, 2014

Cette page est extraitre du tutoriel MFront : [tutoriel.pdf](documents/tutoriel/tutoriel.pdf).

\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\tns}[1]{\underset{\tilde{}}{\mathbf{#1}}}
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

# Comment écrire une loi de comportement: la loi de comportement de Norton

En guise de préliminaire, prenons par exemple la loi de . Elle est
définie par: $$\left\{
  \begin{aligned}
    \tepsilonto   &= \tepsilonel+\tepsilonvis \\
    \tsigma       &= \tenseurq{D}\,:\,\tepsilonel\\
    \tdepsilonvis &= \dot{p}\,\tenseur{n} \\
    \dot{p}       &= A\,\sigmaeq^{m}
  \end{aligned}
\right.$$ où:

- $\tepsilonto$, $\tepsilonel$, $\tepsilonvis$ représentant
  respectivement les déformations totale, élastique et visqueuse ;
- $\tenseur{n}=\Frac{3}{2}\,\Frac{\tenseur{s}}{\sigmaeq}$ est la
  direction d’écoulement ;
- $\tenseur{s}$ est le déviateur des contraintes ;
- $\sigmaeq$ est la norme de .

L’opérateur d’élasticité $\tenseurq{D}$ est calculé à partir du module
d’ $E$ et du coefficient de $\nu$.

## Discrétisation implicite de loi de Norton

Pour intégrer cette loi dans un calcul de structure, il faut procéder à
une discrétisation en temps, ce qui revient à définir une suite
d’instants de calcul $\left\{t_{i}\right\}_{1\le i\le I}$.

Pour utiliser un algorithme implicite, il suffit d’écrire toutes les
quantités à l’instant $t_{i}$ et de remplacer les dérivées en temps par
leurs incréments sur l’intervalle $ \Delta t= t_{i} - t_{i-1} $ :
$$\left\{
  \begin{aligned}
    \Delta\,\tepsilonel - \Delta\,\tepsilonto   + \Delta\,p\,\tenseur{n} = 0 \\
    \Delta\,p  - \Delta\,t\, A\,\sigmaeq^{m} = 0 
  \end{aligned}
\right.$$

avec:

-   $\tsigma  = \tenseurq{D}\,:\,\tepsilonel $ ;
-   $\tenseur{n}=\Frac{3}{2}\,\Frac{ \tenseur{s} \paren{t_{i}}} { \sigmaeq \paren{ t_{i}}}\ $

On obtient ainsi un système de 7 équations (6 équations — en 3D —
relatives à la décomposition additive du tenseur des déformations, et
une équation relative à l’écoulement visco-plastique). Les \(7\)
inconnues sont les \(6\) composantes de $\Delta\,\tepsilonel$ (en
\(3D\)) et $\Delta p $.

La résolution implicite de ce système est effectuée par une méthode de .

## Première implantation

Voici un exemple très simple d’intégration implicite de ce modèle avec :

~~~~ {#UO2YoungModulus .cpp .numberLines}
@Parser Implicit;
@Behaviour Norton;
@Algorithm NewtonRaphson_NumericalJacobian ;

@RequireStiffnessTensor;

@MaterialProperty real A;
@MaterialProperty real m;

@StateVariable real p ;

@ComputeStress{
  sig = D*eel ;
}

@Integrator{
  real seq = sigmaeq(sig) ;
  Stensor n = Stensor(0.) ;
  if(seq > 1.e-12){
    n = 1.5*deviator(sig)/seq ;
  }
  feel += dp*n-deto ;
  fp -= dt*A*pow(seq,m) ;
} // end of @Integrator

@TangentOperator{
  Stensor4 Je ;
  getPartialJacobianInvert(Je) ;
  Dt = D*Je ;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Description ligne par ligne

Un fichier commence généralement par une partie déclarative décrivant
l’algorithme utilisé pour la résolution, le nom du comportement (ici
),puis la liste des propriétés matériau utilisées. On fournit ensuite le
nom des variables internes, et la description des équations du système à
résoudre.

-   la ligne $1$ précise que nous allons utiliser une méthode
    d’intégration implicite ;
-   la ligne $2$ donne le nom de la loi ;
-   la ligne $3$ précise que l’on utilise un algorithme de avec
    jacobienne numérique ;
-   la ligne $5$ demande le calcul de la matrice d’élasticité  ;
-   les lignes $7$ et $8$ définissent les propriétés matériau de la
	loi ;
-   la ligne $10$ déclare la variable interne $p$. Signalons que la
    variable interne `eel` (tenseur déformation élastique
    $\tepsilonel$) est prédéfinie par MFront;
-   les lignes $16$ à $24$ définissent les équations à résoudre ; la
    convention de nom est la suivante : pour chaque variale interne `x`,
    l’incrément est noté `dx`, et l’équation correspondante `fx` ;
-   la ligne $17$ demande le calcul de la norme de Von Mises ;
-   les lignes $18$ à $21$ correspondent au calcul de la direction
    d’écoulement ;
-   la ligne $22$ définit l’équation $\Delta\,\tepsilonel -
      \Delta\,\tepsilonto + \Delta\,p\,\tenseur{n} = 0$. En effet,
    `feel` est initialisé à `deel` et l’opérateur `+=` cumule les
    valeurs (comme en langage `c`);
-   la ligne $22$ définit l’équation $\Delta\, p -
      \Delta\,t\,A\,\sigmaeq^{m}=0$. Comme précédemment, `fp`
    est initialisé à dp, et `fp -= xx` est équivalent à
    `fp = fp - xx` .
-   les lignes $26$ à $29$ permettent de calculer automatiquement
    l’opérateur tangent à partir de l’inverse de la matrice jacobienne,
    comme explicité ci-dessous.

On constate que l’écriture de la loi se limite quasiment à la
description des équations. De plus on bénéficie d’un écriture compacte,
utilisant des variables tensorielles.

Différentes méthodes d’intégration sont diponibles dans MFront (
@helfer_generateur_2013). L’algorithme d’intégration utilisé ici
(`NewtonRaphson\_NumericalJacobian`) permet une écriture rapide, et
est donc tout à fait adapté au test de modèles.

L’implantation fournit un code beaucoup plus rapide que celle d’un
algorithme explicite, mais peut toutefois être optimisée en termes de
performances. Pour cela, il suffit d’ajouter les termes de la matrice
jacobienne (dérivées des équations par rapport aux inconnues).

De plus, la matrice tangente en sortie de l’intégration est calculée
automatiquement à partir de la matrice jacobienne, ce qui permet
d’économiser un temps de développement important et conduit à une
matrice tangente cohérente de très bonne qualité
(@helfer_generateur_2013). Tout ceci conduit, en très peu de temps, à
une intégration robuste, et une convergence très bonne. On voit qu’il
est possible de profiter de cette simplicité d’écriture pour effectuer
des variantes, des tests de modèles, etc.

MFront gère la compilation de la loi, il suffit de taper dans un
terminal :

~~~~ {.bash}
$ mfront --obuild -interface=aster norton.mfront
~~~~~~~~~~~~~~~~~~~

ou

~~~~ {.bash}
$ mfront --obuild -interface=umat norton.mfront
~~~~~~~~~~~~~~~~~~~

suivant le code que l’on souhaite utiliser. Ceci génère deux
répertoires : `src` et `include`. Selon l’interface, le répertoire
`src` contient en particulier une bibliothèque dynamique nommée
`libAsterBehaviour.so` pour une exécution avec Code-Aster, ou bien
`libUmatBehaviour.so` pour une exécution avec Cast3M.

## Premier test

De plus, l’outil MTest permet d’effectuer très facilement des
simulations sur point matériel, permettant de calculer la réponse à des
sollicitations en contraintes ou en déformations.

MTest, couplé à un logiciel d’optimisation comme , permet de plus
d’effectuer le recalage des propriétés matériau. Le fichier de données
de MTest (nommé ici `norton.mtest`) se présente de la façon suivante :

~~~~ {#UO2YoungModulus .cpp .numberLines}
@Behaviour<aster> './src/libAsterBehaviour.so' 'asternorton' ;
@MaterialProperty<constant> 'young' 178600.0E6 ;
@MaterialProperty<constant> 'nu' 0.3 ;
@MaterialProperty<constant> 'A' 8.e-67 ;
@MaterialProperty<constant> 'm' 8.2 ;
@ExternalStateVariable 'Temperature' 293.15 ;
@ImposedStress 'SXX' { 0. :  0., 30. : 40.e6};
@ImposedStress 'SXY' { 0. :  0., 30. : 40.e6};
@Times {0.,30. in 100};
~~~~~~~~~~~~~~

-   la ligne $1$ définit la bibliothèque à utiliser et le nom du
    comportement ;
-   les lignes $2$ à $5$ définissent les valeurs des propriétés matériau
    de la loi ;
-   la ligne $6$ déclare la valeur de la température (inutile dans le
    cas présent) ;
-   les lignes $7$ à $8$ spécifient le chargement, sous la forme de
    composantes de contraintes ou de déformations en fonction du temps ;
-   la ligne $9$ définit la discrétisation en temps, donc les instants
    calculés.

Il suffit alors de lancer le calcul par :

~~~~ {.bash}
mtest norton.mtest
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ceci produit un fichier résultat `norton.res ` contenant les
composantes des tenseurs de déformation, de contrainte, et les
variables internes en fonction du temps. La réponse en déformation est
représentée ci-dessous:

![Réponse d’une loi de Norton à un essai de fluage en
traction-cisaillement](img/tutorielch12-img1.png)

# Références
