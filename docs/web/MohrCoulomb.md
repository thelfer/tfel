% Invariant-based implementation of the Mohr-Coulomb elasto-plastic model in OpenGeoSys using MFront
% Thomas Nagel, Dmitri Naumov, Thomas Helfer
% 1/08/2019

<!--
pandoc --filter pandoc-crossref --filter pandoc-citeproc --bibliography=bibliography.bib -fmarkdown+tex_math_single_backslash MohrCoulomb.md -o MohrCoulomb.pdf --toc --number-sections  -V geometry:margin=1in
-->

\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\dcdot}{\,:\,}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonvis}{\tenseur{\varepsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonvis}{\tenseur{\dot{\varepsilon}}^{\mathrm{vis}}}
\newcommand{\tepsilonp}{\tenseur{\varepsilon}^{\mathrm{p}}}
\newcommand{\tdepsilonp}{\tenseur{\dot{\varepsilon}}^{\mathrm{p}}}
\newcommand{\cancelto}[2]{\underbrace{#2}_{#1}}
\newcommand{\nn}{\nonumber}


The following proposal is based on @Nagel2016 but extended by an apex
smoothing from @Abbo1995 (hyperbolic approximation).

# Yield function and plastic potential

Tensile stresses are taken as positive.

\[
	I_1 = \trace{\tsigma} \qquad J_2 = \frac{1}{2} \tsigma^\mathrm{D} \dcdot \tsigma^\mathrm{D} \qquad J_3 = \det \tsigma^\mathrm{D} \qquad \theta = \frac{1}{3} \arcsin \left( -\frac{3\sqrt{3} J_3}{2 \sqrt{J_2^3}} \right)
\]

The yield function reads

\[
	F = \frac{I_1}{3} \sin \phi + \sqrt{J_2 K(\theta)^2 + a^2 \sin^2 \phi} - c \cos \phi
\]{#eq:yield_function}
where 
\[
K(\theta) = \begin{cases}
	\cos \theta - \frac{1}{\sqrt{3}} \sin \phi \sin \theta & |\theta| < \theta_\mathrm{T}
	\\ 
	A - B \sin 3\theta &  |\theta| \geq \theta_\mathrm{T}
\end{cases}
\]{#eq:K_abbo}

where \(c\) and \(\phi\) are the cohesion and friction angle, respectively, and
\[
\begin{aligned}
	A &= \frac{1}{3} \cos \theta_\mathrm{T} \left[ 3 + \tan \theta_\mathrm{T} \tan 3\theta_\mathrm{T} + \frac{1}{\sqrt{3}} \mathrm{sign \theta} \, (\tan 3\theta_\mathrm{T} - 3\tan \theta_\mathrm{T}) \sin \phi \right] \\
	%
	B &= \frac{1}{3} \frac{1}{\cos 3\theta_\mathrm{T}} \left[ \mathrm{sign \theta} \, \sin \theta_\mathrm{T} + \frac{1}{\sqrt{3}} \sin \phi \cos \theta_\mathrm{T} \right]
\end{aligned}
\]{#eq:Sloan}

The plastic potential differs from the yield surface in order to more
accurately estimate dilatancy, but has an analogous structure:

\[
	G_\mathrm{F} = \frac{I_1}{3} \sin \psi + \sqrt{J_2 K_G^2 + a^2 \sin^2 \psi} - c \cos \psi
\]{#eq:plastic_potential}

where \(\psi\) is the dilatancy angle. \(K_G\), \(A_G\) and \(B_G\) follow
from @eq:K_abbo and @eq:Sloan by substituting the friction angle
with the dilatancy angle : 

\[
K_G(\theta) = \begin{cases}
	\cos \theta - \frac{1}{\sqrt{3}} \sin \psi \sin \theta & |\theta| < \theta_\mathrm{T}
	\\ 
	A_G - B_G \sin 3\theta &  |\theta| \geq \theta_\mathrm{T}
\end{cases}
\]

\[
\begin{aligned}
	A_G &= \frac{1}{3} \cos \theta_\mathrm{T} \left[ 3 + \tan \theta_\mathrm{T} \tan 3\theta_\mathrm{T} + \frac{1}{\sqrt{3}} \mathrm{sign \theta} \, (\tan 3\theta_\mathrm{T} - 3\tan \theta_\mathrm{T}) \sin \psi \right] \\
	%
	B_G &= \frac{1}{3} \frac{1}{\cos 3\theta_\mathrm{T}} \left[ \mathrm{sign \theta} \sin \theta_\mathrm{T} + \frac{1}{\sqrt{3}} \sin \psi \cos \theta_\mathrm{T} \right]
\end{aligned}
\]

--- 

The contribution of the smoothing is visualized in Figure @fig:mc_vis.

![Effect of the smoothing of the yield surface](img/MohrCoulombSmoothing.svg "Effect of the smoothing of the yield surface"){#fig:mc_vis width=115%}



# Flow rule

Plastic flow follows in a general manner for 
\(I_1,\,J_2,\,\theta\)-type yield surface:

\[
	\tdepsilonp = \dot{\lambda} \frac{\partial G_\mathrm{F}}{\partial \tsigma} = \dot{\lambda} \left[ \frac{\partial G_\mathrm{F}}{\partial I_1} \tenseur{I} + \left( \frac{\partial G_\mathrm{F}}{\partial J_2} + \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial \theta}{\partial J_2} \right) \tsigma^\mathrm{D} +  \frac{\partial G_\mathrm{F}}{\partial \theta}  \frac{\partial \theta}{\partial J_3} J_3 {(\tsigma^{\mathrm{D}})}^{-1} \dcdot \tenseurq{P}^\mathrm{D} \right] 
\]{#eq:MC_flow}

Alternative formulation using the Caley-Hamilton theorem:
\[
	\tenseur{A}^3 - I_1 \tenseur{A}^2 + I_2 \tenseur{A} - I_3 \tenseur{I} = \tenseur{0}
\]

Taking the trace and subsequently the derivative w.r.t. \(\tenseur{A}\)
yields:
\[
	I_{3,\tenseur{A}} = \tenseur{A}^2 + I_2 \tenseur{I} - I_1 \tenseur{A}
\]

For the deviator we have \(I_2(\tsigma^\mathrm{D}) = -J_2\) and
\(I_1(\tsigma^\mathrm{D})=0\) and thus write:
\[
	\frac{\partial J_3}{\partial \tsigma^\mathrm{D}} = J_3 (\tsigma^\mathrm{D})^{-1} = (\tsigma^\mathrm{D})^{2} - J_2 \tenseur{I}
\]

Furthermore,
\[
\frac{\partial J_3}{\partial \tsigma} = \left[(\tsigma^\mathrm{D})^{2} - J_2 \tenseur{I} \right] \dcdot \tenseurq{P}^\mathrm{D} = (\tsigma^\mathrm{D})^{2} \dcdot \tenseurq{P}^\mathrm{D}
\]

Therefore, 
\[
	\tenseur{n} = \frac{\partial G_\mathrm{F}}{\partial I_1} \tenseur{I} + \left( \frac{\partial G_\mathrm{F}}{\partial J_2} + \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial \theta}{\partial J_2} \right) \tsigma^\mathrm{D} +  \frac{\partial G_\mathrm{F}}{\partial \theta}  \frac{\partial \theta}{\partial J_3} J_3 {(\tsigma^{\mathrm{D}})}^{-1} \dcdot \tenseurq{P}^\mathrm{D}
\]
or
\[
	\tenseur{n} = \frac{\partial G_\mathrm{F}}{\partial I_1} \tenseur{I} + \left( \frac{\partial G_\mathrm{F}}{\partial J_2} + \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial \theta}{\partial J_2} \right) \tsigma^\mathrm{D} +  \frac{\partial G_\mathrm{F}}{\partial \theta}  \frac{\partial \theta}{\partial J_3} (\tsigma^\mathrm{D})^{2}\dcdot \tenseurq{P}^\mathrm{D}
\]

with the material-independent derivatives:

\[
\begin{aligned}
	\left. \frac{\partial \theta}{\partial J_2} \right|_{J_3} &= -\frac{\tan 3\theta}{2J_2}
	\\
	\left. \frac{\partial \theta}{\partial J_3} \right|_{J_2} &= \frac{\tan 3\theta}{3J_3}
\end{aligned}
\]

and the material-dependent derivatives
\[
\begin{aligned}
	\left. \frac{\partial G_\mathrm{F}}{\partial I_1} \right|_{J_2,J_3} &= \frac{1}{3} \sin \psi
	\\
	\left. \frac{\partial G_\mathrm{F}}{\partial J_2} \right|_{I_1,\theta} &= 
	\frac{K'^2}{2\sqrt{J_2 K_G^2 + a^2 \sin^2 \psi}}
	\\
	\left. \frac{\partial G_\mathrm{F}}{\partial \theta} \right|_{I_1,J_2} &= 
	\frac{J_2 K_G}{\sqrt{J_2 K_G^2 + a^2 \sin^2 \psi}} \frac{\partial K_G}{\partial \theta}
	\\
	\frac{\partial K}{\partial \theta} &= \begin{cases}
	-\sin \theta - \frac{1}{\sqrt{3}}\sin \phi \cos\theta & |\theta| < \theta_\mathrm{T}
	\\ 
	-3B\cos3\theta
\end{cases}
\end{aligned}
\]

The following combined entries are useful:
\[
\begin{aligned}
	\frac{\partial G_\mathrm{F}}{\partial \theta}\frac{\partial \theta}{\partial J_2} &= -\frac{K_G\tan 3\theta}{2\sqrt{J_2 K_G^2 + a^2 \sin^2 \psi}} \frac{\partial K_G}{\partial \theta} 
\end{aligned}
\]

Thus,
\[
\begin{aligned}
	\left. \frac{\partial G_\mathrm{F}}{\partial I_1} \right|_{J_2,J_3} &= \frac{1}{3} \sin \psi
	\\
	\left.\frac{\partial G_\mathrm{F}}{\partial J_2}\right|_{I_1,J_3} &= 
	\frac{K_G}{2\sqrt{J_2 K_G^2 + a^2 \sin^2 \psi}} \left(K_G - \tan 3\theta \frac{\partial K_G}{\partial \theta} \right)
	\\
	\left.\frac{\partial G_\mathrm{F}}{\partial J_3}\right|_{I_1,J_2} &= \frac{J_2K_G\tan 3\theta}{3J_3\sqrt{J_2 K_G^2 + a^2 \sin^2 \psi}} \frac{\partial K_G}{\partial \theta}
\end{aligned}
\]

so that:
\[
	\tenseur{n} = \frac{\partial G_\mathrm{F}}{\partial I_1} \tenseur{I} + \left.\frac{\partial G_\mathrm{F}}{\partial J_2}\right|_{I_1,J_3} \tsigma^\mathrm{D} +  \left.\frac{\partial G_\mathrm{F}}{\partial J_3}\right|_{I_1,J_2} (\tsigma^{\mathrm{D}})^{2} \dcdot \tenseurq{P}^\mathrm{D}
\]

# Linearization of flow rule

\[
\begin{split}
	\frac{\partial \tenseur{n}}{\partial \tsigma} &= \left.\frac{\partial G_\mathrm{F}}{\partial J_2}\right|_{I_1,J_3} \tenseurq{P}^\mathrm{D} + \left.\frac{\partial G_\mathrm{F}}{\partial J_3}\right|_{I_1,J_2} \tenseurq{P}^\mathrm{D} \dcdot \left[ \left( \tenseur{I} \otimes \tsigma^\mathrm{D} \right)^{\overset{23}{\mathrm{T}}} + \left(  \tsigma^\mathrm{D} \otimes \tenseur{I}\right)^{\overset{23}{\mathrm{T}}}\right] \dcdot \tenseurq{P}^\mathrm{D} \\
	&+ \left.\frac{\partial^2 G_\mathrm{F}}{\partial J_2^2} \right|_{J_3} \tsigma^\mathrm{D} \otimes \tsigma^\mathrm{D} + \left. \frac{\partial^2 G_\mathrm{F}}{\partial J_3^2} \right|_{J_2} \left[{(\tsigma^{\mathrm{D}})}^{2} \dcdot \tenseurq{P}^\mathrm{D} \otimes  {(\tsigma^{\mathrm{D}})}^{2} \dcdot \tenseurq{P}^\mathrm{D} \right] \\
	&+ \frac{\partial}{\partial J_2}\left[ \left. \frac{\partial G_\mathrm{F}}{\partial J_3} \right|_{J_2} \right]_{J_3}  \left[ \tsigma^\mathrm{D} \otimes {(\tsigma^{\mathrm{D}})}^{2} \dcdot \tenseurq{P}^\mathrm{D} + {(\tsigma^{\mathrm{D}})}^{2} \dcdot \tenseurq{P}^\mathrm{D} \otimes \tsigma^{\mathrm{D}} \right]
\end{split}
\]

We further require the derivatives:
\[
\begin{aligned}
	\left.\frac{\partial^2 G_\mathrm{F}}{\partial J_2^2} \right|_{J_3} &= \left. \frac{\partial}{\partial J_2}\right|_\theta \left.\frac{\partial G_\mathrm{F}}{\partial J_2}\right|_{I_1,J_3} + \left. \frac{\partial}{\partial \theta} \right|_{J_2} \left.\frac{\partial G_\mathrm{F}}{\partial J_2}\right|_{I_1,J_3} \frac{\partial \theta}{\partial J_2}
	\\
	&= \frac{\partial^2 G_\mathrm{F}}{\partial^2 J_2^2} + \frac{\partial^2 G_\mathrm{F}}{\partial J_2 \partial \theta} \frac{\partial \theta}{\partial J_2} + \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial^2 \theta}{\partial J_2^2} \\
	&+\frac{\partial \theta}{\partial J_2} \left(\frac{\partial^2 G_\mathrm{F}}{\partial J_2 \partial \theta} + \frac{\partial^2 G_\mathrm{F}}{\partial \theta^2} \frac{\partial \theta}{\partial J_2} + \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial^2 \theta}{\partial J_2 \partial \theta}\right)
	\\
	&= \frac{\partial^2 G_\mathrm{F}}{\partial^2 J_2^2} +  \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial^2 \theta}{\partial J_2^2}
	\\
	&+\frac{\partial \theta}{\partial J_2} \left(2\frac{\partial^2 G_\mathrm{F}}{\partial J_2 \partial \theta} + \frac{\partial^2 G_\mathrm{F}}{\partial \theta^2} \frac{\partial \theta}{\partial J_2} + \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial^2 \theta}{\partial J_2 \partial \theta}\right)
	\\
	\\
	\left. \frac{\partial^2 G_\mathrm{F}}{\partial J_3^2} \right|_{J_2} &= \left. \frac{\partial}{\partial J_3} \right|_\theta \left.\frac{\partial G_\mathrm{F}}{\partial J_3}\right|_{I_1,J_2} + \left. \frac{\partial}{\partial \theta} \right|_{J_3} \left.\frac{\partial G_\mathrm{F}}{\partial J_3}\right|_{I_1,J_2} \frac{\partial \theta}{\partial J_3}
	\\
	&= \cancelto{0}{\frac{\partial^2 G_\mathrm{F}}{\partial \theta \partial J_3}}\frac{\partial \theta}{\partial J_3} + \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial^2 \theta}{\partial J_3^2} 
	\\
	&+\frac{\partial \theta}{\partial J_3} \left(\frac{\partial^2 G_\mathrm{F}}{\partial \theta^2}\frac{\partial \theta}{\partial J_3} + \frac{\partial G_\mathrm{F}}{\partial \theta}\frac{\partial^2 \theta}{\partial J_3 \partial \theta} \right)
	\\
	&= \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial^2 \theta}{\partial J_3^2} +\frac{\partial \theta}{\partial J_3} \left(\frac{\partial^2 G_\mathrm{F}}{\partial \theta^2}\frac{\partial \theta}{\partial J_3} + \frac{\partial G_\mathrm{F}}{\partial \theta}\frac{\partial^2 \theta}{\partial J_3 \partial \theta} \right)
	\\
	\\
	\frac{\partial}{\partial J_2}\left[ \left. \frac{\partial G_\mathrm{F}}{\partial J_3} \right|_{J_2} \right]_{J_3}  &= \left. \frac{\partial}{\partial J_2} \right|_\theta \left.\frac{\partial G_\mathrm{F}}{\partial J_3}\right|_{I_1,J_2} + \left. \frac{\partial}{\partial \theta} \right|_{J_2} \left.\frac{\partial G_\mathrm{F}}{\partial J_3}\right|_{I_1,J_2} \frac{\partial \theta}{\partial J_2}
	\\
	&= \frac{\partial^2 G_\mathrm{F}}{\partial J_2 \partial \theta}\frac{\partial \theta}{\partial J_3} + \frac{\partial G_\mathrm{F}}{\partial \theta}\cancelto{0}{\frac{\partial^2 \theta}{\partial J_3 \partial J_2}} 
	\\
	&+ \frac{\partial \theta}{\partial J_2} \left( \frac{\partial^2 G_\mathrm{F}}{\partial \theta^2} \frac{\partial \theta}{\partial J_3} + \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial^2 \theta}{\partial J_3 \partial \theta} \right)
	\\
	&=\frac{\partial^2 G_\mathrm{F}}{\partial J_2 \partial \theta}\frac{\partial \theta}{\partial J_3} + \frac{\partial \theta}{\partial J_2} \left( \frac{\partial^2 G_\mathrm{F}}{\partial \theta^2} \frac{\partial \theta}{\partial J_3} + \frac{\partial G_\mathrm{F}}{\partial \theta} \frac{\partial^2 \theta}{\partial J_3 \partial \theta} \right)
\end{aligned}
\]

Material-independent derivatives:
\[
\begin{aligned}
	\left. \frac{\partial^2 \theta}{\partial J_2^2} \right|_\theta &= \frac{\tan 3\theta}{2J_2^2}
	\\
	\left. \frac{\partial^2 \theta}{\partial J_3^2} \right|_\theta &= -\frac{\tan 3\theta}{3J_3^2}
	\\
	\frac{\partial^2 \theta}{\partial J_2 \partial \theta} &= - \frac{3}{2J_2 \cos^2 3\theta}
	\\
	\frac{\partial^2 \theta}{\partial J_3 \partial \theta} &= \frac{1}{J_3 \cos^2 3\theta}
\end{aligned}
\]

Material-dependent derivatives:
\[
\begin{aligned}
	\frac{\partial^2 G_\mathrm{F}}{\partial J_2^2} &= 
	-\frac{K_G^4}{4\sqrt{(J_2 K_G^2 + a^2 \sin^2 \psi)^3}}
	\\
	\frac{\partial^2 G_\mathrm{F}}{\partial \theta^2} &= 
	\left( \frac{J_2}{\sqrt{J_2 K_G^2 + a^2 \sin^2 \psi}} - \frac{J_2^2 K_G^2}{\sqrt{(J_2 K_G^2 + a^2 \sin^2 \psi)^3}} \right)\left(\frac{\partial K_G}{\partial \theta}\right)^2 \\
	&+  \frac{J_2K_G}{\sqrt{J_2 K_G^2 + a^2 \sin^2 \psi}} \frac{\partial^2 K_G}{\partial \theta^2}  
\\
	&=  \frac{J_2}{\sqrt{J_2 K_G^2 + a^2 \sin^2 \psi}} \left[\left(\frac{\partial K_G}{\partial \theta}\right)^2 \left(1 - \frac{J_2K_G^2}{J_2 K_G^2 + a^2 \sin^2 \psi} \right) + K_G \frac{\partial^2 K_G}{\partial \theta^2}  \right]
	\\
	\frac{\partial^2 G_\mathrm{F}}{\partial J_2 \partial \theta} &= 
	\left( \frac{K_G}{\sqrt{J_2 K_G^2 + a^2 \sin^2 \psi}} - \frac{J_2 K_G^3}{2\sqrt{(J_2 K_G^2 + a^2 \sin^2 \psi)^3}} \right)\frac{\partial K_G}{\partial \theta}
	\\
	&=\frac{K_G}{\sqrt{J_2 K_G^2 + a^2 \sin^2 \psi}}\frac{\partial K_G}{\partial \theta}\left( 1 - \frac{J_2 K_G^2}{2(J_2 K_G^2 + a^2 \sin^2 \psi)} \right)
	\\
	\frac{\partial^2 K}{\partial \theta^2} &= \begin{cases}
	-\cos \theta + \frac{1}{\sqrt{3}}\sin \phi \sin\theta & |\theta| < \theta_\mathrm{T}
	\\ 
	9B\sin3\theta
\end{cases}
\end{aligned}
\]

Jacobian is here (non-associated!)
\[
\tenseur{J} = 
\begin{pmatrix}
    \tenseurq{I} + \Delta \lambda \dfrac{\partial \tenseur{n}}{\partial \Delta \tepsilonel} & \tenseur{n} \\
    E^{-1}\tenseur{n}_F \dcdot \tenseurq{C} & 0
\end{pmatrix}
\]{#eq:Jacobian}

<!--
% ## Return false
%
%Principal stresses:
%\begin{align*}
%	\sigma_1 &= \frac{2}{\sqrt{3}} \sqrt{J_2} \sin \left( \theta + \frac{2\pi}{3} \right) + \frac{I_1}{3}
%	\\
%	\sigma_2 &= \frac{2}{\sqrt{3}} \sqrt{J_2} \sin \theta + \frac{I_1}{3}
%	\\
%	\sigma_3 &= \frac{2}{\sqrt{3}} \sqrt{J_2} \sin \left( \theta - \frac{2\pi}{3} \right) + \frac{I_1}{3}
%\end{align*}
%
%Maximum shear stress:
%\[
%	\tau_\mathrm{max} = \frac{\sigma_1 - \sigma_3}{2} = \sqrt{J_2} \cos \theta
%\]
%
%MC can be expressed as (include graphics)
%\[
%	\tau_\mathrm{max} = -\sigma_n \sin \phi + c \cos \phi
%\]
%
%Where the normal stress is:
%\[
%	\sigma_\mathrm{n} = \frac{\sigma_1 + \sigma_3}{2} = - \frac{\sqrt{J_2}}{\sqrt{3}} \sin \theta + \frac{I_1}{3}
%\]
%
%Thus,
%
%\[
%	\tau_\mathrm{Y} =  \frac{\sqrt{J_2}}{\sqrt{3}} \sin \theta \sin \phi + c \cos \phi - \frac{I_1}{3} \sin\phi
%\]
-->


# Verification

![Trace of the yield surface (green) when approached on different stress paths (purple)](img/MohrCoulombPiPlane.svg "Trace of the yield surface (green) when approached on different stress paths (purple)"){#fig:MC_stress_paths width=90%}

The yield function was approached along different stress paths, shown in
Figure @fig:MC_stress_paths within the \(\pi\)-plane. This shows that
a) yield is correctly detected, and b) the stress-state is correctly
pulled back onto the yield surface.

![Verification against analytical example. Description in @Nagel2016.](img/MohrCoulombAnalitical.svg "Verification against analytical example. Description in @Nagel2016"){#fig:MC_vis width=125%}

The second test (included as a benchmark) follows an analytical solution
for a stress-free cavity in an infinite medium under a variable
far-field stress. The solution computes stress- and displacement fields
as well as the location of the plastified zone. It has been used in
@Nagel2016 where the test is described in more detail (with a
partial extension towards non-associated flow).


# Implementation

## Choice of domain specific language

While not mandatory (the `@DSL` keyword can be place anywhere in the
file), its is convenient to start the implementation by declaring the
domain specific language to be used. For an integration by a
\(\theta\)-scheme, the `Implicit` domain specific language is choosen:

~~~~{.cxx}
@DSL Implicit;
~~~~

## Name of the behaviour

The `@Behaviour` keyword is used to give the name of the behaviour.

~~~~{.cxx}
@Behaviour MohrCoulombAbboSloan;
~~~~

## Metadata

The following instructions give some information about the author, the
date 

~~~~{.cxx}
@Author Thomas Nagel;
@Date 05/02/2019;
~~~~

## Name of the Algorithm of resolution


The `@Algorithm` keyword is used to give the name of the algorithm.

~~~~{.cxx}
@Algorithm NewtonRaphson;
~~~~

## Usage of the `StandardElasticity` brick

The implicit scheme used satisfies the requirements of the
`StandardElasticity` brick as described [here](BehaviourBricks.html).

The `StandardElasticity` brick which provides:

- Automatic computation of the stress tensor at various stages of the
  behaviour integration.
- Automatic computation of the consistent tangent operator.
- Automatic support for plane stress and generalized plane stress
  modelling hypotheses (The axial strain is defined as an additional
  state variable and the associated equation in the implicit system is
  added to enforce the plane stess condition).
- Automatic addition of the standard terms associated with the elastic
  strain state variable.
The usage of the `StandardElasticity` is introduced as follows:

~~~~{.cpp}
@Brick StandardElasticity;
~~~~


## Numerical parameters

The following instruction changes the default value of the stopping
criterion \(\epsilon\) used by the Newton-Raphson method  and the time integration scheme parameter \(\theta\) :

~~~~{.cxx}
@Theta 1.0;
@Epsilon 1.e-14;
~~~~

## Supported modelling hypothesis

Thanks to the `StandardElasticity` brick, all the modelling hypotheses
can be supported. The following statement, starting with the
`@ModellingHypotheses`, enables all the modelling hypotheses:

~~~~{.cpp}
@ModellingHypotheses {".+"};
~~~~


## RequireStiffnessTensor<Unaltered>

The `@RequireStiffnessTensor` keyword requires the stiffness tensor to
be computed by the calling code. This generally means that some extra
material properties will be introduced and handled by the interface
before the behaviour integration.


~~~~{.cpp}
@RequireStiffnessTensor<UnAltered>;
~~~~


## State variable

The `@StateVariable` keyword introduces the EquivalentPlasticStrain \(\lambda\).

~~~~{.cpp}
@StateVariable real lam;
lam.setGlossaryName("EquivalentPlasticStrain");
~~~~

## Material properties

The `@MaterialProperty` keyword introduces several properties, here:

- The cohesion c
- The friction angle \(\phi\) 
- The dilatancy angle \(\psi\)
- The transition angle \(\theta_T\)
- The tension cut-off control parameter a

~~~~{.cpp}
@MaterialProperty stress c;
c.setEntryName("Cohesion");
@MaterialProperty real phi;
phi.setEntryName("FrictionAngle");
@MaterialProperty real psi;
psi.setEntryName("DilatancyAngle");
@MaterialProperty real lodeT;
lodeT.setEntryName("TransitionAngle");
@MaterialProperty stress a;
a.setEntryName("TensionCutOffParameter");
~~~~

## Local variable

In `MFront`, an integration variable is defined to store a variable and
use it in various code block.

Here several local variables are declared such as the bolean variable F:
if true, plastic loading

~~~~{.cxx}
@LocalVariable Stensor np;
@LocalVariable bool F; // if true, plastic loading
@LocalVariable real sin_psi;
@LocalVariable real sin_phi;
@LocalVariable real cos_phi;
@LocalVariable real cos_lodeT;
@LocalVariable real sin_lodeT;
@LocalVariable real tan_lodeT;
@LocalVariable real cos_3_lodeT;
@LocalVariable real sin_3_lodeT;
@LocalVariable real tan_3_lodeT;
~~~~


## Initialisation a the local variable

The `@InitLocalVariables` code block is called before the behaviour
integration. 

~~~~{.cxx}
@InitLocalVariables 
{
~~~~

First, we define some variables :

~~~~{.cxx}
  constexpr auto sqrt3 = Cste<real>::sqrt3;
  constexpr auto isqrt3 = Cste<real>::isqrt3;
  // conversion to rad
  phi *= pi / 180.;
  psi *= pi / 180.;
  lodeT *= pi / 180.;
  sin_psi = sin(psi);
  cos_phi = cos(phi);
  sin_phi = sin(phi);
  sin_lodeT = sin(lodeT);
  cos_lodeT = cos(lodeT);
  tan_lodeT = tan(lodeT);
  cos_3_lodeT = cos(3. * lodeT);
  sin_3_lodeT = sin(3. * lodeT);
  tan_3_lodeT = tan(3. * lodeT);
~~~~

Then the  `computeElasticPrediction` method (introducted with the 
`StandardElasticity` brick) is used to compute \(\sigma^{el}\)


~~~~{.cxx}
  // elastic prediction
  const auto sig_el = computeElasticPrediction();
~~~~


The three invariant \(I_{1}^{el}\), \(J_{2}^{el}\) and \(J_{3}^{el}\) 
corresponding to the elastic prediction are calculated:

~~~~{.cxx}
  const auto s_el = deviator(sig_el);
  const auto I1_el = trace(sig_el);
  const auto J2_el = max((s_el | s_el) / 2., local_zero_tolerance);
  const auto J3_el = det(s_el);
~~~~

The \(\theta^{el}\) angle is defined:

~~~~{.cxx}
  const auto arg = min(max(-3. * sqrt3 * J3_el / (2. * J2_el * sqrt(J2_el)),
                           -1. + local_zero_tolerance),
                       1. - local_zero_tolerance);
  const auto lode_el = 1. / 3. * asin(arg);
~~~~

K is initiliazed as \(K^{el}\) value:

~~~~{.cxx} 
  auto K = 0.0;
  if (fabs(lode_el) < lodeT) 
  {
    K = cos(lode_el) - isqrt3 * sin_phi * sin(lode_el);
  } 
  else 
  {
    const auto sign =
        min(max(lode_el / max(abs(lode_el), local_zero_tolerance), -1.), 1.);
    const auto A = 1. / 3. * cos_lodeT *
                   (3. + tan_lodeT * tan_3_lodeT +
                    isqrt3 * sign * (tan_3_lodeT - 3. * tan_lodeT) * sin_phi);
    const auto B = 1 / (3. * cos_3_lodeT) *
                   (sign * sin_lodeT + isqrt3 * sin_phi * cos_lodeT);
    K = A - B * arg;
  }
~~~~

To finish \(F^{el}\) is calculated and the normal `np` is initialized.

~~~~{.cxx}
  const auto sMC =
      I1_el / 3 * sin_phi + sqrt(J2_el * K * K + a * a * sin_phi * sin_phi);
  F = sMC - c * cos_phi > 0.;
  np = Stensor(real(0));
~~~~


## Implicit system implementation

The implementation of the implicit system and its derivative is done in
the `@Integrator` code block:

~~~~{.cxx}
@Integrator{
~~~~

Some expressions are defined

~~~~{.cxx}
  constexpr auto sqrt3 = Cste<real>::sqrt3;
  constexpr auto isqrt3 = Cste<real>::isqrt3;
  constexpr auto id = Stensor::Id();
  constexpr auto id4 = Stensor4::Id();
~~~~

If there is no plasticity (elastic strain) there is no need for additional 
calculation because the various variables have already been initialized 
with elastic hypothesis. If there is plastic strain the rest is necessary.

~~~~{.cxx}
if (F) 
  {
~~~~

\(K\) and \(\dfrac{\partial K}{\partial \theta}\) are computed:

~~~~{.cxx}
const auto s = deviator(sig);
    const auto I1 = trace(sig);
    const auto J2 = max((s | s) / 2., local_zero_tolerance);
    const auto J3 = real(det(s) < 0. ? min(det(s), -local_zero_tolerance)
                                     : max(det(s), local_zero_tolerance));
    const auto arg = min(max(-3. * sqrt3 * J3 / (2. * J2 * sqrt(J2)),
                             -1. + local_zero_tolerance),
                         1. - local_zero_tolerance);
    const auto lode = 1. / 3. * asin(arg);
    const auto cos_lode = cos(lode);
    const auto sin_lode = sin(lode);
    const auto cos_3_lode = cos(3. * lode);
    const auto sin_3_lode = arg;
    const auto tan_3_lode = tan(3. * lode);
    auto K = 0.;
    auto dK_dlode = 1.;
    if (fabs(lode) < lodeT) 
    {
      K = cos_lode - isqrt3 * sin_phi * sin_lode;
      dK_dlode = -sin_lode - isqrt3 * sin_phi * cos_lode;
    } 
    else 
    {
      const auto sign =
          min(max(lode / max(abs(lode), local_zero_tolerance), -1.), 1.);
      const auto A = 1. / 3. * cos_lodeT *
                     (3. + tan_lodeT * tan_3_lodeT +
                      isqrt3 * sign * (tan_3_lodeT - 3. * tan_lodeT) * sin_phi);
      const auto B = 1. / (3. * cos_3_lodeT) *
                     (sign * sin_lodeT + isqrt3 * sin_phi * cos_lodeT);
      K = A - B * sin_3_lode;
      dK_dlode = -3. * B * cos_3_lode;
    }
~~~~

\(K_G\),  \(\dfrac{\partial K_G}{\partial \theta}\) and
 \(\dfrac{\partial^2 K_G}{\partial^2 \theta}\) are computed :

~~~~{.cxx}
auto KG = 0.0; // move into a function to avoid code duplication
    auto dKG_dlode = 1.;
    auto dKG_ddlode = 1.;
    if (fabs(lode) < lodeT) 
    {
      KG = cos_lode - isqrt3 * sin_psi * sin_lode;
      dKG_dlode = -sin_lode - isqrt3 * sin_psi * cos_lode;
      dKG_ddlode = -cos_lode + isqrt3 * sin_psi * sin_lode;
    } 
    else 
    {
      const auto sign =
          min(max(lode / max(fabs(lode), local_zero_tolerance), -1.), 1.);
      const auto A = 1. / 3. * cos_lodeT *
                     (3. + tan_lodeT * tan_3_lodeT +
                      isqrt3 * sign * (tan_3_lodeT - 3. * tan_lodeT) * sin_psi);
      const auto B = 1. / (3. * cos_3_lodeT) *
                     (sign * sin_lodeT + isqrt3 * sin_psi * cos_lodeT);
      KG = A - B * sin_3_lode;
      dKG_dlode = -3. * B * cos_3_lode;
      dKG_ddlode = 9. * B * sin_3_lode;
    }
~~~~
 
The flow direction is computed :

~~~~{.cxx}
// flow direction
    const auto dev_s_squared = computeJ3Derivative(
        sig); // replaces dev_s_squared = deviator(square(s));
    const auto dG_dI1 = sin_psi / 3.;
    const auto root = max(sqrt(J2 * KG * KG + a * a * sin_psi * sin_psi),
                          local_zero_tolerance);
    const auto dG_dJ2 = KG / (2. * root) * (KG - tan_3_lode * dKG_dlode);
    const auto dG_dJ3 = J2 * KG * tan_3_lode / (3. * J3 * root) * dKG_dlode;
    const auto n = eval(dG_dI1 * id + dG_dJ2 * s + dG_dJ3 * dev_s_squared);
~~~~

The yield function F is computed:

~~~~{.cxx}
// yield function
    const auto rootF = max(sqrt(J2 * K * K + a * a * sin_phi * sin_phi), local_zero_tolerance);

    const auto Fy1 = I1 * sin_phi / 3 + rootF;
    const auto Fy =  Fy1 - c * cos_phi;
~~~~
 
Derivatives are calculated before computing the Jacobian matrix:

\(\dfrac{\partial F}{\partial I_1}\), \(\dfrac{\partial F}{\partial J_2}\),
\(\dfrac{\partial F}{\partial J_3}\), \(n_F\),
\(\dfrac{\partial G}{\partial \theta}\), \(\dfrac{\partial^2 G}{\partial^2 \theta}\)
\(\dfrac{\partial^2 G}{\partial \theta \partial J_2}\),
\(\dfrac{\partial^2 G}{\partial^2 J_2}\), \(\dfrac{\partial^2 G}{\partial^2 J_3}\), 
\(\dfrac{\partial ^2G}{\partial J_2 \partial J_3}\)

~~~~{.cxx}
// yield function derivative for Jacobian
    const auto dF_dI1 = sin_phi / 3.;
    const auto dF_dJ2 = K / (2. * rootF) * (K - tan_3_lode * dK_dlode);
    const auto dF_dJ3 = J2 * K * tan_3_lode / (3. * J3 * rootF) * dK_dlode;
    const auto nF = eval(dF_dI1 * id + dF_dJ2 * s + dF_dJ3 * dev_s_squared);

    // building dfeel_ddeel
    const auto Pdev = id4 - (id ^ id) / 3;

    const auto dG_dlode = KG * J2 / (root)*dKG_dlode;
    const auto dG_ddlode =
        J2 / root * (dKG_dlode * dKG_dlode * (1. - J2 * KG * KG / (root * root)) + KG * dKG_ddlode);
    const auto dG_ddlodeJ2 = KG / root * dKG_dlode * (1. - J2 * KG * KG / (2 * root * root));
    const auto dG_ddJ2 =
        -KG * KG * KG * KG / (4. * root * root * root) + dG_dlode * tan_3_lode / (2 * J2 * J2) -
        tan_3_lode / (2 * J2) * (2 * dG_ddlodeJ2 - tan_3_lode / (2 * J2) * dG_ddlode -
                                 3 / (2 * J2 * cos_3_lode * cos_3_lode) * dG_dlode);
    const auto dG_ddJ3 = -tan_3_lode / (3 * J3 * J3) * dG_dlode +
                         tan_3_lode / (3 * J3) * (dG_ddlode * tan_3_lode / (3 * J3) +
                                                  dG_dlode * 1. / (J3 * cos_3_lode * cos_3_lode));
    const auto dG_ddJ2J3 = dG_ddlodeJ2 * tan_3_lode / (3 * J3) -
                           tan_3_lode / (2 * J2) * (dG_ddlode * tan_3_lode / (3 * J3) +
                                                    dG_dlode * 1. / (J3 * cos_3_lode * cos_3_lode));
~~~~

\(f^{el}\), \(\dfrac{\partial f^{el}}{\partial \Delta\,\tepsilonel}\),
\(\dfrac{\partial f^{el}}{\partial \Delta\,\lambda}\) are computed:

~~~~{.cxx}
  // elasticity
    feel += dlam * n;
    dfeel_ddeel += theta * dlam * (dG_dJ2 * Pdev + dG_dJ3 * computeJ3SecondDerivative(sig) +
                                   dG_ddJ2 * (s ^ s) + dG_ddJ3 * (dev_s_squared ^ dev_s_squared) +
                                   dG_ddJ2J3 * ((dev_s_squared ^ s) + (s ^ dev_s_squared))) *
                   D;
    dfeel_ddlam = n;
~~~~

These equations are equivalent to:

\[
\begin{aligned}
  & f^{el} = \Delta\,\tepsilonel - \Delta\,\tepsilonto\ + \Delta\,\lambda\,\tenseur{n} = 0  \\
  & \dfrac{\partial f^{el}}{\partial \Delta\,\tepsilonel} = 
  1 + \Delta \lambda \frac{\partial \tenseur{n}}{\partial \Delta \tepsilonel} =
  1 + \Delta \lambda \frac{\partial \tenseur{n}}{\partial \tenseur{\sigma}} 
  \frac{\partial \tenseur{\sigma}}{\partial \Delta \tepsilonel} \, \\
  & \dfrac{\partial f^{el}}{\partial \Delta\,\lambda} = \tenseur{n}
\end{aligned}
\]


because this implementation takes into account the fact that the `Implicit` DSL
automatically initializes `feel` to the current estimation of
\(\Delta\,\tepsilonel\) and the Jacobian to identity. 
Moreover, the `StandardElasticity` brick automatically subtracts 
\(\Delta\,\tepsilonto\) to `feel`. 

And to finish \(f^{\lambda}\), \(\dfrac{\partial f^{\lambda}}{\partial \Delta\,\lambda}\),
\(\dfrac{\partial f^{\lambda}}{\partial \Delta\,\tepsilonel}\) are computed:

~~~~{.cxx}
 // plasticity
    flam = Fy / D(0, 0);
    dflam_ddlam = strain(0);
    dflam_ddeel = theta * (nF | D) / D(0, 0);
    np = n;
~~~~

The Jacobian can now be computed (see @Eq:Jacobian) 

# References
