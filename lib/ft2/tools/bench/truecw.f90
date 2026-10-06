! print the true 174-bit codeword (post-rvec, as the decoder compares it) for a message
program truecw
  use packjt77
  implicit none
  character(len=37) :: msg,msgsent
  character(len=77) :: c77
  integer*1 :: message77(77),rvec(77),cw(174)
  integer :: i3,n3,z
  logical :: ok
  character(len=100) :: arg
  data rvec/0,1,0,0,1,0,1,0,0,1,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,0,1,1,0, &
      1,0,0,1,0,1,1,0,0,0,0,1,0,0,0,1,0,1,0,0,1,1,1,1,0,0,1,0,1, &
      0,1,0,1,0,1,1,0,1,1,1,1,1,0,0,0,1,0,1/
  call get_command_argument(1,arg); msg=arg(1:37)
  i3=-1; n3=-1
  call pack77(msg,i3,n3,c77,1)
  read(c77,'(77i1)') message77
  message77=mod(message77+rvec,2)
  call encode174_91(message77,cw)
  write(*,'(a,174i1)') 'TRUE cw ',cw
end program truecw
