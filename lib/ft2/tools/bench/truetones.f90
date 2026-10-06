! the true 103 channel tones (incl. Costas) for a message, as the decoder's own mapper makes them
program truetones
  use packjt77
  implicit none
  character(len=37) :: msg
  character(len=77) :: c77
  integer*1 :: message77(77),rvec(77)
  integer :: i3,n3,i4tone(103)
  character(len=100) :: arg
  data rvec/0,1,0,0,1,0,1,0,0,1,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,0,1,1,0, &
      1,0,0,1,0,1,1,0,0,0,0,1,0,0,0,1,0,1,0,0,1,1,1,1,0,0,1,0,1, &
      0,1,0,1,0,1,1,0,1,1,1,1,1,0,0,0,1,0,1/
  call get_command_argument(1,arg); msg=arg(1:37)
  i3=-1; n3=-1
  call pack77(msg,i3,n3,c77,1)
  read(c77,'(77i1)') message77
  call get_ft4_tones_from_77bits(message77,i4tone)
  write(*,'(a,103i1)') 'TRUE tones ',i4tone
end program truetones
