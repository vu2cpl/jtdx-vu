! JTDX-VU: FT2 - FT4's protocol at twice the speed (IU8LMC, Martino, ARI
! Caserta; C++ reference in MSHV by LZ2HV).  Same LDPC(174,91), same 103
! channel symbols with four 4x4 Costas arrays, GFSK BT=1; 288 samples per
! symbol at 12000 S/s (41.67 baud), 3.75 s T/R period.  Ported from JTDX's
! FT4 chain (K1JT/K9AN et al., UA3DJY) by halving every timing constant.
! last time modified by Igor UA3DJY on 20200105

subroutine partintft2(ndelay,nutc)

  use ft2_mod1, only : ddf2
  real rnd

  if(ndelay.gt.50) ndelay=50
  numsamp=nint((float(ndelay))*1200) ! 12000 sample rate
  ddf2(numsamp+1:41472)=ddf2(1:(41472-numsamp))
  do i=1,numsamp
     call random_number(rnd)
     ddf2(i)=10.0*rnd-5.
  enddo
  write(*,2) nutc,'partial loss of data','d'
2 format(i6.6,2x,a20,21x,a1)
  call flush(6)

  return
end subroutine partintft2