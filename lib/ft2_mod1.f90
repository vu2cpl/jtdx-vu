! JTDX-VU: FT2 - FT4's protocol at twice the speed (IU8LMC, Martino, ARI
! Caserta; C++ reference in MSHV by LZ2HV).  Same LDPC(174,91), same 103
! channel symbols with four 4x4 Costas arrays, GFSK BT=1; 288 samples per
! symbol at 12000 S/s (41.67 baud), 3.75 s T/R period.  Ported from JTDX's
! FT4 chain (K1JT/K9AN et al., UA3DJY) by halving every timing constant.
module ft2_mod1

  real*4 ddf2(41472)
  logical(1) llagcc2,lfilter2,lhidetest2,lhidetelemetry2
  integer nFT2decd,nfafilt2,nfbfilt2

end module ft2_mod1
