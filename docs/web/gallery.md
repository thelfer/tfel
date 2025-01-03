---
title: The `MFront` gallery
author: Thomas Helfer
date: 1/12/2016
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Mechanical behaviours

## Hyperelasticity

- [SaintVenantKirchhoffElasticity](./gallery/hyperelasticity/SaintVenantKirchhoffElasticity.mfront)
- [Signorini](signorini.html)
- [Ogden](ogden.html)

## Hyperviscoelasticity

We provide a general framework to build hyperviscoelastic
behaviour. This framework is described
[here](hyperviscoelasticity.html).

## Viscoelasticity

- [GeneralizedMaxwell](./gallery/viscoelasticity/GeneralizedMaxwell.mfront)
- [This page](sovs.html) describes how to implement the
  Skorohold-Olevsky Viscous Sintering (SOVS) model.
- [This page](LoadInducedThermalStrainBehaviourTorelli2018.html)
  describes how to implement Torelli's behaviour describing the load
  induced thermal strain in concrete.
- [This page](Burger_EDF_CIWAP_2021.html) describes the implementation
  of the `̀Burger_EDF_CIWAP_2021` which aims at modelling the following
  phenomena in concrete:
    - Elasticity
    - Drying shrinkage
    - Basic creep
    - Drying creep

## Non-linear elasticity

- [A non-linear elastic behaviour that mimics the Ramberg-Osgood plastic
  behaviour on monotonic
  loadings](RambergOsgoodNonLinearElasticity.html) (useful in fracture
  analysis

## Damage

- [Marigo](gallery/damage/Marigo.mfront)
- [Mazars](gallery/damage/Mazars.mfront)
- The implementation of the Fichant-La Borderie damage behaviour is
  described [here](FichantLaBorderieDamageBehaviour.html)

## Plasticity

- [IsotropicLinearHardeningPlasticity](IsotropicLinearHardeningPlasticity.html)
- [IsotropicLinearKinematicHardeningPlasticity](gallery/plasticity/IsotropicLinearKinematicHardeningPlasticity.mfront)
- The implementation of an orthotropic plastic behaviour with
  isotropic linear hardening is described
  [here](orthotropiclinearhardeningplasticity.html).
- The implementation of a multi-surface plastic behaviour (the Iwan
  behaviour) is described [here](iwan.html).
- The implementation of a perfect plastic behaviour based on the
  Hosford equivalent stress is described [here](hosford.html).
- The implementation of the Barlat Yld2004-18p behaviour is described
  [here](barlat-yld2004.html).
- The implementation of a perfect plastic behaviour based on the
  Green yield criterion is described [here](greenplasticity.html).
- The implementation of a multi-surface, compressible and perfect
  plastic behaviour using the Drucker-Prager yield criterion and a cap
  is described [here](drucker-prager-cap.html)
- An invariant-based implementation of the Mohr-Coulomb elasto-plastic
  model is described [here](MohrCoulomb.html)

## Viscoplasticity

- [Hayhurst](gallery/viscoplasticity/Hayhurst.mfront)
- [ChabocheViscoplasticity](gallery/viscoplasticity/ChabocheViscoplasticity.mfront)
- The implementation of an isotropic plastic behaviour with several
  kinematic variables following the Amstrong-Frederic evolution law is
  described
  [here](isotropicplasticityamstrongfrederickinematichardening.html)
- [ImplicitNorton](gallery/viscoplasticity/ImplicitNorton.mfront)

### Single crystal

- The implementation of the Méric-Cailletaud single crystal behaviour
  under the small and finite strain assumptions are described
  [here](MericCailletaudSingleCrystalPlasticity.html)

<!--
### PolyCrystal

- The description of the implementation of a polycrystal behaviour
  based on the Berveiller-Zaoui homogenisation scheme using an
  explicit scheme is available
  [here](ExplicitBerveillerZaouiPolyCrystals.html)
-->

<!-- Local IspellDict: english -->
