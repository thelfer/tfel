% MFront usage in [Cast3M](http://www-cast3m.cea.fr/)
% Helfer Thomas
% October 13, 2014

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

The `umat` interface has been developed for the CEA
[`Cast3M`](http://www-cast3m.cea.fr/) finite element solver. Its name
comes from the name of
[`UMAT`](http://www-cast3m.cea.fr/index.php?page=sources&source=umat)
subroutine which is meant to be overridden by the user, loosely
following the `Abaqus` standard.

A french version of this document is available [here](castem-fr.html).

This document has been written using `Cast3M` 2014. We will assume
working in a standard `POSIX` environment. The shell used is `bash`.

We will use the Norton behaviour described in the
[tutorial](tutorial.html). All the files used can be downloaded
[here](downloads/example-castem2014.tar.bz2).

We broke this documents in three parts:

- compilation of the behaviour
- use in the `Cast3M` finite element solver
- unit testing of an uniaxial tensile test.

# Compilation of the behaviour

The following instruction will compile the `MFront` behaviour using
the `umat` interface :

~~~~ {.bash}
$ mfront --obuild -interface=umat norton.mfront
~~~~~~~~~~~~~~~~~~~

Three directories are created, respectively called `include`, `src`,
`castem`. Only the `src` and `castem` directories have interest for
the end-user:

- the `src` directory contains the a shared library called
  `libUmatBehaviour.so`.
- the `castem` directory contains an example of how to use the
  generated behaviour for each modelling hypothesis supported.

# Usage in `Cast3M`

There are two ways to use `MFront` generated libraries in the `Cas3M`
finite element solver:

- using direct call to the generated library
- modifying the `UMAT` subroutine. This is method is the only one
  available in the standard version of `Cast3M` 2014.

## Call to shared libraries

[A patch](downloads/patchs-Cast3M-2014.tar.bz2) must be applied to
`Cast3M` 2014 to be able to call shared libraries directly.

This patch allows the following syntax of the `MODELISER` operator:

~~~~{.python}
mod1 = 'MODELISER' s1 'MECANIQUE' 'ELASTIQUE' 'ISOTROPE'
   'NON_LINEAIRE' 'UTILISATEUR'
   'LIB_LOI' 'src/libUmatBehaviour.so'
   'FCT_LOI' 'umatnorton'
   'C_MATERIAU' coel2D
   'C_VARINTER' stav2D
   'PARA_LOI'   para2D
   'CONS' M;
~~~~

Here, we have explicitly given the path to the library. In practice,
it is better to modify the `LD_LIBRARY_PATH` environment variable.

## Standard method

The standard method to integrate a `MFront` behaviour through the
`UMAT` subroutine is described in depth in the
[french version](castem-fr.html) of this document.

# Test

To test our behaviour, we can use the following input file:

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

The results given by `Cast3M` can be compared to an equivalent `MTest`
computation on the following figure:

![Comparaison des résultats `MTest`/`Cast3M`](img/NortonCreepUniaxialTesting
 "Comparaison des résultats `MTest`/`Cast3M`")

# References
