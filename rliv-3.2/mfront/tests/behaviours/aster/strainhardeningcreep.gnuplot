A=8.e-40
Ns=4.2
Np=1.37
seq=20.e6
p0=1.e-7
# ((p+p0)**(Np+1)-p0**(Np+1)) = (Np+1)*A*seq**Ns*t
# ((p+p0)**(Np+1)-p0**(Np+1)) = (Np+1)*A*seq**Ns*t
p(t)=(p0**(Np+1)+(Np+1)*A*seq**Ns*t)**(1/(Np+1))-p0

plot [0:3600] p(x)*100, "/tmp/strainhardeningcreep.res" u 1:($14*100) w l 