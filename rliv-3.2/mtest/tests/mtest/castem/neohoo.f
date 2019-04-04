C NEOHOO    SOURCE    FANDEUR   07/02/22    21:15:07     5665
C=======================================================================
C=        MODELE HYPERELASTIQUE NEOHOOKEEN COMPRESSIBLE                =
C=                 EN GRANDES TRANSFORMATIONS                          =
C=                                                                     =
C=  Exemple d'utilisation d'un modele UMAT en grandes transformations  =
C=  Contribution de Laurent Gornet - Ecole Centrale de Nantes (2006)   =
C=                                                                     =
C=  Pour plus d'informations, voir la presentation de L. Gornet lors   =
C=  du Club Cast3m 2006, disponible sur le site Web de Cast3m.         =
C=                                                                     =
C= Note : Actuellement en grandes deformations dans PASAPAS, le modele =
C=        ne peut contenir que des modeles de type UMAT. On ne peut    =
C=        pas melanger les derivees objectives et les modeles de C3m.  =
C=======================================================================

      SUBROUTINE NEOHOO ( STRESS, STATEV, DDSDDE, STRAN, DSTRAN,
     &                    TIME, DTIME, TEMP, DTEMP, PREDEF, DPRED,
     &                    NDI, NSHR, NTENS, NSTATV, PROPS, NPROPS,
     &                    DFGRD0, DFGRD1, KSTEP, KINC )

      IMPLICIT INTEGER(I-N)
      IMPLICIT REAL*8(A-H,O-Z)

      INTEGER  NDI, NSHR, NTENS, NSTATV, NPROPS, KSTEP, KINC

      REAL*8   STRESS(NTENS), STATEV(NSTATV), DDSDDE(NTENS,NTENS),
     &         STRAN(NTENS), DSTRAN(NTENS), TIME(2), DTIME,
     &         TEMP, DTEMP, PREDEF(*), DPRED(*),
     &         PROPS(NPROPS), DFGRD0(3,3), DFGRD1(3,3)

      PARAMETER (cm5s3=-1.66666666666666666666666666666666666)

      REAL*8   Coe1, Coe2,
     &         F11,F12,F13,F21,F22,F23,F31,F32,F33, detF,
     &         CGg1,CGg2,CGg3,CGg4,CGg5,CGg6, phyd, eg

C  PARAMETRES MATERIAU :
C =======================
C= Dans le cas du modele NeoHookeen compressible,
C= l'energie de deformation est decomposee en deux termes decouples :
C= - la partie isochorique ou incompressible Wiso, fonction des inva-
C=   riants du tenseur de Cauchy-Green modifie ;
C= - la partie purement volumique Wvol, dependant de J=det(F).
C= Pour le present modele, nous avons :
C= - Wiso = Wiso(I1bar,I2bar) = Coe1 * (I1bar-3.)
C= - Wvol = Wvol(J) = 1/Coe2 * (J-1)*(J-1)
C= Coe1 et Coe2 : coefficients du materiau
C= le module de cisaillement est egal a mu = 2*Coe1
C= bbar : tenseur de Cauchy-Green gauche modifie
C= Par definition, bbar = J**(-2/3)*(F.Ft) = J**(-2/3)*CGg
C= I1bar : 1er invariant de bbar (= trace(bbar))
C= Dans le cas de la quasi-incompressibilite, c.a.d. J proche de 1,
C= Wvol peut etre interpretee comme une fonction de penalisation.
C*    Youn = PROPS(1)
C*    Pois = PROPS(2)
      Coe1 = PROPS(3)
      Coe2 = PROPS(4)

C  GRADIENT DE LA TRANSFORMATION (FIN DU PAS) :
C ==============================================
      F11 = DFGRD1(1,1)
      F12 = DFGRD1(1,2)
      F13 = DFGRD1(1,3)
      F21 = DFGRD1(2,1)
      F22 = DFGRD1(2,2)
      F23 = DFGRD1(2,3)
      F31 = DFGRD1(3,1)
      F32 = DFGRD1(3,2)
      F33 = DFGRD1(3,3)

C  JACOBIEN DE LA TRANSFORMATION (FIN DU PAS) :
C ==============================================
      detF =  F11*F22*F33 - F12*F21*F33 + F12*F23*F31
     &      + F13*F32*F21 - F13*F31*F22 - F23*F32*F11

C  TENSEUR DES DEFORMATIONS DE CAUCHY-GREEN GAUCHE
C =================================================
C= Tenseur de Cauchy-Green gauche CGg = F.Ft
      CGg1 = F11*F11 + F12*F12 + F13*F13
      CGg2 = F21*F21 + F22*F22 + F23*F23
      CGg3 = F33*F33 + F31*F31 + F32*F32
      CGg4 = F11*F21 + F12*F22 + F13*F23
      CGg5 = F11*F31 + F12*F32 + F13*F33
      CGg6 = F21*F31 + F22*F32 + F23*F33

C  CONTRAINTES DE CAUCHY (FIN DU PAS) :
C ======================================
C= Les contraintes de Cauchy STRESS se decomposent en deux termes :
C= - STRESS = SCvol + SCiso
C= - SCvol = phyd.Iden = (dWvol(J)/dJ).Iden
C=   avec Iden = tenseur identite d'ordre 2,
C=        phyd = pression hydrostatique,
C= - SCiso = 1/J.(Gam1.bbar+Gam2.bbar.bbar)
C=   avec Gam1 =  2.(dWiso/dI1bar+I1bar.dWiso/dI2bar)
C=        Gam2 = -2.(dWiso/dI2bar)
C= Dans le cas du modele NeoHookeen, nous avons :
C= - phyd = 2 * (J - 1.) / Coe2
C= - Gam1 = 2.Coe1  et  Gam2 = 0
C= - SCiso = 2.Coe1.1/J.J**(-2/3).CGg = 2.Coe1.J**(-5/3).CGg
      phyd = 2. * (detF-1.) / Coe2
      eg   = 2. * Coe1 * (detF**cm5s3)
      STRESS(1) = eg * CGg1 + phyd
      STRESS(2) = eg * CGg2 + phyd
      STRESS(3) = eg * CGg3 + phyd
      STRESS(4) = eg * CGg4
      STRESS(5) = eg * CGg5
      STRESS(6) = eg * CGg6

      RETURN
      END
