subroutine fcc(x, npart, mm, a)
  use mod_setprecision
  implicit none

  integer :: npart, mm
  real (kind=dprec) :: a, x(1:npart, 3)
  integer :: ijk, lg,  i, j, k 
!     
!     generate fcc lattice for atoms inside box
!     
  ijk = 0
  do lg = 0,1
     do i = 0,mm-1
        do j = 0,mm-1
           do k = 0,mm-1
              ijk = ijk + 1
              x(ijk,1) = i * a + lg * a * 0.5d0
              x(ijk,2) = j * a + lg * a * 0.5d0
              x(ijk,3) = k * a
           end do
        end do
     end do
  end do
  do lg = 1,2
     do i = 0,mm-1
        do j = 0,mm-1
           do k = 0,mm-1
              ijk = ijk + 1
              x(ijk,1) = i * a + (2-lg) * a * 0.5d0
              x(ijk,2) = j * a + (lg-1) * a * 0.5d0
              x(ijk,3) = k * a + a * 0.5d0
           end do
        end do
     end do
  end do
!
end subroutine fcc
