program txtest2
  ! JTDX-VU: exercise the GUI FT2 TX path - genft4 tones -> gen_ft2wave at
  ! 48 kHz / 1152 samples per symbol - decimate to 12 kHz, place at 0.5 s in a
  ! 3.75 s period and write a wav for ft2dec
  use wavhdr
  implicit none
  type(hdr) :: h
  character(len=37) :: msg,msgsent
  integer :: itone(105),nsym,nsps4,icmplx,nwave,ichk,ntxhash,i,n12
  integer(1) :: msgbits(77)
  real :: fsample,f0
  real, allocatable :: wave(:)
  integer(2) :: iwave(45000)
  msg='VU2CPL K1ABC RR73'; ichk=0; ntxhash=1
  call genft4(msg,ichk,ntxhash,msgsent,msgbits,itone)
  print*,'sent: ',trim(msgsent)
  nsym=103; nsps4=1152; fsample=48000.0; f0=1500.0; icmplx=0
  nwave=(nsym+2)*nsps4
  allocate(wave(nwave))
  call gen_ft2wave(itone,nsym,nsps4,fsample,f0,wave,wave,icmplx,nwave)
  print*,'nwave=',nwave,' secs=',nwave/48000.0
  n12=nwave/4
  iwave=0
  do i=1,n12
     iwave(6000+i)=nint(8000.0*wave(4*i-3))
  enddo
  h=default_header(12000,45000)
  open(10,file='txtest2.wav',access='stream',status='replace')
  write(10) h,iwave
  close(10)
end program txtest2
