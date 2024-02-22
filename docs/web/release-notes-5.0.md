---
title: Release notes of the 5.0 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer, Maxence Wangermez
date: 2020
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

# New `TFEL/Math` features

## tiny matrices product

The product of two tiny matrices has been implemented:

~~~~{.cxx}
const auto m1 = tmatrix<2u, 2u, int>{0, 1,  //
                                     2, 3};
const auto m2 = tmatrix<2u, 2u, int>{4, -1,  //
                                     5, 2};
const auto m3 = m1 * m2;
~~~~

## Symmetric tensor eigen values and eigen vectors

- New eigen solvers have been introduced.

### New eigen solvers{#sec:eigensolvers}

New eigen solver based on Harari analytical solution have been introduced for symmetric tensors. The computation of eigen values is done with the Harai algorithm [@harari_computation_2023] and the computation of eigen vectors is done with the default eigen solver for symmetric tensors of `TFEL`. Such computations are more efficient but more accurate than the default `TFEL` algorithm.

Those algorithms are available in 3D. For 2D symmetric tensors, we
fall back to some default algorithm as described below.

|            Name             |     Algorithm  in 3D      | Algorithm  in 2D  |
| :-------------------------: | :-----------------------: | :---------------: |
|      `TFELEIGENSOLVER`      |     Analytical (TFEL)     | Analytical (TFEL) |
|   `FSESJACOBIEIGENSOLVER`   |          Jacobi           | Analytical (FSES) |
|     `FSESQLEIGENSOLVER`     |  QL with implicit shifts  | Analytical (FSES) |
|   `FSESCUPPENEIGENSOLVER`   | Cuppen's Divide & Conquer | Analytical (FSES) |
| `FSESANALYTICALEIGENSOLVER` |        Analytical         | Analytical (FSES) |
|   `FSESHYBRIDEIGENSOLVER`   |          Hybrid           | Analytical (FSES) |
| `GTESYMMETRICQREIGENSOLVER` |       Symmetric QR        | Analytical (TFEL) |
|     `HARARIEIGENSOLVER`     |    Analytical (Harari)    | Analytical (TFEL) |
: List of available eigen solvers. {#tbl:eigensolvers}

The various eigen solvers available are enumerated in Table
@tbl:eigensolvers.

The eigen solver is passed as a template argument of the
`computeEigenValues` or the `computeEigenVectors` methods as
illustrated in the code below:

~~~~~{.cpp}
tmatrix<3u,3u,real> m2;
tvector<3u,real>    vp2;
std::tie(vp,m)=s.computeEigenVectors<stensor::HARARIEIGENSOLVER>();
~~~~~

#### Some benchmarks

|          Algorithm          | Failure ratio | \(\Delta_{\infty}\) | Times (ns) | Time ratio |
| :-------------------------: | :-----------: | :-----------------: | :--------: | :--------: |
|      `TFELEIGENSOLVER`      |   0.000642    |      3.29e-05       | 250174564  |     1      |
| `GTESYMMETRICQREIGENSOLVER` |       0       |      1.10e-06       | 359854550  |    1.44    |
|   `FSESJACOBIEIGENSOLVER`   |       0       |      5.62e-07       | 473263841  |    1.89    |
|     `FSESQLEIGENSOLVER`     |   0.000397    |      1.69e-06       | 259080052  |    1.04    |
|   `FSESCUPPENEIGENSOLVER`   |   0.019469    |      3.49e-06       | 274547371  |    1.10    |
|   `FSESHYBRIDEIGENSOLVER`   |   0.076451    |      5.56e-03       | 126689850  |    0.51    |
| `FSESANALYTICALEIGENSOLVER` |   0.108877    |      1.58e-01       | 127236908  |    0.51    |
|     `HARARIEIGENSOLVER`     |   0.000018    |      4.04e-06       | 167626780  |    0.67    |
: Test on \(10^{6}\) random symmetric tensors in single precision (`float`). {#tbl:comp_eigensolvers_float}

|          Algorithm          | Failure ratio | \(\Delta_{\infty}\) | Times (ns) | Time ratio |
| :-------------------------: | :-----------: | :-----------------: | :--------: | :--------: |
|      `TFELEIGENSOLVER`      |   0.000632    |      7.75e-14       | 252663338  |     1      |
| `GTESYMMETRICQREIGENSOLVER` |       0       |      2.06e-15       | 525845499  |    2.08    |
|   `FSESJACOBIEIGENSOLVER`   |       0       |      1.05e-15       | 489507133  |    1.94    |
|     `FSESQLEIGENSOLVER`     |   0.000422    |      3.30e-15       | 367599140  |    1.45    |
|   `FSESCUPPENEIGENSOLVER`   |   0.020174    |      5.79e-15       | 374190684  |    1.48    |
|   `FSESHYBRIDEIGENSOLVER`   |   0.090065    |      3.53e-10       | 154911762  |    0.61    |
| `FSESANALYTICALEIGENSOLVER` |   0.110399    |      1.09e-09       | 157613994  |    0.62    |
|     `HARARIEIGENSOLVER`     |   0.000022    |      8.90e-15       | 175109610  |    0.69    |
: Test on \(10^{6}\) random symmetric tensors in double precision (`double`). {#tbl:comp_eigensolvers_double}

|          Algorithm          | Failure ratio | \(\Delta_{\infty}\) | Times (ns) | Time ratio |
| :-------------------------: | :-----------: | :-----------------: | :--------: | :--------: |
|      `TFELEIGENSOLVER`      |   0.000575    |      2.06e-17       | 428333721  |     1      |
| `GTESYMMETRICQREIGENSOLVER` |       0       |      1.00e-18       | 814990447  |    1.90    |
|   `FSESJACOBIEIGENSOLVER`   |       0       |      5.11e-19       | 748476926  |    1.75    |
|     `FSESQLEIGENSOLVER`     |    0.00045    |      1.83e-18       | 548604588  |    1.28    |
|   `FSESCUPPENEIGENSOLVER`   |   0.009134    |      3.23e-18       | 734707748  |    1.71    |
|   `FSESHYBRIDEIGENSOLVER`   |    0.99959    |      4.01e-10       | 272701749  |    0.64    |
| `FSESANALYTICALEIGENSOLVER` |   0.999669    |      1.36e-11       | 315243286  |    0.74    |
|     `HARARIEIGENSOLVER`     |   0.000044    |      9.49e-18       | 459905551  |    1.07    |
: Test on \(10^{6}\) random symmetric tensors in extended precision (`long double`). {#tbl:comp_eigensolvers_long_double}


We have compared the available algorithm on \(10^{6}\) random
symmetric tensors whose components are in \([-1:1]\).

For a given symmetric tensor, we consider that the computation of the
eigenvalues and eigenvectors failed if:
\[
\Delta_{\infty}=\max_{i\in[1,2,3]}\left\|\tenseur{s}\,\cdot\,\vec{v}_{i}-\lambda_{i}\,\vec{v}_{i}\right\|>10\,\varepsilon
\]
where \(\varepsilon\) is the accuracy of the floatting point considered.

The results of those tests are reported on Tables
@tbl:comp_eigensolvers_float, @tbl:comp_eigensolvers_double and
@tbl:comp_eigensolvers_long_double:

- The Harari eigen solver offers a better compromise between accuracy and numerical efficiency than the default `TFEL` solver.

# Issues fixed

## Issue 526: The `@UseQt` keyword is not mentioned in the `MaterialLaw`'s keywords help page

For more details, see <https://github.com/thelfer/tfel/issues/526>


