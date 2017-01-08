% How to implement an hyperelastic behaviour in MFront
% Thomas Helfer
% 15/12/2016

 <!-- pandoc -f markdown-markdown_in_html_blocks+tex_math_single_backslash --mathjax -s  hyperelasticity.md  -o hyperelasticity.pdf -->

\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}

\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}

\newcommand{\C}{\tenseur{C}}
\newcommand{\S}{\tenseur{S}}
\newcommand{\iJb}{\bar{c}_{I_{3}}}
\newcommand{\diJb}{\deriv{\iJb}{I_{3}}}
\newcommand{\sdiJb}{\sderiv{\iJb}{I_{3}}}

This article shows how to implement various hyperelastic behaviours in
`MFront`.

# A first class of hyperelastic behaviours

In this section, we consider an isotropic hyperelastic behaviour whose
potential \(W\) is written in terms of the invariants of the right
Cauchy tensor \(\C\):

\[
W\paren{\C}=W\paren{I_{1},I_{2},I_{3}}
\]

The three invariants are defined by:
\[
\left\{
\begin{aligned}
I_{1} &= \trace{C} \\
I_{2} &= \Frac{1}{2}\left(\paren{\trace{\C}}^{2}-\trace{\C^{2}}\right) \\
I_{3} &= \det\paren{\C} = J^{2}
\end{aligned}
\right.
\]

where \(J\) is the jacobian of the deformation gradient

## Computation of the second Piola-Kirchhoff stress

The second Piola-Kirchhoff stress \(\S\) is then defined by:

\[
\S=2\deriv{W}{\C}=2\,\deriv{W}{I_{1}}\,\deriv{I_{1}}{\C}+2\,\deriv{W}{I_{2}}\,\deriv{I_{2}}{\C}+2\,\deriv{W}{I_{3}}\,\deriv{I_{3}}{\C}
\]

The derivative of the invariants are classically given by:
\[
\left\{
\begin{aligned}
\deriv{I_{1}}{\C} &= \tenseur{I}\\
\deriv{I_{2}}{\C} &= I_{1}\,\tenseur{I}-\C\\
\deriv{I_{3}}{\C} &= C^{2}-I_{1}\,C-I_{2} \\
\end{aligned}
\right.
\]

## Computation of consistent tangent operator

The second derivative of the invariants are given by:
\[
\left\{
\begin{aligned}
\sderiv{I_{1}}{\C} &= 0\\
\sderiv{I_{2}}{\C} &= \tenseur{I}\otimes\tenseur{I}-\tenseurq{I}\\
\end{aligned}
\right.
\]

In `MFront`, the fourth order tensors
\(\tenseur{I}\otimes\tenseur{I}\) and \(\tenseurq{I}\) are computed by
`Stensor4::IxI()` and `Stensor4::Id()` respectively.

The \(\sderiv{I_{3}}{\C}\) term can be computed using the
`computeJacobianSecondDerivative` function.

# A second class of hyperelastic behaviours

More precisely, we consider an hyperelastic behaviour whose
potential \(W\) is decomposed in an volumetric part \(W^{\text{v}}\)
and an isochoric part \(W^{\text{i}}\):

\[
W\paren{\C}=W^{v}\paren{J}+W^{i}\paren{\bar{I}_{1},\bar{I}_{2}}
\]

where \(\bar{I}_{1}\) and \(\bar{I}_{2}\) are the first and second
invariant of the isochoric right Cauchy tensor \(\tenseur{\bar{C}}\)
defined by:

\[
\tenseur{\bar{C}}=J^{-2/3}\,\C = I_{3}^{-1/3}\,\C = \iJb\,\C
\]

with \(\iJb=I_{3}^{-1/3}\)

## Derivatives of the invariants of the isochoric right Cauchy tensor

\(\bar{I}_{1}\) and \(\bar{I}_{2}\) are related to the first and
second invariants of the right Cauchy tensor by:

\[
\begin{aligned}
  \bar{I}_{1} &= I_{3}^{-1/3}\,I_{1} = \iJb\,I_{1}\\
  \bar{I}_{2} &= I_{3}^{-2/3}\,I_{2} = \iJb^{2}\,I_{2}\\
\end{aligned}
\]

The following derivatives will be useful in the following:
\[
\left\{
\begin{aligned}
	\deriv{\iJb}{\C}  &= \diJb\,\deriv{I_{3}}{\C} =-\frac{1}{3}\,\iJb^{4}\,\deriv{I_{3}}{\C} \\
	\sderiv{\iJb}{\C} &= \frac{4}{9}\,\iJb^{7}\deriv{I_{3}}{\C}\otimes\deriv{I_{3}}{\C}-\frac{1}{3}\,\iJb^{4}\,\sderiv{I_{3}}{\C} \\
\end{aligned}
\right.
\]

\(\deriv{\bar{I}_{1}}{\C}\) is given by:
\[
\deriv{\bar{I}_{1}}{\C}=\iJb\tenseur{I}+\,I_{1}\,\deriv{iJb}{\C}
\]

\(\deriv{\bar{I}_{2}}{\C}\) is given by:
\[
\deriv{\bar{I}_{2}}{\C}=\iJb^{2}\,\deriv{I_{2}}{\C}+2\,I_{2}\,\iJb\,\deriv{\iJb}{\C}
\]

The second derivative of the first invariant of the isochoric right
Cauchy tensor is:

\[
\sderiv{\bar{I}_{1}}{\C}=
\tenseur{I}\otimes\deriv{\iJb}{\C}+
\deriv{\iJb}{\C}\otimes\tenseur{I}+
I_{1}\,\sderiv{\iJb}{\C}
\]

The second derivative of the second invariant of the isochoric right
Cauchy tensor is:

\[
\begin{aligned}
\sderiv{\bar{I}_{2}}{\C}&=
2\,\iJb\,\deriv{I_{2}}{\C}\otimes\deriv{\iJb}{\C}+
\iJb^{2}\,\sderiv{I_{2}}{\C}+
2\,\iJb\,\deriv{\iJb}{\C}\otimes\deriv{I_{2}}{\C}+
2\,I_{2}\,\deriv{\iJb}{\C}\otimes\deriv{\iJb}{\C}+
2\,I_{2}\,\iJb\,\sderiv{\iJb}{\C}
\end{aligned}
\]

## General derivation of the second Piola-Kirchhoff stress and its derivative

The second Piola-Kirchhoff stress \(\S\) is defined by:

\[
\S=2\deriv{W}{\C}
\]

The more natural expression of the consistent tangent operator in that
context is \(\deriv{\S}{\C}\). We let `MFront` make
an implicit transformation to the appropriate tangent operator
expected by the solver.

The general expression of the second Piola-Kirchhoff stress is:

\[
\S=2\,\deriv{W^{v}}{\C}+2\deriv{W^{i}}{\C}=\S^{v}+\S^{i}
\]

where \(\S^{v}=2\,\deriv{W^{v}}{\C}\) is the
volumetric part of the the second Piola-Kirchhoff stress and
\(\S^{i}=2\,\deriv{W^{i}}{\C}\) is the isochoric
part.

### Volumetric part of the second Piola-Kirchhoff stress and its derivative

By chain rule, we have:
\[
  \S^{v} = 2\,\deriv{W^{v}}{\C}=2\,\deriv{W^{v}}{J}\,\deriv{J}{I3}\deriv{I_{3}}{\C}
  = 2\,\deriv{W^{v}}{J}\Frac{1}{2\,J}\,\deriv{I_{3}}{\C}
  = \Frac{1}{J}\,\deriv{W^{v}}{J}\,\deriv{I_{3}}{\C}
\]

The computation of \(\deriv{\S^{v}}{\C}\) is
straightforward:

\[
\begin{aligned}
\deriv{\S^{v}}{\C}
	&=\deriv{}{J}\paren{\Frac{1}{J}\,\deriv{W^{v}}{J}}\,\deriv{J}{I_{3}}\,\deriv{I_{3}}{\C}\otimes\deriv{I_{3}}{\C}+\Frac{1}{J}\,\deriv{W^{v}}{J}\,\sderiv{I_{3}}{\C}\\
	&=\paren{\sderiv{W^{v}}{J}-\Frac{1}{J}\,\deriv{W^{v}}{J}}\,\frac{1}{2\,I_{3}}\,\deriv{I_{3}}{\C}\otimes\deriv{I_{3}}{\C}+\Frac{1}{J}\,\deriv{W^{v}}{J}\,\sderiv{I_{3}}{\C}\\
\end{aligned}
\]

### Isochoric part of the second Piola-Kirchhoff stress and its derivative

By chain rule, we have:
\[
  \S^{i} = 2\,\deriv{W^{i}}{\C}=2\,\deriv{W^{i}}{\bar{I}_{1}}\,\deriv{\bar{I}_{1}}{\C}+2\,\deriv{W^{i}}{\bar{I}_{2}}\,\deriv{\bar{I}_{2}}{\C}
\]

The second derivative is also straightforward:
\[
\begin{aligned}
\sderiv{\S^{i}}{C}=&
2\,\sderiv{W^{i}}{\bar{I}_{1}}\,\deriv{\bar{I}_{1}}{\C}\otimes\deriv{\bar{I}_{1}}{\C}+
2\,{\displaystyle \frac{\displaystyle \partial^{2} W^{i}}{\partial \bar{I}_{1}\partial \bar{I}_{2}}}\,\deriv{\bar{I}_{1}}{\C}\otimes\deriv{\bar{I}_{2}}{\C}+
2\,\deriv{W^{i}}{\bar{I}_{1}}\sderiv{\bar{I}_{1}}{\C}+\\
& 2\,\sderiv{W^{i}}{\bar{I}_{2}}\,\deriv{\bar{I}_{2}}{\C}\otimes\deriv{\bar{I}_{2}}{\C}+
2\,{\displaystyle \frac{\displaystyle \partial^{2} W^{i}}{\partial \bar{I}_{1}\partial \bar{I}_{2}}}\,\deriv{\bar{I}_{2}}{\C}\otimes\deriv{\bar{I}_{1}}{\C}+
2\,\deriv{W^{i}}{\bar{I}_{2}}\sderiv{\bar{I}_{2}}{\C}
\end{aligned}
\]

## Application to the Signorini behaviour

#  Odgen Hyperelastic behaviours

We now consider another class of hyperelastic behaviour of the form:
More precisely, we consider an hyperelastic behaviour whose
potential \(W\) is decomposed in an volumetric part \(W^{\text{v}}\)
and an isochoric part \(W^{\text{i}}\):

\[
W\paren{\C}=W^{v}\paren{J}+\bar{W}^{i}\paren{\bar{\lambda}_{1},\bar{\lambda}_{2},\bar{\lambda}_{3}}
\]

where \(\paren{\bar{\lambda}_{i}}_{i\in \{1,2,3\}}\) are the
eigenvalues of the isochoric right Cauchy tensor \(\tenseur{\bar{C}}\).

\[
\bar{\lambda}_{i}=\iJb\,\lambda_{i}
\]

The volumetric part has been treated in the previous section. Let us
consider the isochoric part which is assumed to have the following
Odgen-type form:

\[
\bar{W}^{i}\paren{\bar{\lambda}_{1},\bar{\lambda}_{2},\bar{\lambda}_{3}}=\sum_{p=1}^{N}\bar{W}^{i}_{p}
\]

\[
\bar{W}^{i}_{p}\paren{\bar{\lambda}_{1},\bar{\lambda}_{2},\bar{\lambda}_{3}}
=\Frac{\mu_{p}}{\alpha_{p}}\paren{\bar{\lambda}_{1}^{\alpha_{p}/2}+\bar{\lambda}_{2}^{\alpha_{p}/2}+\bar{\lambda}_{3}^{\alpha_{p}/2}-3} 
=\Frac{\mu_{p}}{\alpha_{p}}\iJb^{\alpha_{p}/2}\,f\paren{\lambda_{1},\lambda_{2},\lambda_{3}}-\Frac{3\,\mu_{p}}{\alpha_{p}}
\]

with \(f\paren{\lambda_{1},\lambda_{2},\lambda_{3}}=\lambda_{1}^{\alpha_{p}/2}+\lambda_{2}^{\alpha_{p}/2}+\lambda_{3}^{\alpha_{p}/2}\)

Let us focus on the contribution \(\S_{p}\) of
\(W^{i}_{p}\) to the second Piola Kirchoff stress:
\[
\begin{aligned}
\S_{p}=2\,\deriv{\bar{W}^{i}_{p}}{\C}=
\mu_{p}\,\iJb^{\alpha_{p}/2-1}\,f\paren{\lambda_{1},\lambda_{2},\lambda_{3}}\deriv{\iJb}{\C}+
\Frac{2\,\mu_{p}\,\iJb^{\alpha_{p}/2}}{\alpha_{p}}\,\deriv{f}{\C}\\
\end{aligned}
\]

The associated contribution to the consistent tangent operator is
given the derivative of \(\S_{p}\) with respect to the right Cauchy
tensor \(\C\):

\[
\begin{aligned}
\Frac{1}{\mu_{p}}\,\deriv{S_{p}}{\C}
&=\frac{\alpha_{p}-2}{2}\,\iJb^{\alpha_{p}/2-2}\,f\paren{\lambda_{1},\lambda_{2},\lambda_{3}}\deriv{\iJb}{\C}\otimes\deriv{\iJb}{\C}\\
&+\iJb^{\alpha_{p}/2-1}\,f\paren{\lambda_{1},\lambda_{2},\lambda_{3}}\,\sderiv{\iJb}{\C}+\iJb^{\alpha_{p}/2-1}\,\deriv{\iJb}{\C}\otimes\deriv{f}{\C}\\
&+\iJb^{\alpha_{p}/2-1}\,\deriv{f}{\C}\otimes\deriv{\iJb}{\C}+
2\,\Frac{\iJb^{\alpha_{p}/2}}{\alpha_{p}}\,\sderiv{f}{\C}
\end{aligned}
\]

In this expression, we have:
\[
\deriv{f}{\C}=\sum_{i=1}^{3}\Frac{\alpha_{p}}{2}\,\lambda_{i}^{\alpha_{p}/2-1}\,\tenseur{n}_{i}
\]

where \(\paren{\tenseur{n}_{i}}_{i\in \{1,2,3\}}\) are the
eigentenseurs of the right Cauchy tensor.

Computing \(\sderiv{f}{\C}\) is a non trivial task as one have to take
care of the cases where two eigenvalues are equal and repeatedly use
the Hospital rule to pass to the limit. For this reason, it is much
easier to use the `computeIsotropicFunctionAndDerivative` static
method of the stensor class.

<!-- Local IspellDict: english -->
