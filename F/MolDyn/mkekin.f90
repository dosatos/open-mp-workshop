subroutine mkekin(npart,f,vh,hsq2,hsq,ekin)
  use mod_setprecision
  implicit none
  integer :: npart
  real (kind=dprec) :: f(1:npart,3),vh(1:npart,3)
  real (kind=dprec) :: hsq, hsq2, ekin, sum
  integer :: ix, i
  
!
!     scale forces, update velocites and compute k.e.
!
  sum = 0.0d0
  do ix = 1,3
     do i = 1,npart
        f(i,ix) = f(i,ix) * hsq2
        vh(i,ix) = vh(i,ix) + f(i,ix)
        sum = sum + vh(i,ix) * vh(i,ix)
     end do
  end do
  ekin = sum / hsq
!
end subroutine mkekin
