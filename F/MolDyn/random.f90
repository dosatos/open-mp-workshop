function random(is)
  use mod_setprecision
! should work on any 32 bit machine. actual generator
! is only fair and not suitable for detailed work.
  implicit none
  real (kind=dprec) :: random
  integer :: is,imult,imod,is1,is2,iss2
  real (kind=sprec) :: c, scale
  data imult/16807/,imod/2147483647/,scale/4.656612875d-10/
!
! is = mod(is*16807,2**31-1).
  if(is <= 0) is = 1
  is2 = mod(is,32768)
  is1 = (is-is2)/32768
  iss2 = is2 * imult
  is2 = mod(iss2,32768)
  is1 = mod(is1*imult+(iss2-is2)/32768,65536)
  is = mod(is1*32768+is2,imod)
  c = scale * dfloat(is)
  random=c
  return
end function random
