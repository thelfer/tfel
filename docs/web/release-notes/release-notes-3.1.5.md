% Release notes of the 3.1.5 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

This version was released along with Version `3.0.5` and inherits from
all the fixes of this version.

# Known issues

Following the resolution of Ticket #185, the number of material
properties of behaviours based on the `DDIF2Brick` has changed for the
`AxisymmetricalGeneralisedPlaneStrain` modelling hypothesis (the
`AngularCoordinate` material property si no more needed).

# Tickets fixed

## Ticket #191: The interaction matrix aimed at describing the effets of hardening of the mean free path of dislocation is not correctly handled by the `SlipSystemsDescription` class

For more details, see: <https://sourceforge.net/p/tfel/tickets/191/>

## Ticket #190: The `FiniteStrainSingleCrystal` brick does not enforce that the plastic slips must be the second integraiton variables

For more details, see: <https://sourceforge.net/p/tfel/tickets/190/>

## Ticket #185: The `AngularCoordinate` material property shall not be defined in 1D by the DDIF2 brick

For more details, see: <https://sourceforge.net/p/tfel/tickets/185/>

## Ticket #183: The behaviour kinematic defined by the `abaqus` interface is wrong when using a strain measure**

For more details, see: <https://sourceforge.net/p/tfel/tickets/183/>
