% Utilisation de MFront dans [Cast3M](http://www-cast3m.cea.fr/)
% Helfer Thomas
% August 19, 2014

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

`MFront` est un générateur de code open-source développé dans le cadre
du projet
[`PLEIADES`](http://www-cadarache.cea.fr/fr/actualite/video/video-pleiades.php)
qui permet notamment l'écriture de lois de comportements
mécaniques. Son but est de permettre l'écriture simple et
numériquement efficace de nouvelles lois de comportements mécaniques.

Plus d'informations sont disponibles sur le
[site internet de MFront](http://tfel.sourceforge.net).  Des
[instructions d'installation](http://tfel.sourceforge.net/install.html)
détaillées y sont notamment décrites.

Son utilisation dans `Cast3M` se fait via l'interface générique
[`UMAT`](http://www-cast3m.cea.fr/index.php?page=sources&source=umat).

Dans la suite de ce document, nous supposons travailler dans un
environnement LiNuX. Une page dédiée à l'utilisation de MFront sous
Windows sera ajoutée à terme.

Ce document se découpe en trois partie :

- création de la loi
- intégration dans `Cast3M`
- test sur un essai de traction uniaxial

L'ensemble des fichiers utilisés dans ce document peut être téléchargé
[ici](downloads/integration-castem2014.tar.bz2).

# Création d'une première loi

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
\(3D\)) et $\Delta p$.

La résolution implicite de ce système est effectuée par une méthode de .

## Première implantation

Voici un exemple très simple d’intégration implicite de ce modèle avec :

~~~~ {#Norton .cpp .numberLines}
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

## Compilation de la loi

MFront gère la compilation de la loi, il suffit de taper dans un
terminal :

~~~~ {.bash}
$ mfront --obuild -interface=umat norton.mfront
~~~~~~~~~~~~~~~~~~~

suivant le code que l’on souhaite utiliser. Ceci génère trois
répertoires : `src`, `include` et `castem`.

Le répertoire `src` contient en particulier une bibliothèque dynamique nommée
`libUmatBehaviour.so`.

Le répertoire `castem` contient un exemple de mise en donnée pour la
loi générée pour chacune des hypothèses de modélisation supportée.

## Intégration dans `Cast3M`

Il existe actuellement deux manière d'introduire la loi produite
précédemment dans `Cast3M` :

- la première utilise le module bouchon `umat.eso` (la seule
disponible dans la version client 2014).
- la seconde utilise l'appel direct à la librairie générée. Cette
  possibilité nécessite une version modifiée de `Cast3M` 2014.

### Méthode standard

Pour l'utilisation d'une loi `MFront` de la méthode standard, il nous faut:

- créer une sous-routine `umat` qui surchargera la version par défaut
  livrée avec `Cast3M` ;
- compiler cette sous-routine ;
- compiler un nouvel exécutable `Cast3M`.

Seule la dernière étape nécessitera un effort particulier.

#### La sous-routine `umat`

L'implantation de la sous-routine `umat` est direct:

~~~~{#umat .fortran .numberLines}
      SUBROUTINE UMAT ( STRESS, STATEV, DDSDDE, SSE, SPD, SCD,
     &                  RPL, DDSDDT, DRPLDE, DRPLDT,
     &                  STRAN, DSTRAN, TIME, DTIME,
     &                  TEMP, DTEMP, PREDEF, DPRED,
     &                  CMNAME, NDI, NSHR, NTENS, NSTATV,
     &                  PROPS, NPROPS, COORDS,
     &                  DROT, PNEWDT, CELENT, DFGRD0, DFGRD1,
     &                  NOEL, NPT, LAYER, KSPT, KSTEP, KINC )

      IMPLICIT INTEGER(I-N)
      IMPLICIT REAL*8(A-H,O-Z)

C-----------------------------------------------------------------------
C     Arguments de l'interface
C
      CHARACTER*16  CMNAME
C
      INTEGER       NDI, NSHR, NTENS, NSTATV, NPROPS,
     &              NOEL, NPT, LAYER, KSPT, KSTEP, KINC
C
      REAL*8        STRESS(NTENS), STATEV(*),
     &              DDSDDE(NTENS,NTENS),
     &              SSE, SPD, SCD,
     &              RPL, DDSDDT(NTENS), DRPLDE(NTENS), DRPLDT,
     &              STRAN(NTENS), DSTRAN(NTENS),
     &              TIME(2), DTIME,
     &              TEMP, DTEMP, PREDEF(*), DPRED(*),
     &              PROPS(NPROPS),
     &              COORDS(3),
     &              DROT(3,3),
     &              PNEWDT,
     &              CELENT,
     &              DFGRD0(3,3), DFGRD1(3,3)
C
C     Variables locales
C
      CHARACTER*4   K4ILOI
C=======================================================================
C     Modele externe non reconnu
C=======================================================================
C
      K4ILOI = CMNAME(13:16)

      IF (K4ILOI.EQ.'   1') THEN
         
         CALL UMATNORTON(STRESS, STATEV, DDSDDE, SSE, SPD, SCD,
     &                   RPL, DDSDDT, DRPLDE, DRPLDT,
     &                   STRAN, DSTRAN, TIME, DTIME,
     &                   TEMP, DTEMP, PREDEF, DPRED,
     &                   CMNAME, NDI, NSHR, NTENS, NSTATV,
     &                   PROPS, NPROPS, COORDS,
     &                   DROT, PNEWDT, CELENT, DFGRD0, DFGRD1,
     &                   NOEL, NPT, LAYER, KSPT, KSTEP, KINC )

      ELSE
C
         KINC = -2
C
      ENDIF
C
      RETURN
      END
~~~~~~~~~~~~~~~~~~~

L'appel à la loi générée par `MFront` se fait entre les lignes \(46\)
et \(53\). Nous pouvons noter que celle-ci prend exactement les mêmes
arguments que la fonction `umat` initiale.

La sous-routine précédente est implantée dans un fichier nommé
`umat.eso`.

#### Compilation de la sous-routine `umat`

La compilation de la loi précédente peut se faire en utilisant le
script `compilcast`:

~~~~ {.bash}
$ compilcast umat.eso
~~~~~~~~~~~~~~~~~~~

#### Génération de l'exécutable `Cast3M`

La génération de l'exécutable `Cast3M` nécessite de modifier le script
`essaicast14` (dans le cas de la version 2014) pour que l'exécutable
soit lié à la librairie générée par `MFront`.

Commençons par copier ce script localement:

~~~~ {.bash}
$ cp $(which essaicast14) .
~~~~~~~~~~~~~~~~~~~

et éditons le. Il faut modifier les lignes suivantes :

~~~~ {.bash}
  # Edition des liens pour générer le fichier "${CAST}"
  ${GFORTRAN} -m${BIT} -static-libgfortran -static-libgcc -o ${CAST} temp/main.o temp/bdata.o ${O_BINARY} \
         ${CASTEM_REP}/lib${BIT}/libcastem_${BIT}.a    \
         ${CASTEM_REP}/lib${BIT}/libesope_${BIT}.a     \
         ${CASTEM_REP}/lib${BIT}/libfxdr_${BIT}.a      \
         ${CASTEM_REP}/lib${BIT}/libjpeg_${BIT}.a      \
         ${CASTEM_REP}/lib${BIT}/libmed_${BIT}.a       \
         ${CASTEM_REP}/lib${BIT}/libhdf5_${BIT}.a      \
         $SHARED_LIBRARY -lpthread -lrt ${GLUTLIB} -lGLU -lGL -lX11 -lstdc++ -ldl > link_${CAST}.log 2>&1
~~~~~~~~~~~~~~~~~~~

et les remplacer par:

~~~~ {.bash}
  # Edition des liens pour générer le fichier "${CAST}"
  ${GFORTRAN} -m${BIT} -static-libgfortran -static-libgcc -o ${CAST} temp/main.o temp/bdata.o ${O_BINARY} \
         ${CASTEM_REP}/lib${BIT}/libcastem_${BIT}.a    \
         ${CASTEM_REP}/lib${BIT}/libesope_${BIT}.a     \
         ${CASTEM_REP}/lib${BIT}/libfxdr_${BIT}.a      \
         ${CASTEM_REP}/lib${BIT}/libjpeg_${BIT}.a      \
         ${CASTEM_REP}/lib${BIT}/libmed_${BIT}.a       \
         ${CASTEM_REP}/lib${BIT}/libhdf5_${BIT}.a      \
		 -Lsrc -lUmatBehaviour                         \
         $SHARED_LIBRARY -lpthread -lrt ${GLUTLIB} -lGLU -lGL -lX11 -lstdc++ -ldl > link_${CAST}.log 2>&1
~~~~~~~~~~~~~~~~~~~

(Nous n'avons modifié que l'avant dernière ligne).

Il suffit alors de lancer la version locale de ce script :

~~~~ {.bash}
$ ./essaicast14 umat.eso
~~~~~~~~~~~~~~~~~~~

#### Test

Pour tester notre loi, il est nécessaire que la librairie
`libUmatBehaviour.so` soit accessible. Pour cela, il faut mettre à
jour la variable d'environnement `LD_LIBRARY_PATH` :

~~~~ {.bash}
$ export LD_LIBRARY_PATH=$(pwd)/src
~~~~~~~~~~~~~~~~~~~

Dans la version \(2014\), il est nécessaire de modifier le script
`castem14`. En effet, celui-ci écrase la valeur de la variable
`LD_LIBRARY_PATH`. Il faut modifier la ligne \(38\) du script ainsi :

~~~~ {.bash .numberLines  startFrom="38"}
export LD_LIBRARY_PATH="/lib64:/usr/lib64:/usr/local/lib64:/lib:/lib/i686:/usr/lib:/usr/local/lib:$CASTEM_REP/lib$BIT":$LD_LIBRARY_PATH
~~~~~~~~~~~~~~~~~~~


Pour tester notre loi, nous pouvons utiliser le jeu de donnés suivant,
qui fonctionne si on a utilisé la première méthode d'intégration~:

~~~~{.python .numberLines}
'OPTION' 'ERREUR' 'FATALE' ;
'OPTION' 'DIME' 2 'ELEM' qua4;
'OPTION' 'MODELISER' 'AXISYMETRIE';

TMAX = 1.;
NPAS = 50;
UMAX = 3.5e-2;

O = 0. 0.;
A = 1. 0.;

l1 = 'DROIT' 1 O A;
l2 = l1 'PLUS' (0. 1.);

s1 = l1 'REGLER' 1 l2;

coel2D = 'MOTS' 'YOUN' 'NU' 'RHO' 'ALPH' 'A' 'E';
stav2D = 'MOTS' 'EERR' 'EEZZ' 'EETT' 'EERZ'  'P';
para2D = 'MOTS' 'T';

mod1 = 'MODELISER' s1 'MECANIQUE' 'ELASTIQUE' 'ISOTROPE'
   'NON_LINEAIRE' 'UTILISATEUR'
   'NUME_LOI' 1
   'C_MATERIAU' coel2D
   'C_VARINTER' stav2D
   'PARA_LOI'   para2D
   'CONS' M;
   
MAT1 = 'MATERIAU' MOD1 'YOUN' 80e9 'NU' 0.35 'ALPH' 0. 'RHO' 0.
   'A' (0.003944e-6 '**' 4.39) 'E' 4.39;

***
LIT1 = PROG 0. PAS (TMAX/ NPAS)  TMAX;

*** CONDITIONS AUX LIMITES
************************************************
* Conditions aux limites éprouvette
CL1 = 'BLOQUE' 'UZ'  L1 ;
CL2 = 'BLOQUE' 'UZ'  L2 ;
CLE1 = CL1 ET CL2;

* Chargement mécanique et thermique
LI1 = PROG 0. TMAX ;
LI2 = PROG 0. 1. ;
EV = EVOL MANU T LI1 F(T) LI2 ;
DEP1 = DEPI CL2 UMAX;
CHA1 = CHAR 'DIMP' DEP1 EV ;

* Champ de température
TEK = 293.15;
THE1 = MANU 'CHPO' S1 1 'T' TEK ;
EV2 = EVOL MANU (prog 0. TMAX) (prog 1. 1.) ;
CHARTHER = CHAR 'T' THE1 EV2 ;
                             
** CALCUL
* Définition des pas de calcul et de sauvegarde
************************************************************
* Définition de la table de la phase de charge
TAB1 = TABLE ;
*TAB1.'K_SIGMA' =  FAUX;
TAB1.'MOVA' = 'MOT' 'RIEN' ;
TAB1.'TEMPERATURES' = TABLE ;
TAB1.'VARIABLES_INTERNES' = TABLE ;
TAB1.'BLOCAGES_MECANIQUES' = CLE1 ;
TAB1.'MODELE' = MOD1 ;
TAB1.'CHARGEMENT' = CHA1 'ET' CHARTHER ;
TAB1.'TEMPERATURES' . 0 = THE1 ;
TAB1.'CARACTERISTIQUES' = MAT1 ;        
TAB1.'TEMPS_CALCULES' = LIT1 ;
TAB1.'TEMPS_SAUVES' = LIT1 ;
TAB1.VARIABLES_INTERNES.0 = (ZERO MOD1 'VARINTER');
TAB1.'PRECISION' = 1.e-8;

* Lancement du calcul de la phase de charge 
PASAPAS TAB1 ;

'REPETER' i ('DIME' tab1.'CONTRAINTES');
   idx = &i '-' 1;
   s = tab1.'CONTRAINTES' . idx;
   d = tab1.'DEPLACEMENTS'. idx;
   v = tab1.'VARIABLES_INTERNES'. idx;
   'MESSAGE' ('MAXIMUM' ('EXCO' d 'UZ')) ' '
             ('MINIMUM' ('EXCO' d 'UR')) ' '
             ('MAXIMUM' ('EXCO' s 'SMZZ')) ' '
             ('MAXIMUM' ('EXCO' v 'P'));
'FIN' i;

nb = ('DIME' tab1.'CONTRAINTES') '-' 1;
psig = 'PROG';
peto = 'PROG';
'REPETER' i nb;
   s = tab1.'CONTRAINTES' . &i;
   d = tab1.'DEPLACEMENTS'. &i;
   psig = psig 'ET' ('PROG' ('MAXIMUM' ('EXCO' s 'SMZZ')));
   peto = peto 'ET' ('PROG' ('MAXIMUM' ('EXCO' d 'UZ')));
'FIN' i;


EVSIG = 'EVOL' 'ROUG' 'MANU' 'EZZ' peto 'SMZZ' psig;
'LISTE' evsig;
'DESSIN' evsig;

'FIN';
~~~~

`MFront` est fourni avec un logiciel de test des lois de comportement
mécanique sur un point matériel (voir la
[page dédiée](http://tfel.sourceforge.net/mtest.html)). Le résultat
obtenu par `Cast3M` peut-être comparé à la simulation `MTest`
équivalente sur la figure ci-dessous :

![Comparaison des résultats `MTest`/`Cast3M`](img/NortonCreepUniaxialTesting
 "Comparaison des résultats `MTest`/`Cast3M`")

### Appels dynamiques

Il est possible d'utiliser un appel direct à la librairie `MFront`
générée. Pour la version \(2014\), il est nécessaire d'appliquer les
[certains patchs](downloads/patchs-Cast3M-2014.tar.bz2).

Pour utiliser la librairie générée via les appels dynamiques, il
suffit de modifier l'appel à l'opérateur `MODELISER` dans l'exemple
précédent ainsi (lignes 21 à 27) :

~~~~{.python .numberLines startFrom="21"}
mod1 = 'MODELISER' s1 'MECANIQUE' 'ELASTIQUE' 'ISOTROPE'
   'NON_LINEAIRE' 'UTILISATEUR'
   'LIB_LOI' 'src/libUmatBehaviour.so'
   'FCT_LOI' 'umatnorton'
   'C_MATERIAU' coel2D
   'C_VARINTER' stav2D
   'PARA_LOI'   para2D
   'CONS' M;
~~~~

Nous avons explicitement indiqué le chemin vers la librairie. En
pratique, il est préférable de ne pas le faire et de modifier la
variable `LD_LIBRARY_PATH`.

# Référence
