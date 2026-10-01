program txtest
  ! JDTX-VU: exercise the exact GUI TX path - genjtty_profile -> gen_jttywave at
  ! 48 kHz / 1536 samples per symbol - decimate to 12 kHz and write a wav for rjtty
  use wavhdr
  implicit none
  type(hdr) :: h
  character(len=80) :: msg
  integer :: itone(16*59),nsym,nsps4,icmplx,nwave,profile,i,n12
  real :: bt,fsample,f0
  real, allocatable :: wave(:)
  integer(2), allocatable :: iwave(:)
  ! message from the command line, e.g. ./txtest "CQ PU5SIX DE VU2CPL, VU2OY/P K"
  msg='CQ VU2CPL CQ'
  if (command_argument_count() >= 1) call get_command_argument(1,msg)
  profile=0
  call genjtty_profile(msg,profile,itone,nsym)
  print*,'nsym=',nsym
  nsps4=1536; bt=2.0; fsample=48000.0; f0=1500.0; icmplx=0
  nwave=nsps4*nsym
  allocate(wave(nwave))
  call gen_jttywave(itone,nsym,nsps4,bt,fsample,f0,wave,wave,icmplx,nwave)
  print*,'nwave=',nwave,' peak=',maxval(abs(wave)),' secs=',nwave/48000.0
  n12=nwave/4
  allocate(iwave(n12+48000))          ! half a second before, 3.5 s after
  iwave=0
  do i=1,n12
     iwave(6000+i)=nint(32000.0*wave(4*i-3))
  enddo
  h=default_header(12000,n12+48000)
  open(10,file='txtest.wav',access='stream',status='replace')
  write(10) h,iwave
  close(10)
end program txtest
