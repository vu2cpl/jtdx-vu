! JTDX-VU: FT2 - FT4's protocol at twice the speed (IU8LMC, Martino, ARI
! Caserta; C++ reference in MSHV by LZ2HV).  Same LDPC(174,91), same 103
! channel symbols with four 4x4 Costas arrays, GFSK BT=1; 288 samples per
! symbol at 12000 S/s (41.67 baud), 3.75 s T/R period.  Ported from JTDX's
! FT4 chain (K1JT/K9AN et al., UA3DJY) by halving every timing constant.
! This source code file was last time modified by Igor UA3DJY on 20190429
! All changes are shown in the patch file coming together with the full JTDX source code.

subroutine agccft2()

  use ft2_mod1, only : ddf2
  integer, parameter :: NFFT=8192,NSZ=3413 !3413 = NFFT*5000/12000
  real*4 x11(NFFT),w11(NFFT),ss33(NSZ)
  complex c11(0:NFFT/2)
  integer indx(NSZ)
  logical(1) first
  equivalence (x11,c11)
  data first/.true./
  save first,w11

  fac1=7.e-4

  if(first) then ! Compute the FFT window
    twopi=8.d0*atan(1.d0)
    do k=1,NFFT; w11(k)=sin(twopi*(k+2)/16384); enddo
    first=.false.
  endif

  x11=fac1*w11*ddf2(1:8192); call four2a(c11,NFFT,1,-1,0) !r2c forward FFT
  do i=1,NSZ; s33=ABS(c11(i)); ss33(i)=SQRT(s33); enddo
  call indexx(ss33(1:nsz),nsz,indx)
  smed=ss33(indx(1707)); if(smed.gt.1.E-6) then; s3start=smed; else; s3start=1.0; endif

  x11=fac1*w11*ddf2(33281:41472); call four2a(c11,NFFT,1,-1,0) !r2c forward FFT
  do i=1,NSZ; s33=ABS(c11(i)); ss33(i)=SQRT(s33); enddo
  call indexx(ss33(1:nsz),nsz,indx)
  smed=ss33(indx(1707)); if(smed.gt.1.E-6) then; s3end=smed; else; s3end=1.0; endif

  ddf2(1:8192)=ddf2(1:8192)/s3start; ddf2(33281:41472)=ddf2(33281:41472)/s3end

  return
end subroutine agccft2