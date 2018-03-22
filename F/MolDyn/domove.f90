subroutine domove(n3,x,vh,f,side)
  use mod_setprecision
  implicit none
  real (kind=dprec) :: x(n3), vh(n3), f(n3)
  real (kind=dprec) :: side
  integer :: n3, i
!
!     move particles
!
  do i = 1,n3
     x(i) = x(i) + vh(i) + f(i)
!     periodic boundary conditions
     if (x(i) < 0.0d0) x(i) = x(i) + side
     if (x(i) > side) x(i) = x(i) - side
!     partial velocity updates
     vh(i) = vh(i) + f(i)
!     initialise forces for next iteration
     f(i) = 0.0d0
  end do
!
end subroutine domove
