subroutine dscal (n,sa,sx,incx)
  use mod_setprecision
  implicit none

  integer :: n, incx, i, j
  real (kind=dprec) :: sa, sx(3*n)

  if (incx == 1) then
     do  i=1,n
        sx(i) = sx(i) * sa
     end do
  else
     j=1
     do  i=1,n
        sx(j) = sx(j) * sa
        j = j + incx
     end do
  endif
  return
end subroutine dscal
