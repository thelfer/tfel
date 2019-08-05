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
	A &= \frac{1}{3} \cos \theta_\mathrm{T} \left[ 3 + \tan \theta_\mathrm{T} \tan 3\theta_\mathrm{T} + \frac{1}{\sqrt{3}} \mathrm{sign\,} \theta (\tan 3\theta_\mathrm{T} - 3\tan \theta_\mathrm{T}) \sin \phi \right] \\
	%
	B &= \frac{1}{3} \frac{1}{\cos 3\theta_\mathrm{T}} \left[ \mathrm{sign\,} \theta \sin \theta_\mathrm{T} + \frac{1}{\sqrt{3}} \sin \phi \cos \theta_\mathrm{T} \right]
\end{aligned}
\]{#eq:Sloan}

The plastic potential differs from the yield surface in order to more
accurately estimate dilatancy, but has an analogous structure:

\[
	G_\mathrm{F} = \frac{I_1}{3} \sin \psi + \sqrt{J_2 K'^2 + a^2 \sin^2 \psi} - c \cos \psi
\]{#eq:plastic_potential}

where \(\psi\) is the dilatancy angle. \(K'\), \(A'\) and \(B'\) follow
from @eq:K_abbo and @eq:Sloan by substituting the friction angle
with the dilatancy angle.

![Effect of the smoothing of the yield surface](img/MohrCoulombSmoothing.svg "Effect of the smoothing of the yield surface"){#fig:mc_vis width=75%}

The contribution of the smoothing is visualized in Figure @fig:mc_vis.

# Flow rule

Plastic flow follows in a general manner for a
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
	\frac{K'^2}{2\sqrt{J_2 K'^2 + a^2 \sin^2 \psi}}
	\\
	\left. \frac{\partial G_\mathrm{F}}{\partial \theta} \right|_{I_1,J_2} &= 
	\frac{J_2K'}{\sqrt{J_2 K'^2 + a^2 \sin^2 \psi}} \frac{\partial K'}{\partial \theta}
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
	\frac{\partial G_\mathrm{F}}{\partial \theta}\frac{\partial \theta}{\partial J_2} &= -\frac{K'\tan 3\theta}{2\sqrt{J_2 K'^2 + a^2 \sin^2 \psi}} \frac{\partial K'}{\partial \theta} 
\end{aligned}
\]

Thus,
\[
\begin{aligned}
	\left. \frac{\partial G_\mathrm{F}}{\partial I_1} \right|_{J_2,J_3} &= \frac{1}{3} \sin \psi
	\\
	\left.\frac{\partial G_\mathrm{F}}{\partial J_2}\right|_{I_1,J_3} &= 
	\frac{K'}{2\sqrt{J_2 K'^2 + a^2 \sin^2 \psi}} \left(K' - \tan 3\theta \frac{\partial K'}{\partial \theta} \right)
	\\
	\left.\frac{\partial G_\mathrm{F}}{\partial J_3}\right|_{I_1,J_2} &= \frac{J_2K'\tan 3\theta}{3J_3\sqrt{J_2 K'^2 + a^2 \sin^2 \psi}} \frac{\partial K'}{\partial \theta}
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
	-\frac{K'^4}{4\sqrt{(J_2 K'^2 + a^2 \sin^2 \psi)^3}}
	\\
	\frac{\partial^2 G_\mathrm{F}}{\partial \theta^2} &= 
	\left( \frac{J_2}{\sqrt{J_2 K'^2 + a^2 \sin^2 \psi}} - \frac{J_2^2 K'^2}{\sqrt{(J_2 K'^2 + a^2 \sin^2 \psi)^3}} \right)\left(\frac{\partial K'}{\partial \theta}\right)^2 \\
	&+  \frac{J_2K'}{\sqrt{J_2 K'^2 + a^2 \sin^2 \psi}} \frac{\partial^2 K'}{\partial \theta^2}  
\\
	&=  \frac{J_2}{\sqrt{J_2 K'^2 + a^2 \sin^2 \psi}} \left[\left(\frac{\partial K'}{\partial \theta}\right)^2 \left(1 - \frac{J_2K'^2}{J_2 K'^2 + a^2 \sin^2 \psi} \right) + K' \frac{\partial^2 K'}{\partial \theta^2}  \right]
	\\
	\frac{\partial^2 G_\mathrm{F}}{\partial J_2 \partial \theta} &= 
	\left( \frac{K'}{\sqrt{J_2 K'^2 + a^2 \sin^2 \psi}} - \frac{J_2 K'^3}{2\sqrt{(J_2 K'^2 + a^2 \sin^2 \psi)^3}} \right)\frac{\partial K'}{\partial \theta}
	\\
	&=\frac{K'}{\sqrt{J_2 K'^2 + a^2 \sin^2 \psi}}\frac{\partial K'}{\partial \theta}\left( 1 - \frac{J_2 K'^2}{2(J_2 K'^2 + a^2 \sin^2 \psi)} \right)
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
    \tenseurq{I} + \Delta \lambda \frac{\partial \tenseur{n}}{\partial \Delta \tepsilonel} & \tenseur{n} \\
    E^{-1}\tenseur{n}_F \dcdot \tenseurq{C} & 0
\end{pmatrix}
\]

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

![Trace of the yield surface (green) when approached on different stress paths (purple)](img/MohrCoulombPiPlane.svg "Trace of the yield surface (green) when approached on different stress paths (purple)"){#fig:MC_stress_paths width=50%}

The yield function was approached along different stress paths, shown in
Figure @fig:MC_stress_paths within the \(\pi\)-plane. This shows that
a) yield is correctly detected, and b) the stress-state is correctly
pulled back onto the yield surface.

![Verification against analytical example. Description in @Nagel2016.](img/MohrCoulombAnalitical.svg "Verification against analytical example. Description in @Nagel2016"){#fig:MC_vis width=75%}

The second test (included as a benchmark) follows an analytical solution
for a stress-free cavity in an infinite medium under a variable
far-field stress. The solution computes stress- and displacement fields
as well as the location of the plastified zone. It has been used in
@Nagel2016 where the test is described in more detail (with a
partial extension towards non-associated flow).

# References
