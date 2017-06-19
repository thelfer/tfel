% Getting started with `MFront`
% Thomas Helfer
% 16/06/2017

This document is meant to help new users to find their ways in the
documentation of `MFront`.

> **Material properties and models**
> 
> As most users are focused on mechanical behaviours, this document
> will let aside the *material properties* and *models*.

`MFront` has been designed to help the user writting complex
mechanical behaviours using a domain specific language that are:

- numerically efficient.
- portable from one finite element to another.
- easy to read (by your colleagues) and maintain.

Before writing your own behaviours, a few advices are helpful:

- You must have some knowledge of this behaviour and of its physical
  meaning. If you are new to finite strain single crystal plasticity,
  do not try to directly analyze the examples found in the `MFront`
  tests base.
- You must develop your behaviour step by step. Do not neglect unit
  testing. This is what `MTest` has been made for.
- you must have some knowledge about your finite element solver, in
  particular when using "advanced" features (finite strain, non local
  models).

The final advice is the more important: as every open-source project,
`MFront` has its community of users.

## Installation

The installation of 

<!-- Local IspellDict: english -->
