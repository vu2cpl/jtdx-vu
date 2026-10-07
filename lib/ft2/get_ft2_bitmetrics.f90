! JTDX-VU: FT2 - FT4's protocol at twice the speed (IU8LMC, Martino, ARI
! Caserta; C++ reference in MSHV by LZ2HV).  Same LDPC(174,91), same 103
! channel symbols with four 4x4 Costas arrays, GFSK BT=1; 288 samples per
! symbol at 12000 S/s (41.67 baud), 3.75 s T/R period.  Ported from JTDX's
! FT4 chain (K1JT/K9AN et al., UA3DJY) by halving every timing constant.
! This source code file was last time modified by Igor UA3DJY on 20191101
! All changes are shown in the patch file coming together with the full JTDX source code.

subroutine get_ft2_bitmetrics(cd,bitmetrics,badsync)

!   use ft4_mod1, only : llagcc
   include 'ft2_params.f90'
   parameter (NSS=NSPS/NDOWN,NDMAX=NMAX/NDOWN)
   complex cd(0:NN*NSS-1),cs(0:3,NN),csymb(NSS)
   integer icos4a(0:3),icos4b(0:3),icos4c(0:3),icos4d(0:3),graymap(0:3),ip(1)!,ka(1)
   logical one(0:255,0:7)    ! 256 4-symbol sequences, 8 bits
   logical first,badsync
   real bitmetrics(2*NN,3),s2(0:255),s4(0:3,NN)!,sp(0:3)
   real pwr(0:3,NN)          ! JTDX-VU: tone power, for the noise estimate and the LSE demapper
   real*8 noise_sum,noise_var,beta,beta_eff,maxp1,maxp0,lse1,lse0,ex
   integer nnoise
! JTDX-VU: MSHV's channel estimation / MMSE equalisation (LZ2HV, decoderft2.cpp)
   integer sync_pos(16),itone_sync(16)
   complex h_sync(16),h_est(0:NN-1),cd_eq(0:NN*NSS-1),csymb_eq(NSS)
   real h_mag(0:NN-1),ch_snr(0:NN-1),pwr_eq(0:3,NN),sp_eq(0:3),bmet_eq(2*NN)
   real*8 sum_noise,ncount,noise_var_ch,den,snr_min,snr_max,snr_mean,fading_depth,ld
   real*8 noise_sum_eq,noise_var_eq,beta_eq,snr_weight,blend
   integer nnoise_eq,j,idx,m
   real w
   logical use_cheq

   data icos4a/0,1,3,2/,icos4b/1,0,2,3/,icos4c/2,3,1,0/,icos4d/3,2,0,1/,graymap/0,1,3,2/
   data first/.true./
   save first,one

   ibmax=1

   if(first) then
      one=.false.
      do i=0,255
         do j=0,7
            if(iand(i,2**j).ne.0) one(i,j)=.true.
         enddo
      enddo
      first=.false.
   endif

   do k=1,NN
      i1=(k-1)*NSS
      csymb=cd(i1:i1+NSS-1)
!      if(.not.llagcc) then; csymb(1)=csymb(1)*1.9; csymb(NSS)=csymb(NSS)*1.9; endif
      csymb(1)=csymb(1)*1.9; csymb(NSS)=csymb(NSS)*1.9
      call four2a(csymb,NSS,1,-1,1)
      cs(0:3,k)=csymb(1:4)
      s4(0:3,k)=abs(csymb(1:4))
      pwr(0:3,k)=real(csymb(1:4))**2+aimag(csymb(1:4))**2
   enddo

! JTDX-VU: noise variance from the Costas sync symbols - at a sync position
! the signal is in one known tone, the other three tones carry only noise
! (as MSHV's FT2 demapper by LZ2HV).
   noise_sum=0.d0; nnoise=0
   do k=1,4
      do itone=0,3
         if(itone.ne.icos4a(k-1)) then; noise_sum=noise_sum+pwr(itone,k);    nnoise=nnoise+1; endif
         if(itone.ne.icos4b(k-1)) then; noise_sum=noise_sum+pwr(itone,k+33); nnoise=nnoise+1; endif
         if(itone.ne.icos4c(k-1)) then; noise_sum=noise_sum+pwr(itone,k+66); nnoise=nnoise+1; endif
         if(itone.ne.icos4d(k-1)) then; noise_sum=noise_sum+pwr(itone,k+99); nnoise=nnoise+1; endif
      enddo
   enddo
   noise_var=1.d0; if(nnoise.gt.0) noise_var=noise_sum/nnoise
   if(noise_var.lt.1.d-10) noise_var=1.d-10
   beta=0.5d0/noise_var; beta=max(0.01d0,min(50.d0,beta))

!   sp=0.
!   do k=0,3; sp(k)=sum(s4(k,1:4))+sum(s4(k,23:103)); enddo
!   ka=minloc(sp)-1; k=ka(1); if(k.lt.0) go to 2
!   do kb=0,3
!     if(kb.eq.k) cycle; spr=sp(kb)/sp(k)
!     if(spr.gt.1.4) then; s4(kb,:)=s4(kb,:)/spr; sprsqr=SQRT(spr); cs(kb,:)=cs(kb,:)/sprsqr; endif
!   enddo
!2  continue

! Sync quality check
   is1=0
   is2=0
   is3=0
   is4=0
   badsync=.false.
   do k=1,4
      ip=maxloc(s4(:,k))
      if(icos4a(k-1).eq.(ip(1)-1)) is1=is1+1
      ip=maxloc(s4(:,k+33))
      if(icos4b(k-1).eq.(ip(1)-1)) is2=is2+1
      ip=maxloc(s4(:,k+66))
      if(icos4c(k-1).eq.(ip(1)-1)) is3=is3+1
      ip=maxloc(s4(:,k+99))
      if(icos4d(k-1).eq.(ip(1)-1)) is4=is4+1
   enddo
   nsync=is1+is2+is3+is4   !Number of correct hard sync symbols, 0-16
   if(nsync .lt. 8) then
      badsync=.true.
      return
   endif

   do nseq=1,3             !Try coherent sequences of 1, 2, and 4 symbols
      if(nseq.eq.1) nsym=1
      if(nseq.eq.2) nsym=2
      if(nseq.eq.3) nsym=4
      nt=2**(2*nsym)
      do ks=1,NN-nsym+1,nsym  !87+16=103 symbols.
         amax=-1.0
         do i=0,nt-1
            i1=i/64
            i2=iand(i,63)/16
            i3=iand(i,15)/4
            i4=iand(i,3)
! JTDX-VU: power |s|^2 (was magnitude) for the log-sum-exp demapper below
            if(nsym.eq.1) then
               s2(i)=abs(cs(graymap(i4),ks))**2
            elseif(nsym.eq.2) then
               s2(i)=abs(cs(graymap(i3),ks)+cs(graymap(i4),ks+1))**2
            elseif(nsym.eq.4) then
               s2(i)=abs(cs(graymap(i1),ks  ) + &
                  cs(graymap(i2),ks+1) + &
                  cs(graymap(i3),ks+2) + &
                  cs(graymap(i4),ks+3)   &
                  )**2
            else
               print*,"Error - nsym must be 1, 2, or 4."
            endif
         enddo
         ipt=1+(ks-1)*2
         if(nsym.eq.1) ibmax=1
         if(nsym.eq.2) ibmax=3
         if(nsym.eq.4) ibmax=7
! JTDX-VU: exact log-sum-exp LLR (MSHV's FT2 demapper): LLR = log sum exp(beta*p)
! over bit=1 symbols minus the same over bit=0, instead of max-log (max minus
! max).  beta scales with the noise variance; coherent sums of nsym symbols
! carry nsym times the noise, so beta/nsym.  Max subtracted for stability.
         beta_eff=beta/nsym
         do ib=0,ibmax
            if(ipt+ib.gt.2*NN) cycle
            maxp1=-1.d30; maxp0=-1.d30
            do i=0,nt-1
               if(one(i,ibmax-ib)) then; if(beta_eff*s2(i).gt.maxp1) maxp1=beta_eff*s2(i)
               else; if(beta_eff*s2(i).gt.maxp0) maxp0=beta_eff*s2(i); endif
            enddo
            lse1=0.d0; lse0=0.d0
            do i=0,nt-1
               if(one(i,ibmax-ib)) then; ex=min(20.d0,beta_eff*s2(i)-maxp1); lse1=lse1+exp(ex)
               else; ex=min(20.d0,beta_eff*s2(i)-maxp0); lse0=lse0+exp(ex); endif
            enddo
            lse1=maxp1+log(max(lse1,1.d-30)); lse0=maxp0+log(max(lse0,1.d-30))
            bitmetrics(ipt+ib,nseq)=real(lse1-lse0)
         enddo
      enddo
   enddo

! ---- JTDX-VU: adaptive channel estimation + MMSE equalisation (MSHV FT2) ----
! Step 1: H(k) at the 16 Costas symbols = the received known tone; noise from
! the other three tones.
   do j=1,4
      sync_pos(j)=j-1;      itone_sync(j)=icos4a(j-1)
      sync_pos(j+4)=j+32;   itone_sync(j+4)=icos4b(j-1)
      sync_pos(j+8)=j+65;   itone_sync(j+8)=icos4c(j-1)
      sync_pos(j+12)=j+98;  itone_sync(j+12)=icos4d(j-1)
   enddo
   sum_noise=0.d0; ncount=0.d0
   do j=1,16
      k=sync_pos(j)+1                      ! cs(:,k) is 1-based by symbol
      h_sync(j)=cs(itone_sync(j),k)
      do m=0,3
         if(m.ne.itone_sync(j)) then; sum_noise=sum_noise+pwr(m,k); ncount=ncount+1.d0; endif
      enddo
   enddo
   noise_var_ch=1.d-10; if(ncount.gt.0.d0) noise_var_ch=sum_noise/ncount
! Step 2: interpolate H across all symbols between the Costas group centres
   h_est=0.
   h_est(0)=h_sync(1)
   h_est(1)=(h_sync(1)+h_sync(2))/2.; h_est(2)=(h_sync(2)+h_sync(3))/2.; h_est(3)=(h_sync(3)+h_sync(4))/2.
   do k=4,32
      w=real(k-2)/real(34-2); w=max(0.,min(1.,w))
      h_est(k)=(1.-w)*(h_sync(3)+h_sync(4))/2.+w*(h_sync(5)+h_sync(6))/2.
   enddo
   h_est(33)=(h_sync(5)+h_sync(6))/2.; h_est(34)=(h_sync(6)+h_sync(7))/2.; h_est(35)=(h_sync(7)+h_sync(8))/2.; h_est(36)=h_sync(8)
   do k=37,65
      w=real(k-35)/real(67-35); w=max(0.,min(1.,w))
      h_est(k)=(1.-w)*(h_sync(7)+h_sync(8))/2.+w*(h_sync(9)+h_sync(10))/2.
   enddo
   h_est(66)=(h_sync(9)+h_sync(10))/2.
   h_est(67)=(h_sync(10)+h_sync(11))/2.
   h_est(68)=(h_sync(11)+h_sync(12))/2.
   h_est(69)=h_sync(12)
   do k=70,98
      w=real(k-68)/real(100-68); w=max(0.,min(1.,w))
      h_est(k)=(1.-w)*(h_sync(11)+h_sync(12))/2.+w*(h_sync(13)+h_sync(14))/2.
   enddo
   h_est(99)=(h_sync(13)+h_sync(14))/2.
   h_est(100)=(h_sync(14)+h_sync(15))/2.
   h_est(101)=(h_sync(15)+h_sync(16))/2.
   h_est(102)=h_sync(16)
! Step 3: MMSE equalise y_eq = conj(H) y / (|H|^2 + Nvar), and per-symbol SNR
   snr_min=1.d30; snr_max=-1.d30; snr_mean=0.d0
   do k=0,NN-1
      h_mag(k)=real(h_est(k))**2+aimag(h_est(k))**2
      idx=k*NSS
      den=h_mag(k)+noise_var_ch
      if(den.gt.1.d-20) then; cd_eq(idx:idx+NSS-1)=cd(idx:idx+NSS-1)*conjg(h_est(k))/real(den)
      else; cd_eq(idx:idx+NSS-1)=cd(idx:idx+NSS-1); endif
      if(noise_var_ch.gt.1.d-20) then; ch_snr(k)=h_mag(k)/noise_var_ch; else; ch_snr(k)=100.; endif
      snr_min=min(snr_min,dble(ch_snr(k))); snr_max=max(snr_max,dble(ch_snr(k))); snr_mean=snr_mean+ch_snr(k)
   enddo
   snr_mean=snr_mean/NN
! fading depth in dB; only equalise when it exceeds 6 dB (AWGN gains nothing)
   if(snr_min.gt.1.d-10) then; ld=max(1.d-6,snr_max/snr_min); fading_depth=10.d0*log10(ld)
   else; fading_depth=30.d0; endif
   use_cheq=fading_depth.gt.6.d0
   if(use_cheq) then
      do k=1,NN
         csymb_eq=cd_eq((k-1)*NSS:k*NSS-1)
         call four2a(csymb_eq,NSS,1,-1,1)
         pwr_eq(0:3,k)=real(csymb_eq(1:4))**2+aimag(csymb_eq(1:4))**2
      enddo
      noise_sum_eq=0.d0; nnoise_eq=0
      do k=1,4
         do itone=0,3
            if(itone.ne.icos4a(k-1)) then; noise_sum_eq=noise_sum_eq+pwr_eq(itone,k);    nnoise_eq=nnoise_eq+1; endif
            if(itone.ne.icos4b(k-1)) then; noise_sum_eq=noise_sum_eq+pwr_eq(itone,k+33); nnoise_eq=nnoise_eq+1; endif
            if(itone.ne.icos4c(k-1)) then; noise_sum_eq=noise_sum_eq+pwr_eq(itone,k+66); nnoise_eq=nnoise_eq+1; endif
            if(itone.ne.icos4d(k-1)) then; noise_sum_eq=noise_sum_eq+pwr_eq(itone,k+99); nnoise_eq=nnoise_eq+1; endif
         enddo
      enddo
      noise_var_eq=noise_sum_eq/max(1,nnoise_eq); if(noise_var_eq.lt.1.d-10) noise_var_eq=1.d-10
      beta_eq=0.5d0/noise_var_eq; beta_eq=max(0.01d0,min(50.d0,beta_eq))
! single-symbol LSE metrics on the equalised signal, weighted by per-symbol SNR
      do ks=1,NN
         do i=0,3; sp_eq(i)=pwr_eq(graymap(i),ks); enddo
         ipt=1+(ks-1)*2
         snr_weight=1.d0
         if(snr_mean.gt.1.d-10) snr_weight=max(0.1d0,min(3.d0,sqrt(ch_snr(ks-1)/snr_mean)))
         do ib=0,1
            if(ipt+ib.gt.2*NN) cycle
            maxp1=-1.d30; maxp0=-1.d30
            do i=0,3
               if(one(i,1-ib)) then; maxp1=max(maxp1,beta_eq*sp_eq(i)); else; maxp0=max(maxp0,beta_eq*sp_eq(i)); endif
            enddo
            lse1=0.d0; lse0=0.d0
            do i=0,3
               if(one(i,1-ib)) then; lse1=lse1+exp(min(20.d0,beta_eq*sp_eq(i)-maxp1))
               else; lse0=lse0+exp(min(20.d0,beta_eq*sp_eq(i)-maxp0)); endif
            enddo
            lse1=maxp1+log(max(lse1,1.d-30)); lse0=maxp0+log(max(lse0,1.d-30))
            bmet_eq(ipt+ib)=real((lse1-lse0)*snr_weight)
         enddo
      enddo
      call normalizebmet(bmet_eq,2*NN)
! blend into set A: 0 at 6 dB fading, 1 at 16 dB, capped at 0.9
      blend=max(0.d0,min(0.9d0,(fading_depth-6.d0)/10.d0))
      call normalizebmet(bitmetrics(:,1),2*NN)
      bitmetrics(:,1)=real((1.d0-blend))*bitmetrics(:,1)+real(blend)*bmet_eq
   endif
! ---- end equalisation ----

   bitmetrics(205:206,2)=bitmetrics(205:206,1)
   bitmetrics(201:204,3)=bitmetrics(201:204,2)
   bitmetrics(205:206,3)=bitmetrics(205:206,1)

   call normalizebmet(bitmetrics(:,1),2*NN)
   call normalizebmet(bitmetrics(:,2),2*NN)
   call normalizebmet(bitmetrics(:,3),2*NN)
   return

end subroutine get_ft2_bitmetrics
