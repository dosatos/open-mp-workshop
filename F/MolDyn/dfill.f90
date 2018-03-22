subroutine dfill(n,val,a,ia)
  use mod_setprecision
  implicit none
  integer :: i, ia, n
  real (kind=dprec) :: a(3*n), val
!
!     initialise double precision (kind = 1.0d0) array to scalar value
!
  do i = 1,(n-1)*ia+1,ia
     a(i) = val
  end do
!
end subroutine dfill
