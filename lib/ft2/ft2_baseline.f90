! JTDX-VU: FT2 - FT4's protocol at twice the speed (IU8LMC, Martino, ARI
! Caserta; C++ reference in MSHV by LZ2HV).  Same LDPC(174,91), same 103
! channel symbols with four 4x4 Costas arrays, GFSK BT=1; 288 samples per
! symbol at 12000 S/s (41.67 baud), 3.75 s T/R period.  Ported from JTDX's
! FT4 chain (K1JT/K9AN et al., UA3DJY) by halving every timing constant.
! last time modified by Igor UA3DJY on 20191121

subroutine ft2_baseline(s,nfa,nfb,sbase)

! Fit baseline to spectrum 
! Input:  s(npts)         Linear scale in power
! Output: sbase(npts)    Baseline

  include 'ft2_params.f90'
  implicit real*8 (a-h,o-z)
  real*4 s(NH1)
  real*4 sbase(NH1)
  real*4 base
  real*8 x(1000),y(1000),a(5)
  data nseg/10/,npct/10/

  df=12000.0/NFFT1                    !8.52 Hz
  ia=max(nint(200.0/df),nfa)
  ib=min(NH1,nfb)
  do i=ia,ib
     s(i)=10.0*log10(s(i))            !Convert to dB scale
  enddo

  nterms=5
  nlen=(ib-ia+1)/nseg                 !Length of test segment
  i0=(ib-ia+1)/2                      !Midpoint
  k=0
  do n=1,nseg                         !Loop over all segments
     ja=ia + (n-1)*nlen
     jb=ja+nlen-1
     call pctile(s(ja),nlen,npct,base) !Find lowest npct of points
     do i=ja,jb
        if(s(i).le.base) then
           if (k.lt.1000) k=k+1       !Save all "lower envelope" points
           x(k)=i-i0
           y(k)=s(i)
        endif
     enddo
  enddo
  kz=k
  a=0.
  call polyfit(x,y,y,kz,nterms,0,a,chisqr)  !Fit a low-order polynomial
  do i=ia,ib
     t=i-i0
     sbase(i)=a(1)+t*(a(2)+t*(a(3)+t*(a(4)+t*(a(5))))) + 0.65
!     write(51,3051) i*df,s(i),sbase(i)
!3051 format(3f12.3)
     sbase(i)=10**(sbase(i)/10.0)
  enddo
  return
end subroutine ft2_baseline
