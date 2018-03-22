subroutine velavg(npart, vh, vaver, count, vel, h)
  use mod_setprecision
  implicit none
  integer :: npart, i 
  real (kind=dprec) :: vh(npart, 3), vaver, count, vel, h, vaverh, sq

!     
!     compute average velocity
!     
  vaverh = vaver*h
  vel = 0.0d0
  count = 0.0d0
  do i = 1,npart
     sq = sqrt(vh(i,1)**2 + vh(i,2)**2 + vh(i,3)**2)
     if (sq > vaverh) count = count + 1.0d0
     vel = vel + sq
  end do
  vel = vel / h
!     
end subroutine velavg
