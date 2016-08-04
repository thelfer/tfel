% TFEL development
% Thomas Helfer
% 6 September 2014

![MFront logo](mfront.svg)

<!-- pandoc -s --mathjax -i -t revealjs TODO -o TODO.html -->
<!-- pandoc -t beamer -V theme:Warsow beamercea TODO -o TODO.pdf -->

# Stress free expansion

- stress free expansion for finite strain behaviour:
  - appears as dl0_l0 and dl1_l0

# Specific DSL

- support for external material properties

# Bricks

- finite strain single crystal plasticity
- Marigo and Lorentz damage models
- DDIF2 damage models

# Plane stress

- currently only supported by Implicit DSL
- extension to restricted domain specific languages
  (IsotropicMisesPlasticFlow, etc..)
    - reduction to a *scalar* equation

# Interfaces

- catch c++ exceptions
- catch c errno
- support of parameters modification through txt file

# Update documentation

Installation on windows without MSYS

# Ninja build system

- add a -G option allowing to choose a generator
- generate a ninja.build file

# Fallback

Fall back behaviour if the default one fails

# Mac Os

- let interfaces specify between modules and shared libraries, or undefined
- let the user choose whether he wants to build a bundle or a shared library

<!-- - matrice tangente cohérente pour l'analyseur MultipleIsotropicMisesFlows -->
<!-- - matrice de prédiction pour l'analyseur MultipleIsotropicMisesFlows -->
<!-- - matrice de prédiction pour l'analyseur IsotropicMisesPlasticFlow -->
<!-- - contraintes planes dans les analyseurs spécifiques -->
<!-- - implanter totalement l'algorithme de Powell -->
<!-- - implanter l'algorithme de Levenberg-Marquart -->

<!-- - support des lois non locales (plusieurs variables de pilotages) -->
<!-- - contrôle du pas de temps (non-radialité du chargement) -->
<!-- - pour la génération de fichiers mtest, gérer les calculs MPI -->

<!-- - optimiser les stratégies grandes transformations en ne faisant que -->
<!--   les étapes de pre et post-traitements nécessaires -->
<!--   (contraintes/déformations début de pas optionnelles) -->

<!-- - sortir l'étape de prédiction du constructeur vers l'intégration (non -->
<!--   nécessaire au calcul de l'opérateur de prédiction). -->

