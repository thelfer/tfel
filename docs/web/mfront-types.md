---
title: Predefined aliases in `MFront`
author: Thomas Helfer
date: 17/08/2021
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This page described the type aliases automatically defined in `MFront`.

If quantities are not used, all scalar types are a simple alias to the
numeric type used. This statement can obviously be generalised to the
other mathematical objects.

# Scalar types

- `real`: the numeric type used.
- `time`: a scalar with the dimension of a time.
- `frequency`: a scalar with the dimension of a frequency.
- `length`: a scalar with the dimension of a lenght.
- `inv_length`: a scalar with the dimension of the inverse of a length.
- `displacement`: a scalar with the dimension of a lenght.
- `strain`: a scalar without dimension.
- `strainrate`:  a scalar with the dimension of a frequency.
- `force`: a scalar with the dimension of a force.
- `stress`: a scalar with the dimension of a stress.
- `stressrate`: a scalar with the dimension of a stress divided by a
  time.
- `temperature`:  a scalar with the dimension of a temperature.
- `thermalexpansion`: a scalar representing a thermal expansion
  coefficient with the dimension of the inverse of a temperature.
- `massdensity`: a scalar with the dimension of a mass divided by a
  volume.
- `energydensity`: a scalar with the dimension of an energy divided by a
  volume.
- `speed`: a scalar with the dimension of an length divided by a
  time.
- `thermalconductivity`: a scalar with the dimension of power divided by
  a length, a time and a temperature. In the International System of
  Units, the thermalconductiviy is measured in watts per meter-kelvin
  (\(W/(m\,K)\)).

# Vector types

- `TVector`: 
- `ForceTVector`: 
- `DisplacementTVector`:
- `HeatFlux`:

# Matrix types

- `ThermalConductivityMatrix`:

# Symmetric tensor types

- `Stensor`: 
- `FrequencyStensor`: 
- `StressStensor`: 
- `StressRateStensor`: 
- `StrainStensor`: 
- `StrainRateStensor`: 
- `ThermalExpansionCoefficientTensor`: 

# Tensor types

- `Tensor`: 
- `FrequencyTensor`: 
- `StressTensor`: 
- `DeformationGradientTensor`: 
- `DeformationGradientRateTensor`: 

# Fourth order tensor types of the `st2tost2` type

- `Stensor4`: 
- `StiffnessTensor`: 
