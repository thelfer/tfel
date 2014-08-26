set grid
set term png

p=-70.e6
pi=0.05
A = 8.e-67
E = 8.2
Ap0=(E*(pi**(-1/E)-1))**(-2*E/(E+1));
s0=sqrt(Ap0*p*p)
ds_dp0=Ap0*p/s0
dphi_dp0=A*s0**E*ds_dp0
pv0(t)=dphi_dp0*t
p0(t)=(1-(1-pi)*exp(-pv0(t)))

set xlabel "Temps (s)"
set ylabel "Déformation viscoplastique (&times; 10<sup>3</sup>)"
set output "castemellipticcreep-pv.png"
plot "castemellipticcreep.res" u 1:($20*1e3) t "MTest", \
     pv0(x)*1e3 t "Solution analytique approchée"

set xlabel "Temps (s)"
set ylabel "Porosité (&times; 10<sup>2</sup>)"
set output "castemellipticcreep-porosity.png"
plot "castemellipticcreep.res" u 1:($22*1e2)  t "MTest", \
     p0(x)*1e2 t "Solution analytique approchée"

set xlabel "Itération"
set ylabel "Résidu"
set output "castemellipticcreep-convergence.png"
set logscale y
plot "castemellipticcreep-convergence.txt" u 1:2 t ""

set xlabel "Itération"
set ylabel "Résidu"
set output "castemellipticcreep-convergence-2.png"
set logscale y
plot "castemellipticcreep-convergence.txt" u 1:2 t "sans accélération de convergence", \
     "castemellipticcreep-2-convergence.txt" u 1:2 t "avec accélération de convergence"
