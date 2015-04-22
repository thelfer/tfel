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

# Lois de comportement mécanique

L'utilisation de lois générées par `MFront` dans
[`Cast3M`](http://www-cast3m.cea.fr/) se fait via l'interface
générique
[`UMAT`](http://www-cast3m.cea.fr/index.php?page=sources&source=umat).

Dans la suite de ce document, nous supposons travailler dans un
environnement `POSIX`. Le shell utilisé est `bash`. Une page dédiée à
l'utilisation de `MFront` sous `Windows` sera ajoutée à terme.

Nous utiliserons la loi de Norton décrite dans le
[tutoriel](tutorial.html). L'ensemble des fichiers utilisés dans ce
document peut être téléchargé
[ici](downloads/example-castem2014.tar.bz2).

Ce document se découpe en trois partie :

- compilation de la loi ;
- intégration dans `Cast3M` ;
- test sur un essai de traction uniaxial.

## Compilation de la loi

MFront gère la compilation de la loi, il suffit de taper dans un
terminal :

~~~~ {.bash}
$ mfront --obuild --interface=umat norton.mfront
~~~~~~~~~~~~~~~~~~~

Ceci génère trois répertoires : `src`, `include` et `castem`.

Le répertoire `src` contient en particulier une bibliothèque dynamique nommée
`libUmatBehaviour.so`.

Le répertoire `castem` contient un exemple de mise en donnée pour la
loi générée pour chacune des hypothèses de modélisation supportée.

## Intégration dans `Cast3M`

Il existe actuellement deux manière d'introduire la loi produite
précédemment dans `Cast3M` :

- la première utilise l'appel direct à la librairie générée. Cette
  possibilité nécessite une version modifiée de `Cast3M` 2014.
- la seconde utilise le module bouchon `umat.eso` (Seule cette méthode
est disponible dans la version client 2014).

### Appels dynamiques

Il est possible d'utiliser un appel direct à la librairie `MFront`
générée. Pour la version \(2014\), il est nécessaire d'appliquer les
[certains patchs](downloads/patchs-Cast3M-2014.tar.bz2). Ce patch
n'est plus nécessaire depuis la version 2015.

Pour utiliser la librairie générée via les appels dynamiques, l'appel
à l'opérateur `MODELISER` se fait ainsi :

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

Nous avons explicitement indiqué le chemin vers la librairie. En
pratique, il est préférable de ne pas le faire et de modifier la
variable `LD_LIBRARY_PATH`.

### Méthode standard (dépréciée depuis `Cast3M` 2015)

Pour les version antérieures de `Cast3M` 2015, en l'absence du patch
présenté plus haut, il est nécessaire de faire de passer par des
étapes plus complexes:

- créer une sous-routine `umat` qui surchargera la version par défaut
  livrée avec `Cast3M` ;
- compiler cette sous-routine ;
- compiler un nouvel exécutable `Cast3M`.

Seule la dernière étape nécessitera un effort particulier. Insistons:
depuis `Cast3M` 2015, cette façon de procéder est inutilement
complexe.

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

#### Utilisation de l'exécutable généré

Pour utiliser ce nouvel exécutable , il est nécessaire que la librairie
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

L'appel à la loi se fait alors de manière standard:

~~~~{.python}
mod1 = 'MODELISER' s1 'MECANIQUE' 'ELASTIQUE' 'ISOTROPE'
   'NON_LINEAIRE' 'UTILISATEUR'
   'NUME_LOI' 1
   'C_MATERIAU' coel2D
   'C_VARINTER' stav2D
   'PARA_LOI'   para2D
   'CONS' M;
~~~~

où `s1` est le nom du support géométrique du modèle.

## Test

Pour tester notre loi, nous pouvons utiliser le jeu de donnés suivant :

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

 Le résultat obtenu par `Cast3M` peut-être comparé à la simulation
`MTest` équivalente sur la figure ci-dessous :

![Comparaison des résultats `MTest`/`Cast3M`](img/NortonCreepUniaxialTesting
 "Comparaison des résultats `MTest`/`Cast3M`")

# Propriétés matériau

## Conductivité thermique du combustible carbure \(UPuC\)

La conductivité thermique d'un combustible carbure \(UPuC\)
\(k\paren{T,p,\tau}\) est supposée dépendre de la temperature \(T\)
(Kelvin), de la porosité \(p\) et du taux de combustion \(\tau\)
\(at.\%\)

Une implantation possible de cette propriété matériau en `MFront` est
la suivante:

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

Cette implantation peut être compilée en une librairie dynamique appelable depuis `Cast3M`:

~~~~{#UPuCThermalConductivity-mfront .bash}
$ mfront --obuild --interface=castem UPuCThermalConductivity.mfront
~~~~~~~~~~~~~~~~~~~~~~~~~

# Utilisation dans `Cast3M`

L'appel à des librairies dynamiques depuis l'opérateur `MATERIAU` nécessite
l'application d'un patch~:

-|Cast3M 2014](downloads/patchs-Cast3M-2014.tar.bz2)
-|Cast3M 2015](downloads/patchs-Cast3M-2015.tar.bz2)

La directive `MATERIAU` accepte alors une table comme paramètre:

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

Pour une utilisation dans `PASAPAS`, deux chargements `PORO` et `FIMA`
doivent être définis.

<!-- Local IspellDict: english -->
