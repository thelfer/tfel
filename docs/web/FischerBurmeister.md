% On the Fisher-Burmeister complementary function and its applications to multi-surface plasticity
% Thomas Helfer
% 17/01/2019

\newcommand{\paren}[1]{{\left(#1\right)}}

# The Fisher-Burmeister complementary function

The Fisher-Burmeister complementary function \(\Phi\paren{x,y}\) is
defined as follows:

\[
\Phi\paren{x,y}=x+y-\sqrt{x^{2}+y^{2}}
\]

\(\Phi\paren{x,y}\) has the following property:

\[
\Phi\paren{x,y}=0
\quad\Leftrightarrow\quad
\left\{
\begin{aligned}
x&\geq 0\\
y&\geq 0\\
x\,y&=0
\end{aligned}
\right.
\]{#eq:FB}

## Link with Karush-Kuhn-Tucker condition of plasticity

Equation @eq:FB clearly reflects the classical Karush-Kuhn-Tucker
condition of plasticity:

\[
\left\{
\begin{aligned}
\dot{\lambda}&\geq 0\\
f&\leq 0\\
\dot{\lambda}\,f&=0
\end{aligned}
\right.
\]{#eq:KKK}

where:

- \(\dot{\lambda}\) is the plastic multiplier.
- \(f\) is a the yield function such that:
    - \(f< 0\) defines the elastic domain.
    - \(f= 0\) defines the yield surface.

In other words, finding a plastic multiplier \(\dot{\lambda}\)
satisfying Conditions @eq:KKK is equivalent to find the root of the
following equation:

\[
\Phi\paren{\dot{\lambda},-f}=0
\]{#eq:FBp}

If several plastic surfaces are involved, each plastic multipliers must
satisfy an equation similar to @eq:FBp. In an implicit formulation, this
remove the need of using a status method to determine which mechanisms
are active or inactive.


## A regularized version of the Fisher-Burmeister complementary function

\(\Phi\paren{x,y}\) is differentiable everywhere, except at \(0,0\). For
this reason, following @liao-mcpherson_regularized_2019, we introduce a
regularized version denoted \(\Phi_{\varepsilon}\paren{x,y}\):

\[
\Phi_{\varepsilon}\paren{x,y}=x+y-\sqrt{x^{2}+y^{2}+\varepsilon^{2}}
\]

where \(\varepsilon\) is a small regularization parameter.
\(\Phi_{\varepsilon}\paren{x,y}\) has the following property:

\[
\Phi_{\varepsilon}\paren{x,y}=0
\quad\Leftrightarrow\quad
\left\{
\begin{aligned}
x&\geq 0\\
y&\geq 0\\
2\,x\,y&=\varepsilon^{2}
\end{aligned}
\right.
\]{#eq:FBe}




# Acknowledgements

The authors are grateful to Y. Chemisky, main developer of the `Simcoon`
library, for introducing them to the use of the Fisher-Burmeister
complementary function in multi-surfaces plasticity.