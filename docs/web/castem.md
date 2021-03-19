---
title: MFront interface for Cast3M
author: Thomas Helfer
date: October 13, 2014
lang: en-EN
numbersections: true
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
bibliography: bibliography.bib
---

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

# Mechanical behaviours

The `castem` and `castem21` interfaces have been developed for the CEA
[`Cast3M`](http://www-cast3m.cea.fr/) finite element solver using the
[`UMAT`](http://www-cast3m.cea.fr/index.php?page=sources&source=umat)
subroutine which is meant to be overridden by the user, loosely
following the `Abaqus` standard.

The `castem` interface shall be used for versions prior to Version `21`
and the `castem21` interface shall be used for versions greater than
`21`.

For historical reasons, the `castem` interface also has an alias named
`umat`.

> **Warning**
>

> For technical reasons, the interfaces `castem` and `castem21` can't be
> used in the same directory.

A french version of this document is available [here](castem-fr.html).

This document has been written using `Cast3M` greater than 2014. We
will assume working in a standard `POSIX` environment. The shell used
is `bash`.

We will use the Norton behaviour described in the
[tutorial](tutorial.html). All the files used can be downloaded
[here](downloads/example-castem2014.tar.bz2).

We broke this documents in three parts:

- compilation of the behaviour
- use in the `Cast3M` finite element solver
- unit testing of an uniaxial tensile test.

## Compilation of the behaviour

The following instruction will compile the `MFront` behaviour using
the `castem` interface :

~~~~ {.bash}
$ mfront --obuild --interface=castem norton.mfront
~~~~~~~~~~~~~~~~~~~

This is equivalent to:

~~~~ {.bash}
$ mfront --obuild --interface=umat norton.mfront
~~~~~~~~~~~~~~~~~~~

The `umat` interface is deprecated as it may be confusing for `Abaqus`
users.

Three directories are created, respectively called `include`, `src`,
`castem`. Only the `src` and `castem` directories have interest for
the end-user:

- the `src` directory contains the a shared library called
  `libUmatBehaviour.so`.
- the `castem` directory contains an example of how to use the
  generated behaviour for each modelling hypothesis supported.

## Usage in `Cast3M`

[A patch](downloads/patchs-Cast3M-2014.tar.bz2) must be applied to
`Cast3M` 2014 to be able to call shared libraries directly. This patch
is not mandatory for version greater than 2015.

The `MODELISER` operator can be called as follows:

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

## Test

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
   'LIB_LOI' 'src/libUmatBehaviour.so'
   'FCT_LOI' 'umatnorton'
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

# Material proprerties

## Thermal conductivity of \(UPuC\)

The thermal conductivity of \(UPuC\) \(k\paren{T,p,\tau}\) depends on
the temperature \(T\) (Kelvin), the porosity \(p\) and the Burn-Up
\(\tau\) \(at.\%\)

Here is the implementation of this material property using `MFront`:

~~~~{#UPuCThermalConductivity .cpp .numberLines}
@Parser MaterialLaw;
@Law ThermalConductivity;
@Material UPuC;
@Author Thomas Helfer;
@Output k; //< changing the name of output
@Input T,p,Bu; //< inputs of the law
T.setGlossaryName("Temperature"); //< pleiades name
p.setGlossaryName("Porosity"); //< pleiades name
Bu.setGlossaryName("BurnUp"); //< pleiades name
@PhysicalBounds T in [0 :*[; //< temperature physical bounds
@Bounds T in [0 :2573.15]; //< temperature bounds
@PhysicalBounds p in [0 :1]; //< porosity physical bounds
@PhysicalBounds Bu in [0 :*[; //< burn-up physicalbounds
@Function{
  if (T<=773.15){
    k = (8.14e-6*T-0.010096882)*T+19.65063040915;
  } else {
    k = (-1.88e-6*T+0.009737044)*T+10.2405949657;
  }
  k *= (1.-p)/(1.+2.*p);
  k *= 1.-(0.02*Bu);
}
~~~~~~~~

This implementation can be turned in a shared library callable from `Cast3M`:

~~~~{#UPuCThermalConductivity-mfront .bash}
$ mfront --obuild --interface=castem UPuCThermalConductivity.mfront
~~~~~~~~~~~~~~~~~~~~~~~~~

## Usage in `Cast3M`

A patch must be applied to be
able to call shared libraries directly from the `MATERIAU` operator:

-|Cast3M 2014](downloads/patchs-Cast3M-2014.tar.bz2)
-|Cast3M 2015](downloads/patchs-Cast3M-2015.tar.bz2)

The `MATERIAU` can accept a table as parameter.

~~~~{#UPuCThermalConductivity-castem .python .numberLines}
* Création d un modèle thermique isotrope
ModT1 = 'MODELISER' s1 'THERMIQUE' 'ISOTROPE';
* Création d une table contenant les données relatives
* à la propriété externe :
* - 'MODELE' contient le nom de la fonction appelée
* - 'LIBRAIRIE' contient le nom de la librairie externe
* dans laquelle cette fonction est définie
* - 'VARIABLES' contient la liste des paramètres dont dépend
* la fonction appelée
Tmat = 'TABLE';
Tmat. 'MODELE' = 'UPuC_ThermalConductivity';
Tmat. 'LIBRAIRIE' = 'libUPuCMaterialProperties.so';
Tmat. 'VARIABLES' = 'MOTS' 'T' 'PORO' 'FIMA';
* Création du matériau.
MatT1 = 'MATERIAU' ModT1 'K' Tmat;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When using `PASAPAS`, loadings named `PORO` and `FIMA` must be
declared for the material properties to be evaluated.

<!-- Local IspellDict: english -->
