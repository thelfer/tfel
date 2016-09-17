C MOONEY    SOURCE    CHAT      11/03/28    21:15:02     6923
C=======================================================================
C=        MODELE HYPERELASTIQUE MOONEY-RIVLIN INCOMPRESSIBLE           =
C=                 EN GRANDES TRANSFORMATIONS                          =
C=                                                                     =
C=    Modele actuellement disponible uniquement en CONTRAINTE PLANE    =
C=     (2D PLAN CONT ou 3D avec contraintes hors plan (1,2) nulles)    =
C=                                                                     =
C=  Exemple d'utilisation d'un modele UMAT en grandes transformations  =
C=  Contribution de Laurent Gornet - Ecole Centrale de Nantes (2006)   =
C=                                                                     =
C=  Pour plus d'informations, voir la presentation de L. Gornet lors   =
C=  du Club Cast3m 2006, disponible sur le site Web de Cast3m.         =
C=                                                                     =
C= Note : Actuellement en grandes deformations dans PASAPAS, le modele =
C=        ne peut contenir que des modèles type UMAT. On ne peut pas   =
C=        "melanger" les derivees objectives dans Cast3m.              =
C=======================================================================

      SUBROUTINE MOONEY (STRESS, STATEV, DDSDDE, STRAN, DSTRAN,
     &                   TIME, DTIME, TEMP, DTEMP, PREDEF, DPRED,
     &                   NDI, NSHR, NTENS, NSTATV, PROPS, NPROPS,
     &                   DFGRD0, DFGRD1, KSTEP, KINC,SSE,spd,scd,rpl,
     &                   DDSDDT,DRPLDE,DRPLDT,cmname,coords,drot,PNEWDT,
     &                   celent,NOEL, NPT, LAYER, KSPT )

      IMPLICIT INTEGER(I-N)
      IMPLICIT REAL*8(A-H,O-Z)
      CHARACTER*16  CMNAME

      INTEGER  NDI, NSHR, NTENS, NSTATV, NPROPS, KSTEP, KINC
     &         NOEL, NPT, LAYER, KSPT
      REAL*8   STRESS(NTENS), STATEV(NSTATV),SSE, SPD, SCD, RPL,
     &         DDSDDE(NTENS,NTENS), STRAN(NTENS), DSTRAN(NTENS),
     &         DDSDDT(NTENS), DRPLDE(NTENS), DRPLDT,
     &         TIME(2), DTIME, TEMP, DTEMP, PREDEF(*), DPRED(*),
     &         PROPS(NPROPS), DFGRD0(3,3), DFGRD1(3,3),
     &         COORDS(3),DROT(3,3),PNEWDT,CELENT
      REAL*8   CG11,CG22,CG12,CG33, CInv11,CInv22,CInv12, DLT,
     &         I1,I2, dWdI1,dWdI2, phyd, Coe1,Coe2,
     &         F11,F21,F12,F22, S11,S12,S21,S22
      REAL*8 F33,I1B,I2B,cinv33,detF,mdetF,Jm2s3,Jm1s3
      REAL*8 S33,dwvdj,C1,C2

      Parameter(zero=0.d0,one=1.d0,two=2.d0,three=3.d0,four=4.d0,
     & six=6.d0)
      PARAMETER (cm5s3=-1.66666666666666666666666666666666666)
      PARAMETER (cm7s3=-2.33333333333333333333333333333333333)
      PARAMETER (cm2s3=-0.66666666666666666666666666666666666)
      PARAMETER (cm1s3=-0.33333333333333333333333333333333333)
      PARAMETER (cm4s3=-1.33333333333333333333333333333333333)
      if(ndi.ne.-2) go to 100
* formulation 2D contraintes planes
C  PARAMETRES MATERIAU DU MODELE MOONEY-RIVLIN :
C ===============================================
C= Dans le cas du modele de Mooney-Rivlin,
C= la densite d'energie de deformation est definie par :
C=    W = Coe1 * (I1-3.) + Coe2 * (I2-3.)
C= et le module de cisaillement est donne par :
C=    mu = 2*(Coe1+Coe2)
C= Coe1 et Coe2 : coefficients du materiau
C= I1, I2 et I3 : trois invariants du tenseur de Cauchy-Green droit
C= Incompressibilite ==> I3 = 1
C= On a alors : dW/dI1 = Coe1 et dW/dI2 = Coe2
C=              mu = 2*(Coe1+Coe2) = YOUN/3. (car NU proche de 0.5)
      Coe1 = PROPS(3)
      Coe2 = PROPS(4)
      dWdI1 = Coe1
      dWdI2 = Coe2

C  GRADIENT DE LA TRANSFORMATION (FIN DU PAS) :
C ==============================================
C= Les composantes F31, F32, F13, F23 sont nulles.
C= La composante F33 est non nulle.
      F11 = DFGRD1(1,1)
      F12 = DFGRD1(1,2)
      F21 = DFGRD1(2,1)
      F22 = DFGRD1(2,2)
*      write(6,*) ' f11 f12 f21 f22',f11, f12, f21, f22
C*    F33 = DFGRD1(3,3)

C  TENSEUR DE CAUCHY-GREEN DROIT (FIN DU PAS) :
C ==============================================
C= CG   : tenseur des dilatations de Cauchy-Green droit et CG = Ft*F
C= CG33 : composante dans l'epaisseur de la membrane, calculee par la
C=        conservation du volume (incompressibilite) : I3 = det(CG) = 1
      CG11 = F11*F11 + F21*F21
      CG22 = F12*F12 + F22*F22
      CG12 = F11*F12 + F21*F22
C*    CG33 = F33*F33
      DLT = CG11*CG22 - CG12*CG12
      CG33 = 1.D0 / DLT

C  INVARIANTS DU TENSEUR DE CAUCHY-GREEN DROIT :
C ===============================================
C= I1 : 1er invariant et I1 = trace(CG)
C= I2 : 2e invariant (inutilise ici)
C= I3 : 3e invariant (I3 = det(CG) = 1)
      I1 = CG11 + CG22 + CG33
C*    I2 = 0.5D0*(I1*I1-CG11*CG11-CG22*CG22-CG33*GC33) - CG12*CG12

C  INVERSE DU TENSEUR DE CAUCHY-GREEN DROIT :
C ============================================
      CInv11 =  CG22/DLT
      CInv22 =  CG11/DLT
      CInv12 = -CG12/DLT

C  CONTRAINTES DE PIOLA-KIRCHHOFF DE SECONDE ESPECE :
C ====================================================
C= S ou PK2 : second tenseur des contraintes de Piola-Kirchhoff
C= S = PK2 = -phyd.Inv(CG) + 2.(dW/dI1 + I1.dW/dI2).Iden - 2.dW/dI2.CG
C= Iden : tenseur identite d'ordre 2
C= phyd : pression hydrostatique
C= La pression hydrostatique est calculee a partir de la condition de
C= contrainte planes soit S33 = 0.
      zz = dWdI1 + I1*dWdI2
      phyd = 2.D0*(zz-dWdI2*CG33) * CG33
      S11 =  2.D0*(zz-dWdI2*CG11) - phyd*CInv11
      S22 =  2.D0*(zz-dWdI2*CG22) - phyd*CInv22
      S12 = -2.D0*(   dWdI2*CG12) - phyd*CInv12

C  CONTRAINTES DE CAUCHY (FIN DU PAS) :
C ======================================
C= STRESS : Contraintes de Cauchy = (det(F)-1) F S Ft
C= L'hypothese d'incompressibilite se traduit par det(F)=1.
      STRESS(1) = (S11*F11*F11) + (2.D0*F11*F12*S12) + (F12*F12*S22)
      STRESS(2) = (S11*F21*F21) + (2.D0*F21*F22*S12) + (F22*F22*S22)
C*    STRESS(3) = 0.
      STRESS(4) =  (F21*F11*S11) + (F21*F12*S12) + (F22*F11*S12)
     &           + (F22*F12*S22)
*      write(6,*) 'stres',STRESS(1),STRESS(2),STRESS(4)
      RETURN
 100  CONTINUE
*  formulation 3D massive
      if(ndi.ne.2) go to 200
C  PARAMETRES MATERIAU :
C =======================
C= Dans le cas du modele Mooney Rivlin compressible,
C= l'energie de deformation est decomposee en deux termes decouples :
C= - la partie isochorique ou incompressible Wiso, fonction des inva-
C=   riants du tenseur de Cauchy-Green modifie ;
C= - la partie purement volumique Wvol, dependant de J=det(F).
C= Pour le present modele, nous avons :
C= - Wiso = Wiso(I1bar,I2bar) = Coe2 * (I1bar-3.) + Coe3 * (I2bar-3.)
C= - Wvol = Wvol(J) = 1/Coe1 * (J-1)*(J-1)
C= Coe2 et Coe3 : coefficients du materiau
C= le module de cisaillement est egal a mu = 2*Coe2
C= bbar : tenseur de Cauchy-Green gauche modifie
C= Par definition, bbar = J**(-2/3)*(F.Ft) = J**(-2/3)*CGg
C= I1bar : 1er invariant de bbar (= trace(bbar))
C= Dans le cas de la quasi-incompressibilite, c.a.d. J proche de 1,
C= Wvol peut etre interpretee comme une fonction de penalisation.
C*    Youn = PROPS(1)
C*    Pois = PROPS(2)
      Coe1 = PROPS(3)
      Coe2 = PROPS(4)
      Coe3 = PROPS(5)

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
C G11 G22 G33  G12 G13 G23
      CGg1 = F11*F11 + F12*F12 + F13*F13
      CGg2 = F21*F21 + F22*F22 + F23*F23
      CGg3 = F33*F33 + F31*F31 + F32*F32
      CGg4 = F11*F21 + F12*F22 + F13*F23
      CGg5 = F11*F31 + F12*F32 + F13*F33
      CGg6 = F21*F31 + F22*F32 + F23*F33
C= Tenseur de Cauchy-Green gauche CGg2 = (F.Ft)*(F.Ft)
C indices (1 a 6) =(11 22 33  12 13 23
      C2Gg1 = CGg1*CGg1 + CGg4*CGg4 + CGg5*CGg5
      C2Gg2 = CGg4*CGg4 + CGg2*CGg2 + CGg6*CGg6
      C2Gg3 = CGg5*CGg5 + CGg6*CGg6 + CGg3*CGg3
      C2Gg4 = CGg1*CGg4  + CGg4*CGg2 + CGg5*CGg6
      C2Gg5 = CGg1*CGg5 + CGg4*CGg6 + CGg5*CGg3
      C2Gg6 = CGg4*CGg5 + CGg2*CGg6 + CGg6*CGg3
C INVARIANTS : I1B= J**-1/3 I1
      I1B = (detF**cm2s3)*(CGg1+CGg2+CGg3)
C  CONTRAINTES DE CAUCHY (FIN DU PAS) :
C ======================================
C= Les contraintes de Cauchy STRESS se decomposent en deux termes :
C= - STRESS = SCvol + SCiso
C= - SCvol = phyd.Iden = (dWvol(J)/dJ).Iden
C=   avec Iden = tenseur identite d'ordre 2,
C=        phyd = pression hydrostatique,
C=   avec Gam1 =   (dWiso/dI1bar+I1bar.dWiso/dI2bar)
C=        Gam2 =  (dWiso/dI2bar)
C= - SCiso =  2. J**(-5/3)Gam1 CGg - 2. J**(-7/3) Gam2 C2Gg
C
C ENTRER LE MATERIAU ICI :
      dWisodI1bar = Coe2
      dWisodI2bar = Coe3

      Gam1 =  (dWisodI1bar+I1B*dWisodI2bar)
      Gam2 =  (dWisodI2bar)

      phyd = 2. * (detF-1.0D0) / Coe1

      eg1   = 2. * (detF**cm5s3) * Gam1
      eg2   = 2. * (detF**cm7s3) * Gam2


      STRESS(1) = eg1 * CGg1 - eg2 * C2Gg1 + phyd
      STRESS(2) = eg1 * CGg2 - eg2 * C2Gg2 + phyd
      STRESS(3) = eg1 * CGg3 - eg2 * C2Gg3 + phyd
      STRESS(4) = eg1 * CGg4 - eg2 * C2Gg4
      STRESS(5) = eg1 * CGg5 - eg2 * C2Gg5
      STRESS(6) = eg1 * CGg6 - eg2 * C2Gg6
      return
200   if(ndi.ne.-1) go to 300
         C1  = PROPS(3)
         C2  = PROPS(4)
         Coe1 = PROPS(5)
C Deformation totale stockage perso dans var 4 5 6
C ******************** GREEN LAGRANGE **************
C
       F11 = dfgrd1(1,1)
       F21 = dfgrd1(2,1)
       F12 = dfgrd1(1,2)
       F22 = dfgrd1(2,2)
       F33 = dfgrd1(3,3)
       detF =F11*F22-F12*F21
C ******************** CC = FT * F
C
        cc11 =  F11**2 + F21**2
        cc22 =  F12**2 + F22**2
        cc12 =  (F11*F12)+(F21*F22)
C ******************** CC-1
           DLT = CC11*CC22 - CC12*CC12
               cinv11 =  1.0D0/DLT*CC22
           cinv22 =  1.0D0/DLT*CC11
           cinv12 = -1.0D0/DLT*CC12
           cinv33 =  1.0D0/DLT
C
C***********************************************************
C det J =1  ==>c
            cc33=1.0D0
C trace C  et CB
            i1= cc11+cc22+cc33
            i2= .50D0*(i1**2-cc11**2-cc22**2-cc33**2-2*cc12**2)
C
            I1B = (detF**cm2s3)*I1
            I2B = (detF**cm4s3)*I2
C===============================================================
C 2 INTEGRER VOTRE MATERIAU MOONEY : DWDI1B = ?, DWDI2B = ?
C attention nom !!!  dwdi1 =  DWDI1B et dwdi2 = DWDI2B
C              Wiso = f(i1b,i2b)
C
               dwdi1 = C1
                   dwdi2 = C2
                   dwvdj=  2. * (detF-1.0D0) / Coe1
C===============================================================
                   Jm2s3=detF**cm2s3
                   Jm1s3=detF**cm1s3
                   phyd = Jm1s3*dwvdj
C===============================================================
C p : pression hydrostatique obtenu ici a partir de S33 = 0.
            p=2.0D0*((dwdi1+i1*dwdi2)-dwdi2*cc33)*cc33
C******************** CONTRAINTES PK2    ******************
C
C Epaisseur a ajouter dans les formules!!!
C234567
       S11=2.0D0*Jm2s3*((dwdi1+i1b*dwdi2)-dwdi2*cc11)+phyd*cinv11
       S22=2.0D0*Jm2s3*((dwdi1+i1b*dwdi2)-dwdi2*cc22)+phyd*cinv22
       S12=  2.0D0*Jm2s3*(-dwdi2*cc12)+phyd*cinv12
       S33 =2.0D0*Jm2s3*((dwdi1+i1b*dwdi2)-dwdi2*cc33)+phyd*cinv33
C
C
C******************** CONTRAINTES Cauchy     ******************
C A FAIRE PK2 a transformer par  cauchy  = j-1  F S FT
C ici incompressible j =1 a modifier par F et FT,
C***************************************************************
      mdetF = 1.0D0/detF
C
      stress(1)=mdetF*((S11*F11**2)+(2.0D0*F11*F12*S12)+(F12**2*S22))
      stress(2)=mdetF*((S11*F21**2)+(2.0D0*F21*F22*S12)+(F22**2*S22))
      stress(4)=mdetF*((F21*F11*S11)+(F21*F12*S12)+(F22*F11*S12)
     . +(F22*F12*S22))
      stress(3)= mdetF*S33
      return
300   kinc=-2
      return
      END
