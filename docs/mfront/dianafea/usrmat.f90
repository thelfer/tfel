subroutine USRMAT(eps0, deps, ns, age0, dtime, temp0, &
     dtemp, elemen, intpt, coord, se, iter, &
     usrmod, usrval, nuv, usrsta, nus, &
     usrind, nui, sig, stiff)
  implicit none
  real(kind=8), intent(in) :: eps0(ns)
  real(kind=8), intent(in) :: deps(ns)
  integer, intent(in) :: ns
  real(kind=8), intent(in) :: age0
  real(kind=8), intent(in) :: dtime
  real(kind=8), intent(in) :: temp0
  real(kind=8), intent(in) :: dtemp
  integer, intent(in) :: elemen
  integer, intent(in) :: intpt
  real(kind=8), intent(in) :: coord(3)
  real(kind=8), intent(in) :: se(ns,ns)
  integer, intent(in) :: iter
  character(len=6), intent(in) :: usrmod
  real(kind=8), intent(in) :: usrval(nuv)
  integer, intent(in) :: nuv
  real(kind=8), intent(out) :: usrsta(nus)
  integer, intent(in) :: nus
  integer, intent(in) :: usrind(nui)
  integer, intent(in) :: nui
  real(kind=8), intent(out) :: sig(ns)
  real(kind=8), intent(out) :: stiff(ns,ns)
  integer :: nus2
  integer :: nuv2
    interface
       subroutine dianafea_external_behaviour_call(sig, ddsdde, statev, &
            cname, ntens, nprops, nstatv, eto, deto, dt, props, temp, dtemp) &
            bind(c,name = 'dianafea_external_behaviour_call')
         use, intrinsic :: iso_c_binding, only: c_char, c_double, c_int
         implicit none
         real(kind=c_double), intent(out) :: sig(ntens)
         real(kind=c_double), intent(out) :: ddsdde(ntens, ntens)
         real(kind=c_double), intent(out) :: statev(nstatv)
         character(len=1, kind=c_char), intent(in) :: cname(6)
         integer(kind=c_int), intent(in), value :: ntens
         integer(kind=c_int), intent(in), value :: nprops
         integer(kind=c_int), intent(in), value :: nstatv
         real(kind=c_double), intent(in) :: eto(ntens)
         real(kind=c_double), intent(in) :: deto(ntens)
         real(kind=c_double), intent(in), value :: dt
         real(kind=c_double), intent(in) :: props(nprops)
         real(kind=c_double), intent(in), value :: temp
         real(kind=c_double), intent(in), value :: dtemp
       end subroutine dianafea_external_behaviour_call
    end interface

! if no material properties are defined, DIANA sets nuv to -1 where MFront would expect 0
    if (nus .eq. -1) then
       nus2 = 0
    else
       nus2 = nus
    endif
    
! if no state variables are defined, DIANA sets nus to -1 where MFront would expect 0
    if (nuv .eq. -1) then
       nuv2 = 0
    else
       nuv2 = nuv
    endif

    ! write(*,*) "eto: ", eps0
    ! write(*,*) "deto: ", deps
    write(*,*) "ns: ", ns
    
    call dianafea_external_behaviour_call(sig, stiff, usrsta, usrmod, &
         ns, nuv2, nus2, eps0, deps, dtime, usrval, temp0, dtemp)

    ! write(*,*) "sig: ", sig
    ! write(*,*) "stiff: ", stiff

    ! call exit(-1)
    
end subroutine USRMAT
