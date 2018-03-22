program main
  use mod_setprecision
  use omp_lib
  implicit none
  integer, parameter :: mm = 15, npart = 4*mm*mm*mm
  integer :: irep, istop, iprint, movemx, npartm, move
!     
  real (kind=dprec) :: x(1:npart,1:3), vh(1:npart,1:3),f(1:npart,1:3)
  real (kind=dprec) :: den, side, tref, rcoff, h, a, sideh, hsq, hsq2
  real (kind=dprec) :: rcoffs, tscale, vaver, vir, start, ekin, epot
  real (kind=dprec) :: sc, time, count, vel
!     
!     
  den = 0.83134d0
  side = (dfloat(npart) / den)**0.3333333d0
  tref = 0.722d0
  rcoff = dfloat(mm) / 4.0d0
  h = 0.064d0
  irep = 10
  istop = 20
  iprint = 5
  movemx = 20
!
  write(6,1) npart,side,rcoff,tref,h,irep,istop,iprint,movemx
1 format(' molecular dynamics simulation example program'/&
       ' ---------------------------------------------'//&
       ' number of particles is ............ ',i6/&
       ' side length of the box is ......... ',f13.6/&
       ' cut off is ........................ ',f13.6/&
       ' reduced temperature is ............ ',f13.6/&
       ' basic time step is ................ ',f13.6/&
       ' temperature scale interval ........ ',i6/&
       ' stop scaling at move .............. ',i6/&
       ' print interval .................... ',i6/&
       ' total no. of steps ................ ',i6)
  call flush(6)
!     
  a = side / dfloat(mm)
  sideh = side * 0.5d0
  hsq = h * h
  hsq2 = hsq * 0.5d0
  npartm = npart - 1
  rcoffs = rcoff * rcoff
  tscale = 16.0d0 / (1.0d0 * npart - 1.0d0)
  vaver = 1.13d0 * sqrt(tref / 24.0d0)
!     
!     generate fcc lattice for atoms inside box
!     
  call fcc(x, npart, mm, a)
!     
!     initialise velocites and forces (which are zero in fcc positions)
!     
  call mxwell(vh,3*npart,h,tref)
  call dfill(3*npart, 0.0d0, f, 1)
!     
!     start of md. 
!     
  write(6,3)
3 format(//1x,'   i  ','     ke    ','      pe     ','      e     ',&
       '    temp   ','   pres   ','   vel    ','  rp'/&
       1x,' -----',3('  ----------'),3('  --------'),'  ----')
  call flush(6)
  start = omp_get_wtime()
!  
  do move = 1,movemx
!     
!     move the particles and partially update velocities
!     
     call domove(3*npart,x,vh,f,side)
!     
!     compute forces in the new positions and accumulate the 
!     virial and potential energy.
!     
     call forces(npart, x, f, vir, epot, side, rcoff)
!
!     scale forces, complete update of velocites and compute k.e.
!
     call mkekin(npart,f,vh,hsq2,hsq,ekin)
!
!     average the velocity and temperature scale if desired
!
     call velavg(npart, vh, vaver, count, vel, h)
     if ((move < istop) .and. (mod(move, irep) == 0)) then
        sc = sqrt( tref / (tscale * ekin) )
        call dscal(3*npart, sc, vh, 1)
        ekin = tref / tscale
     endif
!
!     sum to get full potential energy and virial
!
     if (mod(move, iprint) == 0) then
        call prnout(move, ekin, epot, tscale, vir, vel, count, npart, den)
     endif
!
  end do
  time = omp_get_wtime() - start
  write(6,*) ' ' 
  write(6,*) 'Time = ', real(time) 
!
end program main

