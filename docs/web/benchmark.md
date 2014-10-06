% Performance assessments
% Helfer Thomas; Jean-Michel Proix
% August 20, 2014

Numerous performances assessments were made within the PLEIADES
platform: replacing fortran implementations by their MFront
counterparts led to significant improvements, from \(30\%\) to
\(50\%\) of the total computational times of some fuel performance
codes developed in the platform. This improvements were mainly due to
fact that the behaviour integration schemes changed from explicit
{Runge-Kutta} schemes to implicit ones. The main benefit of
MFront was to grant users an easier access to the implicit schemes.

<!-- \begin{table} -->
<!--   \centering -->
<!-- \begin{supertabular}{|>{\centering\small}m{3.5cm}|>{\centering}m{2.cm}|>{\centering}m{1.cm}|>{\centering}m{1cm}|>{\centering}>{\centering}m{4.cm}|} -->
<!-- \hline -->
<!-- \multicolumn{1}{|>{\centering}m{3.5cm}|}{\bfseries Behaviour and test description} & -->
<!-- \multicolumn{1}{ >{\centering}m{2.cm}|}{\bfseries Algorithm}  & -->
<!-- \multicolumn{2}{>{\centering}m{2cm}|}{\bfseries Total computational times} &  -->
<!-- \multicolumn{1}{>{\centering}m{4cm}|}{\bfseries Graphical illustration} -->
<!-- \tabularnewline -->
<!-- \cline{3-4} -->
<!-- & & Native & MFront & \tabularnewline -->
<!-- \hline -->
<!-- \hline -->
<!-- Visco-plastic and damaging for -->
<!-- steel [@mustata_creep_2005,edf_comportement_2012] - 3D Notched -->
<!-- specimen implying large deformation -->
<!-- & -->
<!-- {\tt Implicit}, 10 -->
<!-- equations & -->
<!-- 17mn 43s &7mn 58s -->
<!--  & -->
<!--  \includegraphics[height=3.9cm,angle=90]{Behaviour-img2.eps} -->
<!--  \tabularnewline -->
<!--  \hline -->
<!--  Damaging for -->
<!--  concrete [@mazars_new,edf_modeendommagement_2013], 3D beam -->
<!--  bending & Default parser & 45mn & 63mn & -->
<!--  \includegraphics[width=3.9cm]{Behaviour-img3.eps} \tabularnewline -->
<!-- \hline -->
<!-- Generic Single crystal -->
<!-- viscoplasticity [@meric_single_1991,edf_comportements_2013-1], 3D -->
<!-- aggregate, 300 grains -->
<!--  & -->
<!-- Implicit, 18 equations & -->
<!-- 28mn & 24mn -->
<!--  & -->
<!--  \includegraphics[width=3.9cm]{Behaviour-img5.eps} -->
<!--  \tabularnewline \hline FCC single crystal -->
<!--  viscoplasticity [@monnet_orowan_2011,edf_comportements_2013-1], -->
<!--  2D specimen with displacement boundary conditions from EBSD -->
<!--  experiment & Implicit, 18 equations & 33 m 54 s & 29 m 30 s & -->
<!--  \includegraphics[width=3.9cm]{Behaviour-img6.eps} -->
<!--  \tabularnewline \hline FCC homogeneized -->
<!--  polycrystals [@berveiller_extension_1978,edf_comportements_2013-1], -->
<!--  unit testing & Runge-Kutta 4/5, 30 grains, 1272 equations & 9s67 & -->
<!--  8s22 & -->
<!-- \includegraphics[width=3.cm]{Behaviour-img8.eps} -->
<!-- \tabularnewline -->
<!-- % \hline -->
<!-- % Generic homogeneized FCC POLYCRISTAL, 100 grains, 4212 equations, unit testing -->
<!-- % & -->
<!-- % {\tt Runge-Kutta 4/5}, -->
<!-- % & -->
<!-- % 134s & 89s & -->
<!-- %\tabularnewline -->
<!-- \hline -->
<!-- Anisotropic creep with phase transformation, 3D -->
<!-- pipe [@edf_modecomportement_2013] -->
<!-- & -->
<!-- Implicit  & -->
<!-- 180s & 171s & -->
<!-- \includegraphics[width=3.9cm]{Behaviour-img9.eps} -->
<!-- \tabularnewline -->
<!-- \hline -->
<!-- \end{supertabular} -->
<!-- \label{mfront:overview:benchmark} -->
<!-- \caption{Some benchmarks comparing the -->
<!--   implementation generated through MFront to the native -->
<!--   implementation provided by the Code-Aster finite element solver. -->
<!--   Graphical illustrations shows that the results obtained with both -->
<!--   implementations are indistinguishable.} -->
<!-- \end{table} -->

Developers of the Code-Aster general purpose finite element solver,
made independent extensive tests, comparing their own native
implementations to the ones generated with MFront, generally using an
implicit scheme in both cases. Without discussing the very details of
each test performed, several general conclusions can be drawn:

-  native implementations offers superior performances in the case
  of simple explicit behaviours (Mazars damaging
  behaviour [@mazars_new]) in the case of isotropic
  behaviours that can be reduce to one scalar
  equations [@chaboche_integration_1996}. For explicit behaviours,
  the difference will be reduced by the development of an optimised
  treatment of MFront behaviours. In the second case, the
  difference can be explained by the fact that the Code-Aster
  implementations uses the Brent
  algorithm [@brent_algorithms_1973] which clearly outperforms the
  standard Newton method. The availability of this algorithm in
  MFront is planed.
-  for more complex behaviours, MFront implementation are on par
  or outperforms the native implementations.

For a given behaviour, the development time was found significantly
lower with MFront.

# Réferences
