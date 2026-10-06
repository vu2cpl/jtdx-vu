! JTDX-VU AP7 unit check: call a7d directly on a strong known signal with the
! pair supplied, so the matcher's winner and margin are seen without the
! "already decoded" skip.  usage: a7unit file.wav CALL1 CALL2 GRID4 freq
program a7unit
  use wavhdr
  use ft2_decode
  use ft2_mod1
  use ft8_mod1, only : twopi,mycall,hiscall
  implicit none
  type(hdr) :: h
  type(ft2_decoder) :: dec
  character(len=200) :: fname
  character(len=13) :: c1,c2
  character(len=4) :: g4
  character(len=16) :: arg
  character(len=37) :: message
  integer(2) :: iwave(41472)
  integer :: nread,nharderrors
  real :: xdt,f0,xsnr
  call get_command_argument(1,fname)
  call get_command_argument(2,arg); c1=arg(1:13)
  call get_command_argument(3,arg); c2=arg(1:13)
  call get_command_argument(4,arg); g4=arg(1:4)
  call get_command_argument(5,arg); read(arg,*) f0
  twopi=8.0*atan(1.0); mycall=' '; hiscall=' '
  llagcc2=.false.; lfilter2=.false.; lhidetest2=.false.; lhidetelemetry2=.false.
  open(10,file=trim(fname),status='old',access='stream'); read(10) h
  nread=min(41472,h%ndata/2); iwave=0; read(10) iwave(1:nread); close(10)
  ddf2=iwave
  xdt=0.0; message=''; nharderrors=-1
  call a7d(dec,c1,c2,g4,xdt,f0,nharderrors,message,xsnr)
  print '(a,i4,a,a,a,f7.1,a,f6.1)', 'a7d: nharderrors=',nharderrors,'  message=[',trim(message),']  f=',f0,'  snr=',xsnr
end program a7unit
