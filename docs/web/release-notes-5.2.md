---
title: Release notes of the 5.2 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer, Antoine Martin
date: 2025
lang: en-EN
numbersections: true
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}


The page describes the new functionalities of Version 5.2 of the
`TFEL` project.

# New features in `tfel-check`

## Parallelization

The commands described in different `.check` files can now be run in
distinct processes.

Note that the command described in a `.check` are still run
sequentially.

By default, as in previous versions, only one process is used.

### Specifying the number of jobs run simultaneously

The command line argument `--jobs` (`-j`) allow specifying the number
of jobs run simultaneously.

In no option option is given to `--jobs`, the number of cores
available is used, if the system provide this information.

#### Limit on the number of jobs

By default, this number must be lower than the number of cores
available. This limit can be discarded by
passing`--discard-jobs-limit=true` to `tfel-check`.

### Caution

The user must take care that the commands described in `.check` files
can be run in parallel.

- the jobs shall no exhaust memory.
- the jobs shall not access concurrently to the same resources.

A typical example of conflict would be to have two `.check` files in the
same directory that call `mfront` to build somes shared libraries: the
two `MFront`'s processes may write concurrently files in the `src` and
`include` directories, leading to an undefined behaviour.

Another example would be to use `@CleanDirectories` and `@CleanFiles`
commands inappropriately and erase data used by commands of other
`.check` files.

##  New features in `TFEL/System`

### Improvements in `ProcessManager`

- New overloads of the `createProcess` and `execute` methods allow
  specifying the directory where a command shall executed.
- `ProcessManager` shall now work properly in a multithreaded context.

### Improvements in `SignalManager`

- `SignalManager` shall now work properly in a multithreaded context.

# Acknowledgements

The authors are grateful to the many contributors to the `TFEL/MFront`
project. This research was conducted in the framework of the PLEIADES
project, which was supported financially by the CEA (Commissariat à
l’Énergie Atomique et aux Énergies Alternatives), EDF (Électricité de
France) and Framatome. Homogenization developments were conducted within
the framework of the AnoHonA ANR project (n° AAPG2023).

# Issues fixed
