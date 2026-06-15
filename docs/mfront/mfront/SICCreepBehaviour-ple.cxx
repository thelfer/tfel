/*
 * \file   pleiades/SiCCreep/SiCCreep.ple
 * \brief  example of how to use the SiCCreep behaviour law
 * in the celaeno fuel performanece code
 * \author Helfer Thomas
 * \date   06 / 12 / 07
 */

MechanicalBehaviour 'IsotropicUmatBehaviour' 'libUmatBehaviour.so' 'umatSiCCreep' MaterialProperty<constant> 'YoungModulus' 150.e9 MaterialProperty<constant> 'PoissonRatio' 0.3 MaterialProperty<
    constant> 'ThermalExpansion' 0. MaterialProperty<constant> 'MassDensity' 0. InternalStateVariable<Stensor> 'ElasticStrain' InternalStateVariable<Scalar> 'EquivalentViscoplasticStrain' EndOfMechanicalBehaviour
    EndOfMaterial
