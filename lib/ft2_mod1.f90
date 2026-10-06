! JTDX-VU: FT2 - FT4's protocol at twice the speed (IU8LMC, Martino, ARI
! Caserta; C++ reference in MSHV by LZ2HV).  Same LDPC(174,91), same 103
! channel symbols with four 4x4 Costas arrays, GFSK BT=1; 288 samples per
! symbol at 12000 S/s (41.67 baud), 3.75 s T/R period.  Ported from JTDX's
! FT4 chain (K1JT/K9AN et al., UA3DJY) by halving every timing constant.
module ft2_mod1

  real*4 ddf2(41472)
  logical(1) llagcc2,lfilter2,lhidetest2,lhidetelemetry2
  integer nFT2decd,nfafilt2,nfbfilt2

! JTDX-VU: AP7 (MSHV's a7 for FT2, from WSJT-X's FT8 a7): call pairs printed in
! recent periods, by even/odd 3.75 s slot, retried two periods later at the same
! frequency against ~158 likely messages.  Index 1 = current period (being
! filled), 2 = the one before (what a7d tries).
  integer, parameter :: A7MAX=20
  integer na7dec(2,0:1)                       ! entries per (table, parity)
  real a7dt(2,0:1,A7MAX),a7f(2,0:1,A7MAX)
  character*37 a7msg(2,0:1,A7MAX)             ! "CALL1 CALL2" or "CALL1 CALL2 GRID"
  integer na7utc                              ! this period's hhmmss (set by decoder.f90)
  integer na7lastutc                          ! last period that was decoded
  integer na7zerop                            ! consecutive periods with no decode
  logical la7dbg                              ! trace every AP7 attempt on stderr (ft2dec: A7DEBUG=1)
  real a7qual                                 ! AP7 quality gate 1-(nharderrors+dmin)/60 >= a7qual (ft2dec: A7QUAL=)
  data na7dec/4*0/, na7utc/-1/, na7lastutc/-1/, na7zerop/0/, la7dbg/.false./, a7qual/0.02/

end module ft2_mod1
