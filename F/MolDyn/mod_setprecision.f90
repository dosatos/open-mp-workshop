module mod_setprecision
! Sets up parameters to handle single (sp) and double precision (dp)
! On lomond these correspond to 4-byte and 8-byte reals respectively. 
  implicit none

  integer, parameter :: sprec = kind(1.0)
  integer, parameter :: dprec = kind(1.0d0)

end module mod_setprecision
