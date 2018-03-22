subroutine mxwell(vh,n3,h,tref)
  use mod_setprecision
  implicit none
  integer :: n3
  real (kind=dprec) :: vh(1:n3), h, tref
  integer :: i, iseed, npart, iof1, iof2
  real (kind=dprec) :: ujunk, tscale, u1, u2, v1, v2, s, r, ekin, sp, sc, ts
  real (kind=dprec) :: random
!
! sample maxwell distribution at temperature tref
!
! alliant 3
  iseed = 4711
  ujunk = random(iseed)
  iseed = 0
  npart = n3/3
  iof1 = npart
  iof2 = npart*2
  tscale = 16.0d0 / (1.0d0 * npart - 1.0d0)
  do i = 1,n3,2
!
! cray 2
!1         u1 = ranf()
!          u2 = ranf()
! alliant 2
1    u1 = random(iseed)
     u2 = random(iseed)
     v1 = 2.0d0 * u1 - 1.0d0
     v2 = 2.0d0 * u2 - 1.0d0
     s = v1*v1 + v2*v2
     if (s >= 1.0) goto 1
!
     r = sqrt(-2.0d0*dlog(s)/s)
     vh(i) = v1 * r
     vh(i+1) = v2 * r
  end do
!
  ekin = 0.0d0
  sp = 0.0d0
  do i = 1,npart
     sp = sp + vh(i)
  end do
  sp = sp / npart
  do i = 1,npart
     vh(i) = vh(i) - sp
     ekin = ekin + vh(i)*vh(i)
  end do
  sp = 0.0d0
  do i = iof1 + 1,iof2
     sp = sp + vh(i)
  end do
  sp = sp / npart
  do i = iof1 + 1,iof2
     vh(i) = vh(i) - sp
     ekin = ekin + vh(i)*vh(i)
  end do
  sp = 0.0d0
  do i = iof2 + 1,n3
     sp = sp + vh(i)
  end do
  sp = sp / npart
  do i = iof2 + 1,n3
     vh(i) = vh(i) - sp
     ekin = ekin + vh(i)*vh(i)
  end do
  ts = tscale * ekin
  sc = h * sqrt(tref/ts)
  do i = 1,n3
     vh(i) = vh(i) * sc
  end do
!
end subroutine mxwell
