subroutine prnout(move, ekin, epot, tscale, vir, vel, count, npart, den)
  use mod_setprecision
  implicit none
  integer :: move, npart
  real (kind=dprec) :: ekin, epot, tscale, vir, vel, count, den
  real (kind=dprec) :: ek, etot, temp, pres, rp
!
!     printout interesting (?) information at current timestep
!
  ek = 24.0d0 * ekin
  epot = 4.0d0 * epot
  etot = ek + epot
  temp = tscale * ekin
  pres = den * 16.0d0 * (ekin - vir) / npart
  vel = vel / npart
  rp = (count / dfloat(npart)) * 100.0d0
  write(6,2) move,ek,epot,etot,temp,pres,vel,rp
2 format(1x,i6,3f12.4,f10.4,f10.4,f10.4,f6.1)
  call flush(6)
!
end subroutine prnout
