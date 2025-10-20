---
title: Implementation of an isotropic pressure-dependent plastic behaviour
author: Thomas Helfer
date: 15/10/2025
lang: en-EN
link-citations: true
colorlinks: true
geometry:
  - margin=2cm
papersize: a4
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonvis}{\tenseur{\varepsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonvis}{\tenseur{\dot{\varepsilon}}^{\mathrm{vis}}}
\newcommand{\tepsilonp}{\tenseur{\varepsilon}^{\mathrm{p}}}
\newcommand{\tdepsilonp}{\tenseur{\dot{\varepsilon}}^{\mathrm{p}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\sigmaH}{\sigma_{H}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

This paper is dedicated to the implementation a perfect plastic
behaviour based on an equivalent stress function of the following
invariants: the hydrostatic pressure and the von Mises stress.

# Description

## Elasticity

The elasticity is assumed linear and isotropic, i.e. given by the
Hooke law:
\[
\tsigma=\lambda\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\,\tepsilonel 
\]
where \(\lambda\) and \(\mu\) are the first and second Lamé parameters.

## Plasticity

The yield surface is chosen as follows:
\[
f\paren{\tsigma}=\sigmaeq^{\star}\paren{p_{r}, \sigmaeq}-\sigma_{Y}
\]
where:

- \(\sigmaeq^{\star}\) is the equivalent stress,
- \(p_{r}\) is the hydrostatic stress, defined by \(p_{r}=\Frac{1}{3}\trace{\tsigma}\),
- \(\sigmaeq\) is the von Mises stress, defined by
  \[
  \sigmaeq=\sqrt{\Frac{3}{2}\tenseur{s}\,\colon\,\tenseur{s}}=\sqrt{\tsigma\,\colon\,\tenseur{M}\,\colon\tsigma}
  \]
  where \(\tenseur{s}=\tsigma-p\,\tenseur{I}\) is the deviatoric stress 
  and \(\tenseurq{M}=\Frac{3}{2}\left(\tenseurq{I}-\tenseur{I}\,\otimes\,\tenseur{I}\right)\),
- The yield stress \(\sigma_{Y}\) is a constant material parameter.

Plasticity is assumed associated, the flow direction \(n_{f}\) is given
by the normality rule:
\[
\begin{aligned}
n_{f}&=\deriv{f}{\tsigma}&=\deriv{f}{\sigmaeq}\,\deriv{\sigmaeq}{\tsigma}+\deriv{f}{p_{r}}\,\deriv{p_{r}}{\tsigma}\\
&&=\deriv{f}{\sigmaeq}\,\tenseur{n}+\frac{1}{3}\,\deriv{f}{p_{r}}\,\tenseur{I}\\
\end{aligned}
\]
where \(\tenseur{n}\) is the von Mises normal:
\[
\tenseur{n}=\frac{3}{2\,\sigmaeq}\tenseur{s}=\frac{1}{\sigmaeq}\,\tenseurq{M}\,\colon\,\tsigma
\]