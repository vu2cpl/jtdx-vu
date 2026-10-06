! does encode174_91's codeword satisfy bpdecode174_91's parity checks, and does bpdecode return cw(1:77)=message?
program enccheck
  implicit none
  integer*1 message77(77),cw(174),cw2(174),m2(77),apmask(174)
  real llr(174)
  integer i,nharderror,iter
  character(len=77) :: c77='00001001101111011110001101010000011000010100100111011100000111111010100111001'
  integer*1 rvec(77)
  data rvec/0,1,0,0,1,0,1,0,0,1,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,0,1,1,0, &
            1,0,0,1,0,1,1,0,1,0,1,0,1,0,1,1,0,0,0,1,1,0,1,0,0,1,0,0,0,0, &
            1,0,0,0,1,0,0,1,0,0,1,0,1,1,1,0,1,0/
  read(c77,'(77i1)') message77
  message77=mod(message77+rvec,2)
  call encode174_91(message77,cw)
  llr=5.0*(2.0*cw-1.0); apmask=0
  call bpdecode174_91(llr,apmask,30,m2,cw2,nharderror,iter)
  print *,'bpdecode nharderror (0 = encoder cw is a valid codeword):',nharderror
  print *,'bpdecode message == encoder input:',all(m2.eq.message77)
  print *,'bpdecode cw == encoder cw:',all(cw2.eq.cw)
  write(*,'(a,174i1)') 'enc cw ',cw
end program enccheck
