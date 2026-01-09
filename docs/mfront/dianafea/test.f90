program main
  integer :: ns
  real(kind=8), dimension(6) :: eps0
  real(kind=8), dimension(6) :: deps
  real(kind=8) :: age0
  real(kind=8) :: dtime
  real(kind=8) :: temp0
  real(kind=8) :: dtemp
  integer :: elemen
  integer :: intpt
  real(kind=8) :: coord(3)
  real(kind=8) :: se(6,6)
  integer :: iter
  character(len=6) :: usrmod
  real(kind=8) :: usrval(1)
  integer :: nuv
  real(kind=8) :: usrsta(1)
  integer :: nus
  integer :: usrind(1)
  integer :: nui
  real(kind=8) :: sig(6)
  real(kind=8) :: stiff(6,6)

  ns = 6
  nuv = 1
  nus = 1
  nui = 1
  usrmod = 'TEST  '
  temp0 = 293.15
  dtemp = 64.21
  
  call USRMAT(eps0, deps, ns, age0, dtime, temp0, &
       dtemp, elemen, intpt, coord, se, iter, &
       usrmod, usrval, nuv, usrsta, nus, &
       usrind, nui, sig, stiff)

end program main
