set term eps
set grid
set key right bottom

r(x)= abs(x)<=1 ? x*x*(2-x*x) : 1

set output "r-epsilon.eps"
plot [-1:1] r(x) t "r<sub>1</sub>(x)", \
     r(x/0.25) t "r<sub>&frac14;</sub>(x)", \
     r(x/0.5)  t "r<sub>&frac12;</sub>(x)"

set output "r-epsilon-2.eps"
plot [-2:2] r(x)/x t "<sup>r<sub>1</sub>(x)</sup>&frasl;<sub>x</sub>", \
     r(x/0.5)/x t "<sup>r<sub>1</sub>(x)</sup>&frasl;<sub>x</sub>"

