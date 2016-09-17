C UMAT      SOURCE    CHAT      11/03/16    21:32:49     6902           
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
C
C     DESCRIPTION FONCTIONNELLE :
C     -------------------------
C
C     Version du module externe UMAT integree au code pour eviter tout
C     probleme d'edition de lien et pour permettre le passage de tests
C     de non regression.
C
C     En utilisation normale, ce module doit etre surcharge par celui
C     de meme nom fourni par l'utilisateur.
C
C     Si cette version de UMAT est appelee, cela signifie qu'il n'y a
C     pas eu surcharge par le module utilisateur
C     ATTENTION : si l'utilisateur oublie de surcharger et affecte a sa
C     loi le meme numero que l'un de ceux pris en charge ci-apres, il y
C     a risque d'erreur non controlee a l'execution.
C
C     CONVENTION : on autorise la programmation de l'utilisateur a se
C     servir de KINC comme code retour, de la meme maniere que l'on
C     s'en sert ci-dessous. Regles a respecter :
C     1 - Pas d'initialisation superflue de KINC en entrant dans UMAT.
C         KINC est initialise a 1 par WKUMA0, avant l'appel a UMAT.
C     2 - En cas d'erreur, KINC est affecte d'une valeur differente de 1
C
C      Dans ce dernier cas, l'utilisateur a la possibilite de completer
C      le message d'erreur affiche (de numero 965) en surchargeant le
C      sousprogramme UMAERR (umaerr.eso).
C
C-----------------------------------------------------------------------
C
C     DESCRIPTION DE L'INTERFACE :
C     --------------------------
C
C     IN/OUT : STRESS : REAL*8(NTENS), tenseur des contraintes
C                       En entree : tenseur des contraintes a t0
C                       En sortie : tenseur des contraintes a t0+dt
C                       Dans le cas des grandes transformations, il
C                       s'agit des contraintes de Cauchy ("actuelles").
C     IN/OUT : STATEV : REAL*8(*), variables internes
C                       En entree : variables internes a t0
C                       En sortie : variables internes a t0+dt
C
C
C     OUT    : DDSDDE : REAL*8(NTENS,NTENS), matrice jacobienne du
C                       modele (matrice de Hooke tangente) a t0+dt
C                       Sortie facultative, non exploitee par CAST3M
C                       pour l'instant
C
C     IN/OUT : SSE    : REAL*8, energie de deformation elastique
C              SPD    : REAL*8, dissipation plastique
C              SCD    : REAL*8, dissipation visqueuse
C                       Valeurs a t0 en entree, a t0+dt en sortie
C                       Entrees/sorties facultatives, non exploitees par
C                       CAST3M pour l'instant
C
C
C     OUT    : RPL    : REAL*8, puissance calorifique volumique degagee
C                       par le travail mecanique, a t0+dt
C              DDSDDT : REAL*8(NTENS),derivee du tenseur des contraintes
C                       par rapport a la temperature, a t0+dt
C              DRPLDE : REAL*8(NTENS), derivees de RPL par rapport aux
C                       composantes du tenseur des deformations, a t0+dt
C              DRPLDT : REAL*8, derivee de RPL par rapport a la
C                       temperature, a t0+dt
C                       Sorties facultatives, non exploitees par CAST3M
C                       pour l'instant
C                       Ces sorties sont prevues pour un couplage fort
C                       entre thermique et mecanique
C
C
C     IN     : STRAN  : REAL*8(NTENS), tenseur des deformations
C                       totales a t0
C              DSTRAN : REAL*8(NTENS), tenseur des increments de
C                       deformation totale par rapport a l'etat
C                       de reference a t0
C
C
C     IN     : TIME   : REAL*8(2), TIME(1) = 0
C                                  TIME(2) = t0
C              DTIME  : REAL*8,    DTIME   = dt
C                       t0 : precedent instant d'equilibre atteint
C                       dt : nouveau pas de temps propose par PASAPAS
C                       pour atteindre l'equilibre avec l'increment de
C                       deformation totale impose (DSTRAN)
C                       Remarque : les notions attachees a ces entrees
C                       sont differentes lorsque l'on adhere a ABAQUS
C                       (dans ce cas la variable TIME(1) a son utilite)
C
C
C     IN     : TEMP   : REAL*8, temperature a t0
C              DTEMP  : REAL*8, increment de temperature a t0+dt
C              PREDEF : REAL*8(*), vecteur des parametres externes de
C                       la loi de comportement, valeurs a t0
C              DPRED  : REAL*8(*), increments des parametres externes
C                       a t0+dt
C
C
C     IN     : CMNAME : CHARACTER*16, identifiant de la loi de
C                       comportement
C                       On conserve le type 'chaine de caracteres' pour
C                       l'identifiant de la loi, afin de preserver la
C                       compatibilite avec ABAQUS.
C                       Dans le cas d'une adherence a CAST3M, la loi est
C                       identifiee par le numero qui lui a ete attribue:
C                       argument 'NUME_LOI' de l'operateur MODE.
C                       Par convention, ce numero est encode dans les 4
C                       derniers caracteres de la chaine, et doit etre
C                       recupere dans UMAT par une instruction du type
C                       K4ILOI = CMNAME(13:16)
C                       avec K4ILOI variable locale de type CHARACTER*4
C
C
C     IN     : NDI    : INTEGER definissant le type de calcul CAST3M
C                       =  2 : tridimensionnel (3D)
C                       =  0 : axisymetrie (2D)
C                       = -1 : deformations planes (2D)
C                       = -2 : contraintes planes (2D)
C                       = -3 : deformations planes generalisees (2D)
C                       =  3 a 11 : unidimensionnel plan (massif 1D)
C                       = 12 a 14 : unidimensionnel axisymetrique (massif 1D)
C                       = 15 : unidimensionnel spherique (massif 1D)
C
C
C     IN     : NSHR   : INTEGER, nombre de composantes extradiagonales
C                       du tenseur des contraintes
C                       Entree non active dans le cas d'une adherence
C                       a CAST3M
C
C
C     IN     : NTENS  : INTEGER, nombre de composantes du tenseur des
C                       contraintes
C              NSTATV : INTEGER, nombre de variables internes
C
C
C     IN     : PROPS  : REAL*8(NPROPS), vecteur des constantes
C                       de materiau
C                       N.B. Il peut s'agir de coefficients constants
C                       intervenant dans des formules analytiques
C                       decrivant les evolutions des proprietes du
C                       materiau en fonction de la temperature et/ou
C                       d'autres parametres externes.
C                       OU il peut s'agir plus simplement de proprietes
C                       de materiau constantes.
C              NPROPS : INTEGER, nombre de constantes de materiau
C
C
C     IN     : COORDS : REAL*8(3), coordonnees cartesiennes du point
C                       d'integration courant
C
C
C     IN     : DROT   : REAL*8(3,3)
C                       Dans le cas d'une adherence a CAST3M
C                       ------------------------------------
C                       Matrice de passage du repere local de l'element
C                       fini massif au repere general du maillage
C                       DROT(l,c) : composante l du vecteur c de la base
C                       locale dans la base generale
C                       Entree non active pour les elements finis non
C                       massifs
C                       Dans le cas d'une adherence a ABAQUS
C                       ------------------------------------
C                       Matrice d'increments de rotation
C                       Cette matrice decrit la rotation sur le pas de
C                       temps de la base dans laquelle sont exprimes
C                       les tenseurs de contraintes et de deformations
C
C
C     OUT    : PNEWDT : REAL*8, rapport entre le nouveau pas de temps
C                       suggere et le pas de temps donne en entree
C                       (NEWDT = PNEWDT * DTIME)
C
C
C     IN     : CELENT : REAL*8, longueur caracteristique de l'element
C                       Dans le cas d'une adherence a CAST3M, cette
C                       longueur caracteristique est determinee par
C                       LOCARA comme la distance maximale entre deux
C                       noeuds de l'element.
C
C
C     IN     : DFGRD0 : REAL*8(3,3), tenseur gradient de deplacement
C                       a t0
C              DFGRD1 : REAL*8(3,3), tenseur gradient de deplacement
C                       a t0+dt
C
C
C     IN     : NOEL   : INTEGER, numero de l'element courant
C              NPT    : INTEGER, numero du point d'integration courant
C
C
C     IN     : LAYER  : INTEGER, numero de couche pour des coques
C                       composites ou des solides multi-couches
C              KSPT   : INTEGER, numero de section dans la couche
C                       courante
C                       Entrees non actives dans le cas d'une adherence
C                       a CAST3M
C
C
C     IN     : KSTEP  : INTEGER
C     IN     : KINC   : INTEGER
C                       Entrees n'ayant pas de sens dans le cas
C                       d'une adherence a CAST3M
C
C-----------------------------------------------------------------------
C
C     MODELES PRIS EN CHARGE par le module UMAT 'bouchon' :
C     ---------------------------------------------------
C
C     ILOI = 10 : ELASTICITE ISOTROPE ISOTHERME
C                 FORMULATION MASSIVE
C
C     ILOI = 11 : ELASTICITE ISOTROPE NON ISOTHERME
C                 FORMULATION MASSIVE
C                 La temperature 'TETA' est definie en tant que
C                 parametre externe
C              => Instanciation du materiau et prise en charge de la
C                 dilatation thermique dans UMAT
C
C     ILOI = 12 : ELASTICITE ORTHOTROPE ISOTHERME
C                 FORMULATION MASSIVE
C
C     ILOI = 21 : Modele 'FLUAGE' 'POLYNOMIAL' de CAST3M, INPLAS=21
C                 RESTRICTION A LA FORMULATION MASSIVE
C
C     ILOI = 22 : Modele 'FLUAGE' 'POLYNOMIAL' de CAST3M, INPLAS=21
C                 RESTRICTION A LA FORMULATION MASSIVE
C                 Instanciation des composantes de materiau au fur et a
C                 mesure des iterations internes.
C                 Le modele comprend cette fois des parametres externes.
C                 Les composantes de materiau sont evaluees par les
C                 memes fonctions externes que celles utilisees par
C                 l'operateur VARI.
C                 MODELE 'MAQUETTE'
C
C     ILOI =  7 : Modele 'VISCOPLASTIQUE' 'GATT_MONERIE' de CAST3M,
C                 INPLAS=107
C                 RESTRICTION A LA FORMULATION MASSIVE
C
C     ILOI =  5 : Modele 'PLASTIQUE' 'ISOTROPE' de CAST3M, INPLAS=5
C                 RESTRICTION A LA FORMULATION MASSIVE
C
C     ILOI = 31 : Modele MOONEY-RIVLIN en GRANDES TRANSFORMATIONS
C                 Cas des CONTRAINTES PLANES dans la direction 3 (2D/3D)
C                 Restriction a la formulation MASSIVE
C
C     ILOI = 32 : Modele NEO-HOOKIEN en GRANDES TRANSFORMATIONS
C                 Restriction a la formulation MASSIVE
C
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
C
      PARAMETER(UN=1.D0,DEUX=2.D0,UNDEMI=.5D0)
C
      REAL*8        YOUN, NU, BULK3, G2, LAM, EPST0(6), DEPST(6),
     &              TETA0, DTETA, TETAF, ALPH, TREF, DYDT, DNDT,
     &              YOUN0, NU0, BULK30, G20, LAM0,
     &              YOUNF, NUF, BULK3F, G2F, LAMF,
     &              DELDSE(6,6), ETHERM(6), DTHERM(6),
     &              YREF, NREF, EPSE0(6), DEPSE(6),DHOOK(6,6),
     &              DSIG2(3,3),DEPST2(3,3),TENS(3,3),DSIG(6),
     &              V1(3),V2(3),V3(3),V(3,3),P(3,3)
C
C
C---------------------- Debut du code executable -----------------------
C
C
      K4ILOI = CMNAME(13:16)
C
C=======================================================================
C     ELASTICITE ISOTROPE ISOTHERME
C=======================================================================
C
      IF (K4ILOI.EQ.'  10') THEN
C
C-----------------------------------------------------------------------
C        Constantes de materiau
C        ----------------------
C        PROPS(1)  : YOUN  Module d'Young
C        PROPS(2)  : NU    Coefficient de Poisson
C-----------------------------------------------------------------------
C
C        Recuperation des constantes de materiau
C
         YOUN = PROPS(1)
         NU   = PROPS(2)
C
C        Calcul de la matrice de Hooke (constante)
C        Hypotheses : NTENS=6
C                     les composantes diagonales sont les 3 premieres
C
         BULK3 = YOUN/(1.0D0-2.0D0*NU)
         G2 = YOUN/(1.0D0+NU)
         LAM = (BULK3-G2)/3.0D0
C
         DO 100 J=1,NTENS
            DO 101 I=1,NTENS
               DDSDDE(I,J)=0.0D0
 101        CONTINUE
 100     CONTINUE
C
         DO 110 J=2,3
            DO 111 I=1,J-1
               DDSDDE(I,J)=LAM
               DDSDDE(J,I)=LAM
 111        CONTINUE
 110     CONTINUE
         DO 112 J=1,3
            DDSDDE(J,J)=G2+LAM
 112     CONTINUE
         DO 113 J=4,6
            DDSDDE(J,J)=G2
 113     CONTINUE
C
C        Deformations : gamma -> epsilon pour les termes extradiagonaux
C        Uniquement sur les increments car on n'a pas besoin des cumuls
C
         DO 120 J=1,3
            DEPST(J)=DSTRAN(J)
 120     CONTINUE
         DO 121 J=4,6
            DEPST(J)=DSTRAN(J)/2.0D0
 121     CONTINUE
C
C        Calcul des contraintes a la fin du pas
C
         DO 130 J=1,NTENS
            DO 131 I=1,NTENS
               STRESS(I)=STRESS(I)+DDSDDE(I,J)*DEPST(J)
 131        CONTINUE
 130     CONTINUE
C
C=======================================================================
C     Modele HYPERELASTIQUE MOONEY-RIVLIN 
C            CONTRAINTES PLANES INCOMPRESSIBLE (direction 3)
C            DEFORMATIONS PLANES  Quasi INCOMPRESSIBLE
C            TRIDIMENSIONNEL      Quasi INCOMPRESSIBLE
C     Contribution de Laurent Gornet - Ecole Centrale de Nantes (2006)
C=======================================================================
C 
      ELSE IF (K4ILOI.EQ.'  31') THEN
C
        CALL MOONEY ( STRESS, STATEV, DDSDDE, STRAN, DSTRAN,
     &                TIME, DTIME, TEMP, DTEMP, PREDEF, DPRED,
     &                NDI, NSHR, NTENS, NSTATV, PROPS, NPROPS,
     &                DFGRD0, DFGRD1, KSTEP, KINC,SSE,spd,scd,rpl,
     &                DDSDDT,DRPLDE,DRPLDT,cmname,coords,drot, PNEWDT,
     &                celent,NOEL, NPT, LAYER, KSPT )
C
C=======================================================================
C     Modele HYPERELASTIQUE NEO-HOOKEEN COMPRESSIBLE (2006/2010)
C     en GRANDES TRANSFORMATIONS
C     Contribution de Laurent Gornet - Ecole Centrale de Nantes (2010)
C=======================================================================
C
      ELSE IF (K4ILOI.EQ.'  32') THEN
C
        CALL NEOHOO ( STRESS, STATEV, DDSDDE, STRAN, DSTRAN,
     &                TIME, DTIME, TEMP, DTEMP, PREDEF, DPRED,
     &                NDI, NSHR, NTENS, NSTATV, PROPS, NPROPS,
     &                DFGRD0, DFGRD1, KSTEP, KINC )

C
c$$$      ELSE IF (K4ILOI.EQ.'  33') THEN
c$$$C
c$$$        CALL GDGD (STRESS, STATEV, DDSDDE, STRAN, DSTRAN,
c$$$     &                   TIME, DTIME, TEMP, DTEMP, PREDEF, DPRED,
c$$$     &                   NDI, NSHR, NTENS, NSTATV, PROPS, NPROPS,
c$$$     &                   DFGRD0, DFGRD1, KSTEP, KINC,SSE,spd,scd,rpl,
c$$$     &                   DDSDDT,DRPLDE,DRPLDT,cmname,coords,drot,PNEWDT,
c$$$     &                   celent,NOEL, NPT, LAYER, KSPT )
c$$$      ELSE  IF (K4ILOI.EQ.'  34') THEN
c$$$       CALL HARTS (STRESS, STATEV, DDSDDE, STRAN, DSTRAN,
c$$$     &                   TIME, DTIME, TEMP, DTEMP, PREDEF, DPRED,
c$$$     &                   NDI, NSHR, NTENS, NSTATV, PROPS, NPROPS,
c$$$     &                   DFGRD0, DFGRD1, KSTEP, KINC,SSE,spd,scd,rpl,
c$$$     &                   DDSDDT,DRPLDE,DRPLDT,cmname,coords,drot,PNEWDT,
c$$$     &                   celent,NOEL, NPT, LAYER, KSPT )
c$$$C      
c$$$      ELSE  IF (K4ILOI.EQ.'  35') THEN
c$$$C      
c$$$       CALL BIDERM (STRESS, STATEV, DDSDDE, STRAN, DSTRAN,
c$$$     &                   TIME, DTIME, TEMP, DTEMP, PREDEF, DPRED,
c$$$     &                   NDI, NSHR, NTENS, NSTATV, PROPS, NPROPS,
c$$$     &                   DFGRD0, DFGRD1, KSTEP, KINC,SSE,spd,scd,rpl,
c$$$     &                   DDSDDT,DRPLDE,DRPLDT,cmname,coords,drot,PNEWDT,
c$$$     &                   celent,NOEL, NPT, LAYER, KSPT ) 
c$$$      ELSE IF (K4ILOI.EQ.'  36') THEN
c$$$C
c$$$      CALL HUITCH (STRESS, STATEV, DDSDDE, STRAN, DSTRAN,
c$$$     &                   TIME, DTIME, TEMP, DTEMP, PREDEF, DPRED,
c$$$     &                   NDI, NSHR, NTENS, NSTATV, PROPS, NPROPS,
c$$$     &                   DFGRD0, DFGRD1, KSTEP, KINC,SSE,spd,scd,rpl,
c$$$     &                   DDSDDT,DRPLDE,DRPLDT,cmname,coords,drot,PNEWDT,
c$$$     &                   celent,NOEL, NPT, LAYER, KSPT )     
c$$$C=======================================================================
c$$$C     Modele externe non reconnu
c$$$C=======================================================================
c$$$C
      ELSE
C
         KINC = -2
C
      ENDIF
C
      RETURN
      END

 
 
