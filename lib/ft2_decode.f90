! JTDX-VU: FT2 - FT4's protocol at twice the speed (IU8LMC, Martino, ARI
! Caserta; C++ reference in MSHV by LZ2HV).  Same LDPC(174,91), same 103
! channel symbols with four 4x4 Costas arrays, GFSK BT=1; 288 samples per
! symbol at 12000 S/s (41.67 baud), 3.75 s T/R period.  Ported from JTDX's
! FT4 chain (K1JT/K9AN et al., UA3DJY) by halving every timing constant.
module ft2_decode

  type :: ft2_decoder
    procedure(ft2_decode_callback), pointer :: callback
  contains
    procedure :: decode
  end type ft2_decoder

  abstract interface
    subroutine ft2_decode_callback (this,snr,dt,freq,decoded,servis4)
      import ft2_decoder
      implicit none
      class(ft2_decoder), intent(inout) :: this
      integer, intent(in) :: snr
      real, intent(in) :: dt
      real, intent(in) :: freq
      character(len=26), intent(in) :: decoded
      character(len=1), intent(in) :: servis4
    end subroutine ft2_decode_callback
  end interface

contains

  subroutine decode(this,callback,nQSOProgress,nfqso,nfa,nfb,ndepth,stophint,swl)
!    use timer_module, only: timer
    use packjt77
    use ft2_mod1, only : nFT2decd,nfafilt2,nfbfilt2,lfilter2,lhidetest2,lhidetelemetry2, &
         A7MAX,na7dec,a7dt,a7f,a7msg,na7utc,na7lastutc,na7zerop,a7qual
    use ft8_mod1, only : sumxdtt,avexdt,mycall,hiscall
    include 'ft2/ft2_params.f90'
    class(ft2_decoder), intent(inout) :: this
    procedure(ft2_decode_callback) :: callback
    parameter (NSS=NSPS/NDOWN,NDMAX=NMAX/NDOWN)
    character message*37,msg26*26,msgsent*37,msg37_2*37
    character c77*77
    character*37 decodes(100)
    character*12 mycall0,hiscall0,call_a,call_b
    character*4 servis4
    character*37 a7m
    integer ja7,ia7,na7h,n7,ia7f
    real xdt7,f7,xsnr7
    character*13 a7w(19),c1_7,c2_7
    integer a7nw(19)
    character*4 grid7
    complex cd2(0:NDMAX-1)                  !Complex waveform
    complex cb(0:NDMAX-1)
    complex cd(0:NN*NSS-1)                       !Complex waveform
    complex ctwk(2*NSS),ctwk2(2*NSS,-16:16)
    real a(5)
    real bitmetrics(2*NN,3)
    real llr(2*ND),llra(2*ND),llrb(2*ND),llrc(2*ND),llrd(2*ND)
    real llrmx(2*ND),llrav(2*ND)   ! JTDX-VU: MSHV's sets D (max |LLR| of A/B/C) and E (mean)
    real candidate(2,100)
    integer apbits(2*ND)
    integer*1 message77(77),rvec(77),apmask(2*ND),cw(2*ND)
    integer*1 hbits(2*NN)
    integer i4tone(103)
    integer nappasses(0:5)    ! # of decoding passes for QSO States 0-5
    integer naptypes(0:5,4)   ! nQSOProgress, decoding pass
    integer mcq(29),mrrr(19),m73(19),mrr73(19)
    logical nohiscall,unpk77_success,first,dobigfft,dosubtract,doosd,badsync,lFreeText,lhidemsg
    logical(1), intent(in) :: stophint,swl
    logical(1) falsedec

    data first/.true./
    data     mcq/0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0/
    data    mrrr/0,1,1,1,1,1,1,0,1,0,0,1,0,0,1,0,0,0,1/
    data     m73/0,1,1,1,1,1,1,0,1,0,0,1,0,1,0,0,0,0,1/
    data   mrr73/0,1,1,1,1,1,1,0,0,1,1,1,0,1,0,1,0,0,1/
    data rvec/0,1,0,0,1,0,1,0,0,1,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,0,1,1,0, &
      1,0,0,1,0,1,1,0,0,0,0,1,0,0,0,1,0,1,0,0,1,1,1,1,0,0,1,0,1, &
      0,1,0,1,0,1,1,0,1,1,1,1,1,0,0,0,1,0,1/
    save fs,dt1,tt,txt,twopi,h,first,apbits,nappasses,naptypes, &
      mycall0,hiscall0,ctwk2

    this%callback => callback
    mycalllen1=len_trim(mycall)+1
    smax=0.; smax1=0.; nd1=0 ! smax init value shall be increased to 1.+ ?

    if(first) then
      fs=12000.0/NDOWN                !Sample rate after downsampling
      dt1=1/fs                         !Sample interval after downsample (s)
      tt=NSPS*dt1                      !Duration of "itone" symbols (s)
      txt=NZ*dt1                       !Transmission length (s) without ramp up/down
      twopi=8.0*atan(1.0)
      h=1.0

      do idf=-16,16
        a=0.
        a(1)=real(idf)
        ctwk=1.
        call twkfreq1(ctwk,0,2*NSS,2*NSS,fs/2.0,a,ctwk2(:,idf))
      enddo

      mcq=2*mod(mcq+rvec(1:29),2)-1
      mrrr=2*mod(mrrr+rvec(59:77),2)-1
      m73=2*mod(m73+rvec(59:77),2)-1
      mrr73=2*mod(mrr73+rvec(59:77),2)-1
      nappasses(0)=2; nappasses(1)=2; nappasses(2)=2; nappasses(3)=2; nappasses(4)=2; nappasses(5)=3

! iaptype
!   1        CQ     ???    ???           (29 ap bits)
!   2        MyCall ???    ???           (29 ap bits)
!   3        MyCall DxCall ???           (58 ap bits)
!   4        MyCall DxCall RRR           (77 ap bits)
!   5        MyCall DxCall 73            (77 ap bits)
!   6        MyCall DxCall RR73          (77 ap bits)

      naptypes(0,1:4)=(/1,2,0,0/) ! Tx6 selected (CQ)
      naptypes(1,1:4)=(/2,3,0,0/) ! Tx1
      naptypes(2,1:4)=(/2,3,0,0/) ! Tx2
      naptypes(3,1:4)=(/3,6,0,0/) ! Tx3
      naptypes(4,1:4)=(/3,6,0,0/) ! Tx4
      naptypes(5,1:4)=(/3,1,2,0/) ! Tx5

      mycall0=''; hiscall0=''; first=.false.
    endif

    l1=index(mycall,char(0)); if(l1.ne.0) mycall(l1:)=" "
    l1=index(hiscall,char(0)); if(l1.ne.0) hiscall(l1:)=" "
    if(mycall.ne.mycall0 .or. hiscall.ne.hiscall0) then
      apbits=0; apbits(1)=99; apbits(30)=99
      if(len(trim(mycall)) .lt. 3) go to 10
      nohiscall=.false.; hiscall0=hiscall
! use mycall for dummy hiscall - mycall won't be hashed
      if(len(trim(hiscall0)).lt.3) then; hiscall0=mycall; nohiscall=.true.; endif
      message=trim(mycall)//' '//trim(hiscall0)//' RR73'
      i3=-1; n3=-1
      call pack77(message,i3,n3,c77,1); call unpack77(c77,1,msgsent,unpk77_success,1)
      if(i3.ne.1 .or. (message.ne.msgsent) .or. .not.unpk77_success) go to 10
      read(c77,'(77i1)') message77
      message77=mod(message77+rvec,2)
      call encode174_91(message77,cw)
      apbits=2*cw-1
      if(nohiscall) apbits(30)=99
10    continue
      mycall0=mycall; hiscall0=hiscall
    endif

    maxcand=100; ndecodes=0; decodes=' '; fa=nfa; fb=nfb
! ndepth=1: 1 pass, no subtraction
! ndepth=2: 3 passes, bp only
! ndepth=3: 3 passes, bp+osd
    max_iterations=40; syncmin=1.2; dosubtract=.true.; doosd=.true.; nsp=3
    if(ndepth.eq.2) doosd=.false.
    if(ndepth.eq.1) then; nsp=1; dosubtract=.false.; doosd=.false.; endif

    do isp = 1,nsp
      if(isp.eq.2) then; if(ndecodes.eq.0) exit; nd1=ndecodes
      elseif(isp.eq.3) then; nd2=ndecodes-nd1; if(nd2.eq.0) exit
      endif

      candidate=0.0
      ncand=0
      call getcandidates2(fa,fb,syncmin,nfqso,maxcand,candidate,ncand)
      dobigfft=.true.
      do icand=1,ncand
        f0=candidate(1,icand)
        nf0=nint(f0); if(lfilter2 .and. (nf0.lt.nfafilt2 .or. nf0.gt.nfbfilt2)) cycle
        snr=candidate(2,icand)-1.0
!print *,icand,f0,snr
        call ft2_downsample(dobigfft,f0,cd2)  !Downsample to 32 Sam/Sym
        if(dobigfft) dobigfft=.false.
        sum2=sum(cd2*conjg(cd2))/(real(NMAX)/real(NDOWN))
        if(sum2.gt.0.0) cd2=cd2/sqrt(sum2)
! Sample rate is now 12000/9 = 1333.33 samples/second; 0.5 sec = 667 samples
! three segments of ibwindow cover DT -0.52..+1.52 s (MSHV's FT2 window)
        if(swl) then; ibwindow=1000; else; ibwindow=904; endif
        ibottom=(0.5+avexdt)*1333.33-1333
        do iseg=1,3                ! DT search is done over 3 segments
          do isync=1,2          
            if(isync.eq.1) then
              idfmin=-12
              idfmax=12
              idfstp=3
!-1.0..+1.4; -1.2..+1.7 start window
              if(abs(avexdt).lt.1.e-6) then
                if(iseg.eq.1) then
                  if(swl) then; ibmin=200; ibmax=1200; else; ibmin=216; ibmax=1120; endif
                elseif(iseg.eq.2) then
                  smax1=smax
                  if(swl) then; ibmin=1201; ibmax=2200; else; ibmin=1121; ibmax=2024; endif
                elseif(iseg.eq.3) then
                  if(swl) then; ibmin=-800; ibmax=199; else; ibmin=-688; ibmax=215; endif
                endif
              else
                if(iseg.eq.1) then
                  ibmin=ibottom+ibwindow+1; ibmax=ibottom+ibwindow*2
                elseif(iseg.eq.2) then
                  smax1=smax
                  ibmin=ibottom+ibwindow*2+1; ibmax=ibottom+ibwindow*3
                elseif(iseg.eq.3) then
                  ibmin=ibottom; ibmax=ibottom+ibwindow
                endif
              endif
              ibstp=4
            else
              idfmin=idfbest-4
              idfmax=idfbest+4
              idfstp=1
              ibmin=ibest-5
              ibmax=ibest+5
              ibstp=1
            endif
            ibest=-1
            idfbest=0
            smax=-99.
            do idf=idfmin,idfmax,idfstp
              do istart=ibmin,ibmax,ibstp
                call sync2d(cd2,istart,ctwk2(:,idf),1,sync)  !Find sync power
                if(sync.gt.smax) then
                  smax=sync
                  ibest=istart
                  idfbest=idf
                endif
              enddo
            enddo
          enddo
          if(iseg.eq.1) smax1=smax
          if(smax.lt.1.2) cycle
          if(iseg.gt.1 .and. smax.lt.smax1) cycle 
          f1=f0+real(idfbest)
          if( f1.le.10.0 .or. f1.ge.4990.0 ) cycle
          call ft2_downsample(dobigfft,f1,cb) !Final downsample, corrected f0
          sum2=sum(abs(cb)**2)/(real(NSS)*NN)
          if(sum2.gt.0.0) cb=cb/sqrt(sum2)
          cd=0.
          if(ibest.ge.0) then
            it=min(NDMAX-1,ibest+NN*NSS-1)
            np=it-ibest+1
            cd(0:np-1)=cb(ibest:it)
          else
            cd(-ibest:ibest+NN*NSS-1)=cb(0:NN*NSS+2*ibest-1)
          endif
          call get_ft2_bitmetrics(cd,bitmetrics,badsync)
          if(badsync) cycle
          hbits=0
          where(bitmetrics(:,1).ge.0) hbits=1
          ns1=count(hbits(  1:  8).eq.(/0,0,0,1,1,0,1,1/))
          ns2=count(hbits( 67: 74).eq.(/0,1,0,0,1,1,1,0/))
          ns3=count(hbits(133:140).eq.(/1,1,1,0,0,1,0,0/))
          ns4=count(hbits(199:206).eq.(/1,0,1,1,0,0,0,1/))
          nsync_qual=ns1+ns2+ns3+ns4
          if(nsync_qual.lt. 20) cycle

          scalefac=2.83
          llra(  1: 58)=bitmetrics(  9: 66, 1)
          llra( 59:116)=bitmetrics( 75:132, 1)
          llra(117:174)=bitmetrics(141:198, 1)
          llra=scalefac*llra
          llrb(  1: 58)=bitmetrics(  9: 66, 2)
          llrb( 59:116)=bitmetrics( 75:132, 2)
          llrb(117:174)=bitmetrics(141:198, 2)
          llrb=scalefac*llrb
          llrc(  1: 58)=bitmetrics(  9: 66, 3)
          llrc( 59:116)=bitmetrics( 75:132, 3)
          llrc(117:174)=bitmetrics(141:198, 3)
          llrc=scalefac*llrc

          apmag=maxval(abs(llra))*1.1
! JTDX-VU: two more non-AP passes as MSHV's FT2 - D takes, per bit, whichever
! of the 1/2/4-symbol metrics is most confident; E averages the three
          do i=1,2*ND
            if(abs(llra(i)).ge.abs(llrb(i)) .and. abs(llra(i)).ge.abs(llrc(i))) then; llrmx(i)=llra(i)
            elseif(abs(llrb(i)).ge.abs(llrc(i))) then; llrmx(i)=llrb(i)
            else; llrmx(i)=llrc(i); endif
            llrav(i)=(llra(i)+llrb(i)+llrc(i))/3.0
          enddo
          npasses=5+nappasses(nQSOProgress)
          if(stophint) npasses=6
          if(ndepth.eq.1) npasses=5
          do ipass=1,npasses
            if(ipass.eq.1) llr=llra; if(ipass.eq.2) llr=llrb; if(ipass.eq.3) llr=llrc
            if(ipass.eq.4) llr=llrmx; if(ipass.eq.5) llr=llrav
            if(ipass.le.5) then; apmask=0; iaptype=0; endif
            if(ipass.gt.5) then
              llrd=llra
              iaptype=naptypes(nQSOProgress,ipass-5)
              if(stophint) iaptype=1
! Conditions that cause us to bail out of AP decoding
              napwid=50
              if(iaptype.ge.3 .and. (abs(f1-nfqso).gt.napwid)) cycle
              if(iaptype.ge.2 .and. apbits(1).gt.1) cycle  ! No, or nonstandard, mycall
              if(iaptype.ge.3 .and. apbits(30).gt.1) cycle ! No, or nonstandard, dxcall

              if(iaptype.eq.1) then; apmask=0; apmask(1:29)=1; llrd(1:29)=apmag*mcq(1:29); endif ! CQ
              if(iaptype.eq.2) then; apmask=0; apmask(1:29)=1; llrd(1:29)=apmag*apbits(1:29); endif ! MyCall,???,???
              if(iaptype.eq.3) then; apmask=0; apmask(1:58)=1; llrd(1:58)=apmag*apbits(1:58); endif ! MyCall,DxCall,???
              if(iaptype.eq.4 .or. iaptype.eq.5 .or. iaptype.eq.6) then ! mycall, hiscall, RRR|73|RR73
                apmask=0; apmask(1:77)=1; if(iaptype.eq.6) llrd(1:77)=apmag*apbits(1:77)
              endif
              llr=llrd
            endif
            message77=0; dmin=0.0
            call bpdecode174_91(llr,apmask,max_iterations,message77,cw,nharderror,niterations)
            if(doosd .and. nharderror.lt.0) then
              ndeep=3
!              if(abs(nfqso-f1).le.napwid) ndeep=4
              call osd4_174_91(llr,apmask,ndeep,message77,cw,nharderror,dmin)
            endif

            if(sum(message77).eq.0) cycle
            if(nharderror.ge.0) then
              message77=mod(message77+rvec,2) ! remove rvec scrambling
              write(c77,'(77i1)') message77(1:77); read(c77(72:74),'(b3)') n3; read(c77(75:77),'(b3)') i3
              call unpack77(c77,1,message,unpk77_success,1)
              if(message.eq."") cycle ! being treated as false decode
              if(unpk77_success.and.dosubtract) then
                call get_ft4_tones_from_77bits(message77,i4tone)
                dt=real(ibest)/1333.33
                call subtractft2(i4tone,f1,dt)
              endif

              lhidemsg=.false.
              if(lhidetelemetry2 .and. i3.eq.0 .and. n3.eq.5) lhidemsg=.true.
              if(lhidetest2) then
                if((i3.eq.0 .and. n3.gt.1 .and. n3.lt.5) .or. i3.eq.3 .or. i3.gt.4) then
                  if(mycalllen1.lt.4 .or. message(1:mycalllen1).ne.trim(mycall)//' ') lhidemsg=.true.
                endif
                if(message(1:3).eq.'CQ ') then
                  if(message(1:6).eq.'CQ RU ' .or. message(1:6).eq.'CQ FD ' .or. message(1:8).eq.'CQ TEST ') &
                    lhidemsg=.true.
                endif
              endif

              lFreeText=.false.; if(i3.eq.0 .and. n3.eq.0) lFreeText=.true.
! delete braces
              if(.not.lFreeText .and. index(message,'<').gt.0) then ! DXpedition being not supported in FT2
                ispc1=index(message,' '); ispc2=index(message((ispc1+1):),' ')+ispc1
                ispc3=index(message((ispc2+1):),' ')+ispc2
                ieoc1=ispc1-1; iboc2=ispc1+1; ieoc2=ispc2-1
                if(message(1:1).eq.'<' .and. message(2:2).ne.'.') then
                  message(ieoc1:37)=message(ieoc1+1:37)//' '; message(1:37)=message(2:37)//' '
                else if(message(iboc2:iboc2).eq.'<' .and. message(iboc2+1:iboc2+1).ne.'.') then
                  message(ieoc2:37)=message(ieoc2+1:37)//' '; message(iboc2:37)=message(iboc2+1:37)//' '
                else
                  iboc3=ispc2+1; ieoc3=ispc3-1
                  if(message(iboc3:iboc3).eq.'<' .and. message(iboc3+1:iboc3+1).ne.'.') then
                    message(ieoc3:37)=message(ieoc3+1:37)//' '; message(iboc3:37)=message(iboc3+1:37)//' '
                  endif
                endif
              endif

              idupe=0
              do i=1,ndecodes; if(decodes(i).eq.message) idupe=1; enddo
              if(idupe.eq.1) exit
              ndecodes=ndecodes+1; decodes(ndecodes)=message
              ! FT2: calibrated against ft2sim (the FT4 constant 14.8 read 3.5 dB low with
! half the symbol energy in the candidate spectrum)
              if(snr.gt.0.0) then; xsnr=10*log10(snr)-11.3; else; xsnr=-21.0; endif
              nsnr=nint(max(-21.0,xsnr))
              xdt=ibest/1333.33 - 0.5
! check for false decodes
! i3=3 n3=4  TU; B69FWJ 8Z6IB 559 580  
! i3=3 n3=3  TU; FD9GRU HT1HHY R 529 11
              if(message(1:3).eq.'TU;' .and. nsnr.lt.-15 .and. i3.eq.3 .and. (n3.eq.3 .or. n3.eq.4)) then
                ispc1=index(message,' '); ispc2=index(message((ispc1+1):),' ')+ispc1 
                ispc3=index(message((ispc2+1):),' ')+ispc2
                call_a=''; call_b=''; call_a=message(ispc1+1:ispc2-1); call_b=message(ispc2+1:ispc3-1)
                falsedec=.false.; call chkflscall(call_a,call_b,falsedec)
                if(falsedec) then; message=''; cycle; endif
              endif
              if(iaptype.eq.1 .and. xsnr.lt.-15.) then
                nbadcrc=0; call chkfalse8(message,i3,n3,nbadcrc,iaptype,.false.)
                if(nbadcrc.eq.1) then; message=''; cycle; endif
              endif
! EA1AHY M83WN/R R QA79   *
! MS8QQS UX3QBS/P R NG63  i3=2 n3=7
! 3B4NDC/R C40AUZ/R R IR83  i3=1 n3=7
! EA1AHY PW1BSL R GR47 i3=1 n3=3 mycall
! EA1AHY PW1BSL R GR47 *  i3=1 n3=1 mycall
              if((i3.eq.1 .or. i3.eq.2) .and. index(message,' R ').gt.0) then
                ispc1=index(message,' '); ispc2=index(message((ispc1+1):),' ')+ispc1
                ispc3=index(message((ispc2+1):),' ')+ispc2
                if(message(ispc2:ispc3).eq.' R ') then 
                  call_a='            '; call_b='            '
                  if(message(1:ispc1-1).eq.trim(mycall)) then
                    call_a='CQ          '
                  else
                    if((i3.eq.1 .and. message(ispc1-2:ispc1-1).eq.'/R') .or. &
                       (i3.eq.2 .and. message(ispc1-2:ispc1-1).eq.'/R')) then
                      call_a=message(1:ispc1-3)
                    else
                      call_a=message(1:ispc1-1)
                    endif
                  endif
                  if((i3.eq.1 .and. message(ispc2-2:ispc2-1).eq.'/R') .or. &
                     (i3.eq.2 .and. message(ispc2-2:ispc2-1).eq.'/P')) then
                    call_b=message(ispc1+1:ispc2-3)
                  else
                    call_b=message(ispc1+1:ispc2-1)
                  endif
                  falsedec=.false.; call chkflscall(call_a,call_b,falsedec)
                  if(falsedec) then; nbadcrc=1; message=''; return; endif
                endif
              endif
!write(21,'(i6.6,i5,2x,f4.1,i6,2x,a37,2x,f4.1,3i3,f5.1,i4,i4,i4)') &
!  nutc,nsnr,xdt,nint(f1),message,smax,iaptype,ipass,isp,dmin,nsync_qual,nharderror,iseg
              if(i3.eq.0 .and. n3.eq.1) then ! special DXpedition msg
                call msgparser(message,msg37_2)
                servis4="1"
                msg26=message(1:26); call this%callback(nsnr,xdt,f1,msg26,servis4)
                msg26=msg37_2(1:26); call this%callback(nsnr,xdt,f1,msg26,servis4)
              else
                msg26=message(1:26); servis4=""
                if(lFreeText) then; if(abs(nfqso-nint(f1)).le.10) then; servis4=','; else; servis4='.'; endif; endif
                if(.not.lhidemsg) call this%callback(nsnr,xdt,f1,msg26,servis4)
              endif
              nFT2decd=nFT2decd+1; sumxdtt(1)=sumxdtt(1)+xdt
              call a7_save(xdt,f1,message)   ! JTDX-VU: AP7 history
              exit
            endif
          enddo !Sequence estimation
          if(nharderror.ge.0) exit
        enddo !3 DT segments
      enddo    !Candidate list
    enddo       !Subtraction loop

! JTDX-VU: AP7 - for every call pair printed two periods ago in this even/odd
! slot, re-sync at its frequency and test the likely follow-on messages.
    ja7=a7_parity(na7utc)
    if(ndepth.ge.2) then
      do ia7=1,na7dec(2,ja7)
        if(a7f(2,ja7,ia7).le.-98.0) cycle            ! flagged "already decoded"
        a7m=a7msg(2,ja7,ia7)
        call split77(a7m,na7h,a7nw,a7w)
        if(na7h.lt.2) cycle
        grid7='    '; if(na7h.ge.3) grid7=a7w(3)(1:4)
        if(grid7.eq.'RR73' .or. index(grid7,'+').gt.0 .or. index(grid7,'-').gt.0) grid7='    '
        xdt7=a7dt(2,ja7,ia7); f7=a7f(2,ja7,ia7)
        message=''; nharderror=-1
        c1_7=a7w(1); c2_7=a7w(2)
        call a7d(this,c1_7,c2_7,grid7,xdt7,f7,nharderror,message,xsnr7)
        if(nharderror.ge.0 .and. message.ne.'') then
          idupe=0
          do i=1,ndecodes; if(decodes(i).eq.message) idupe=1; enddo
          if(idupe.eq.0) then
            ndecodes=min(ndecodes+1,100); decodes(ndecodes)=message
! MSHV's AP7 quality, 1-(nharderrors+dmin)/60, with dmin always 0 there: its
! ft2_a7d never hands dmin back to the caller.  The port first used the real
! dmin, which rejected most true AP7 decodes at -16 dB (2026-10-07: with the
! pair supplied the true message ranked first 33/40 times, accepted 7/40).
            if(nint(f7).ge.nfa .and. nint(f7).le.nfb .and. 1.0-real(nharderror)/60.0.ge.a7qual) then
              nsnr=nint(max(-21.0,xsnr7)); msg26=message(1:26); servis4='7'
              call this%callback(nsnr,xdt7,f7,msg26,servis4)
              nFT2decd=nFT2decd+1; sumxdtt(1)=sumxdtt(1)+xdt7
            endif
            call a7_save(xdt7,f7,message)
          endif
        endif
      enddo
    endif
    call a7_roll(ja7)

    return
  end subroutine decode

! ---- JTDX-VU: AP7 support (port of MSHV's ft2_a7_save / ft2_a7d / history) ----

  integer function a7_parity(nutc)
! even (0) or odd (1) 3.75 s slot.  nutc has 1 s resolution; periods start at
! 0, 3.75, 7.5, 11.25 s, so the seconds value alone fixes the quarter seconds
! (3 -> .75, 7 -> .50, 11 -> .25 in each 15 s) - MSHV's ft2_even_odd.
    integer nutc,ss,mm,ms,tp
    ss=mod(nutc,100); mm=mod(nutc/100,100); ms=0
    if(mod(ss,15).eq.3) ms=75
    if(mod(ss,15).eq.7) ms=50
    if(mod(ss,15).eq.11) ms=25
    tp=mod(mm*6000+ss*100+ms,750)
    a7_parity=0; if(tp.ge.375) a7_parity=1
  end function a7_parity

  subroutine a7_save(dt,f,msg)
! remember "CALL1 CALL2[ GRID]" from a decode this period, for a7d two periods on
    use ft2_mod1, only : A7MAX,na7dec,a7dt,a7f,a7msg,na7utc
    use packjt77
    real dt,f
    character*37 msg,m2
    character*13 w(19)
    integer nw(19),nwords,j,i,z,i2
    if(index(msg,'/').gt.0 .or. index(msg,'<').gt.0) return
    m2=msg; call split77(m2,nwords,nw,w)
    if(nwords.lt.2) return
    if(w(1)(1:3).eq.'CQ_') return
    j=a7_parity(na7utc)
    i=na7dec(1,j)+1
    if(i.gt.A7MAX) return
    a7dt(1,j,i)=dt; a7f(1,j,i)=f
    a7msg(1,j,i)=trim(w(1))//' '//trim(w(2))
    if(w(1)(1:3).eq.'CQ ' .and. len_trim(w(2)).le.2 .and. nwords.ge.3) a7msg(1,j,i)='CQ '//trim(w(2))//' '//trim(w(3))
    if(isgrid4(w(nwords))) a7msg(1,j,i)=trim(a7msg(1,j,i))//' '//w(nwords)(1:4)
! a pair already decoded last period at this frequency was subtracted: don't retry it
    do z=1,na7dec(2,j)
      if(a7f(2,j,z).le.-98.0) cycle
      i2=index(a7msg(2,j,z),' '//trim(w(2)))
      if(abs(f-a7f(2,j,z)).le.3.0 .and. i2.ge.2) a7f(2,j,z)=-98.0
    enddo
    na7dec(1,j)=i
  end subroutine a7_save

  logical function isgrid4(w)
    character*13 w
    isgrid4=len_trim(w).eq.4 .and. w(1:1).ge.'A' .and. w(1:1).le.'R' .and. w(2:2).ge.'A' .and. w(2:2).le.'R' &
         .and. w(3:3).ge.'0' .and. w(3:3).le.'9' .and. w(4:4).ge.'0' .and. w(4:4).le.'9'
  end function isgrid4

  subroutine a7_roll(j)
! end of period: this period's table becomes "the one before"; drop the history
! after a gap of more than 11 s or two empty periods
    use ft2_mod1, only : A7MAX,na7dec,a7dt,a7f,a7msg,na7utc,na7lastutc,na7zerop
    integer j,iz,i,t1,t0
    iz=na7dec(1,j)
    if(iz.eq.0) then; na7zerop=na7zerop+1; else; na7zerop=0; endif
    do i=1,iz
      a7dt(2,j,i)=a7dt(1,j,i); a7f(2,j,i)=a7f(1,j,i); a7msg(2,j,i)=a7msg(1,j,i)
    enddo
! Staleness is measured from the previous PERIOD, decoded or not (MSHV's nutc0):
! a gap of more than 11 s means the decoder was stopped, three empty periods
! mean the pair is old news.  Measuring from the last period WITH decodes
! threw away the first decode after any quiet spell - the normal state between
! QSOs at the floor - so the retry two periods later never happened (2026-10-07,
! 20-QSO run: MSHV 33 AP7 decodes at -15 dB, JTDX-VU 2).
    t1=mod(na7utc,100)+60*mod(na7utc/100,100)+3600*(na7utc/10000)
    t0=-1; if(na7lastutc.ge.0) t0=mod(na7lastutc,100)+60*mod(na7lastutc/100,100)+3600*(na7lastutc/10000)
    if((t0.ge.0 .and. t1-t0.gt.11) .or. na7zerop.gt.2) then
      na7dec(2,0)=0; na7dec(2,1)=0; na7zerop=0
      if(iz.gt.0) na7dec(2,j)=iz           ! this period's own pairs are fresh
    else
      if(iz.gt.0) na7dec(2,j)=iz
    endif
    na7dec(1,j)=0
    na7lastutc=na7utc
  end subroutine a7_roll

  subroutine a7d(this,call_1,call_2,grid4,xdt,f0,nharderrors,message,xsnr)
! re-sync at f0 and test ~158 likely messages for the pair; accept the best if it
! stands clear of the second best (MSHV ft2_a7d, from WSJT-X ft8 a7)
    use packjt77
    use ft2_mod1, only : ddf2,la7dbg,a7qual
    include 'ft2/ft2_params.f90'
    parameter (NSS=NSPS/NDOWN,NDMAX=NMAX/NDOWN,MAXMSG=158)
    class(ft2_decoder), intent(inout) :: this
    character*13 call_1,call_2
    character*4 grid4
    real xdt,f0,xsnr
    integer nharderrors
    character*37 message,msg,msgsent,msgbest
    character*77 c77
    character*6 bc1,bc2
    complex cd2(0:NDMAX-1),cb(0:NDMAX-1),cd(0:NN*NSS-1)
    complex ctwk(2*NSS),ctwk2(2*NSS,-16:16)
    real a(5),bitmetrics(2*NN,3),s4(0:3,NN)
    real llra(2*ND),llrb(2*ND),llrc(2*ND),llrd(2*ND),dmm(MAXMSG)
    integer*1 message77(77),rvec(77),cw(2*ND),hd(2*ND)
    integer itone(NN),hbits(2*NN),icos4a(0:3),icos4b(0:3),icos4c(0:3),icos4d(0:3)
    logical badsync,dobigfft,std_1,std_2,unpk77_success,cok
    real*8 pbest,pow0,s88,da,db,dc,dd,dm,dmin,dmin2
    real sync,smax,smax1,sum2,xibest,sm1,sp1,den
    integer ibest,idfbest,iseg,isync,idf,istart,idfmin,idfmax,idfstp,ibmin,ibmax,ibstp,i,j,k,z,ipt
    integer ns1,ns2,ns3,ns4,nsync_qual,count_msg,i3,n3,pos,isnr,it,np
    data rvec/0,1,0,0,1,0,1,0,0,1,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,0,1,1,0, &
      1,0,0,1,0,1,1,0,0,0,0,1,0,0,0,1,0,1,0,0,1,1,1,1,0,0,1,0,1, &
      0,1,0,1,0,1,1,0,1,1,1,1,1,0,0,0,1,0,1/
    data icos4a/0,1,3,2/,icos4b/1,0,2,3/,icos4c/2,3,1,0/,icos4d/3,2,0,1/
    save ctwk2,first7
    logical first7
    data first7/.true./
    if(first7) then
! the same frequency-tweak table the main decoder builds (twopi must be set in
! ft8_mod1 first - the decoder process does that; ft2dec sets it itself)
      do idf=-16,16
        a=0.; a(1)=real(idf)
        ctwk=1.
        call twkfreq1(ctwk,0,2*NSS,2*NSS,(12000.0/NDOWN)/2.0,a,ctwk2(:,idf))
      enddo
      first7=.false.
    endif
    std_1=.true.; if(call_1.ne.'CQ') then; call chkcall(call_1,bc1,cok); std_1=cok .and. index(call_1,'/').eq.0; endif
    call chkcall(call_2,bc2,cok); std_2=cok .and. index(call_2,'/').eq.0
    nharderrors=-1
    dobigfft=.true.
    call ft2_downsample(dobigfft,f0,cd2)
    sum2=sum(cd2*conjg(cd2))/(real(NMAX)/real(NDOWN))
    if(sum2.gt.0.0) cd2=cd2/sqrt(sum2)
    do iseg=1,3
      idfbest=0; ibest=-1; smax=-99.0; smax1=-99.0
      do isync=1,2
        if(isync.eq.1) then
          idfmin=-12; idfmax=12; idfstp=3
          if(iseg.eq.1) then; ibmin=216; ibmax=1120
          elseif(iseg.eq.2) then; smax1=smax; ibmin=1121; ibmax=2024
          else; ibmin=-688; ibmax=215; endif
          ibstp=4
        else
          idfmin=idfbest-4; idfmax=idfbest+4; idfstp=1
          ibmin=ibest-5; ibmax=ibest+5; ibstp=1
        endif
        ibest=-1; smax=-99.0; idfbest=0
        do idf=idfmin,idfmax,idfstp
          do istart=ibmin,ibmax,ibstp
            call sync2d(cd2,istart,ctwk2(:,idf),1,sync)
            if(sync.gt.smax) then; smax=sync; ibest=istart; idfbest=idf; endif
          enddo
        enddo
      enddo
      if(la7dbg) write(0,'(a,2a10,i3,f8.3,i6,i4)') 'a7 sync ',trim(call_1),trim(call_2),iseg,smax,ibest,idfbest
      if(smax.lt.0.50) cycle
      if(iseg.gt.1 .and. smax.lt.smax1) cycle
      f1=f0+real(idfbest)
      if(f1.le.10.0 .or. f1.ge.4990.0) cycle
      call ft2_downsample(dobigfft,f1,cb)
      sum2=sum(abs(cb)**2)/(real(NSS)*NN)
      if(sum2.gt.0.0) cb=cb/sqrt(sum2)
      cd=0.
      if(ibest.ge.0) then
        it=min(NDMAX-1,ibest+NN*NSS-1); np=it-ibest+1; cd(0:np-1)=cb(ibest:it)
      else
        cd(-ibest:ibest+NN*NSS-1)=cb(0:NN*NSS+2*ibest-1)
      endif
      call get_ft2_bitmetrics(cd,bitmetrics,badsync)
      if(badsync) cycle
      xibest=real(ibest)
      if(ibest.gt.0 .and. ibest.lt.NDMAX-1) then
        call sync2d(cd2,ibest-1,ctwk2(:,idfbest),1,sm1); call sync2d(cd2,ibest+1,ctwk2(:,idfbest),1,sp1)
        den=sm1-2.0*smax+sp1
        if(abs(den).gt.1.e-6) xibest=real(ibest)+0.5*(sm1-sp1)/den
      endif
      hbits=0; where(bitmetrics(:,1).ge.0) hbits=1
      ns1=count(hbits(  1:  8).eq.(/0,0,0,1,1,0,1,1/)); ns2=count(hbits( 67: 74).eq.(/0,1,0,0,1,1,1,0/))
      ns3=count(hbits(133:140).eq.(/1,1,1,0,0,1,0,0/)); ns4=count(hbits(199:206).eq.(/1,0,1,1,0,0,0,1/))
      nsync_qual=ns1+ns2+ns3+ns4
      if(la7dbg) write(0,'(a,i3,l2)') 'a7 nsync_qual ',nsync_qual,badsync
      if(nsync_qual.lt.10) cycle
      llra(1:58)=bitmetrics(9:66,1); llra(59:116)=bitmetrics(75:132,1); llra(117:174)=bitmetrics(141:198,1)
      llrb(1:58)=bitmetrics(9:66,2); llrb(59:116)=bitmetrics(75:132,2); llrb(117:174)=bitmetrics(141:198,2)
      llrc(1:58)=bitmetrics(9:66,3); llrc(59:116)=bitmetrics(75:132,3); llrc(117:174)=bitmetrics(141:198,3)
      llra=2.83*llra; llrb=2.83*llrb; llrc=2.83*llrc
      do i=1,2*ND
        if(abs(llra(i)).ge.abs(llrb(i)) .and. abs(llra(i)).ge.abs(llrc(i))) then; llrd(i)=llra(i)
        elseif(abs(llrb(i)).ge.abs(llrc(i))) then; llrd(i)=llrb(i); else; llrd(i)=llrc(i); endif
      enddo
! symbol magnitudes for the SNR of the winning message
      do k=1,NN
        s4(0:3,k)=0.
      enddo
      pbest=0.d0; dmin=1.d30; msgbest=''; count_msg=MAXMSG; dmm=1.e30
      do i=0,MAXMSG-1
        call a7_msg(call_1,std_1,call_2,std_2,grid4,i,msg,count_msg)
        if(i.ge.count_msg) exit
        i3=-1; n3=-1
        call pack77(msg,i3,n3,c77,1)
        read(c77,'(77i1)') message77
        message77=mod(message77+rvec,2)
        call encode174_91(message77,cw)
        if(msg(1:6).eq.'QU1RK ') then; msgsent=msg
        else; call unpack77(c77,1,msgsent,unpk77_success,1); if(.not.unpk77_success) cycle; endif
        da=0.d0; db=0.d0; dc=0.d0; dd=0.d0
        do z=1,2*ND
          if(llra(z).ge.0.0) then; hd(z)=1; else; hd(z)=0; endif
          if(hd(z).ne.cw(z)) da=da+abs(llra(z))
          if(llrb(z).ge.0.0) then; hd(z)=1; else; hd(z)=0; endif
          if(hd(z).ne.cw(z)) db=db+abs(llrb(z))
          if(llrc(z).ge.0.0) then; hd(z)=1; else; hd(z)=0; endif
          if(hd(z).ne.cw(z)) dc=dc+abs(llrc(z))
          if(llrd(z).ge.0.0) then; hd(z)=1; else; hd(z)=0; endif
          if(hd(z).ne.cw(z)) dd=dd+abs(llrd(z))
        enddo
        dm=min(da,db,dc,dd); dmm(i+1)=dm
        if(dm.lt.dmin) then
          dmin=dm; msgbest=msgsent; nharderrors=0
          do z=1,2*ND
            if(dm.eq.da) then; if(real(2*cw(z)-1)*llra(z).lt.0.0) nharderrors=nharderrors+1
            elseif(dm.eq.db) then; if(real(2*cw(z)-1)*llrb(z).lt.0.0) nharderrors=nharderrors+1
            elseif(dm.eq.dc) then; if(real(2*cw(z)-1)*llrc(z).lt.0.0) nharderrors=nharderrors+1
            else; if(real(2*cw(z)-1)*llrd(z).lt.0.0) nharderrors=nharderrors+1; endif
          enddo
        endif
      enddo
! second-best distance, for the acceptance ratio
      pos=1; do z=2,count_msg; if(dmm(z).lt.dmm(pos)) pos=z; enddo
      dmm(pos)=1.e30
      dmin2=1.d30; do z=1,count_msg; if(dmm(z).lt.dmin2) dmin2=dmm(z); enddo
      message=msgbest
      if(la7dbg) write(0,'(a,a30,2f9.2,f7.2,i4)') 'a7 best ',msgbest(1:30),dmin,dmin2,dmin2/max(dmin,1.d-4),nharderrors
      if(dmin.eq.0.d0) dmin=0.0001d0
      if(dmin.gt.100.d0 .or. dmin2/dmin.lt.1.27d0) nharderrors=-1
      if(msgbest(1:3).eq.'CQ ' .and. std_2 .and. grid4.eq.'    ') nharderrors=-1
      if(msgbest(1:6).eq.'QU1RK ' .or. message.eq.'') nharderrors=-1
      if(nharderrors.gt.95) nharderrors=-1
! The quality gate (1 - hard errors/60 >= a7qual) is applied by the caller, as
! MSHV does: a decode that fails it is not shown but still goes into the history.
      if(nharderrors.ge.0) then
! SNR from the sync strength, on the same scale as the main decoder's candidates
        xsnr=max(-21.0,10.0*log10(max(1.e-3,smax))-11.3)
        xdt=xibest/1333.33-0.5
        f0=f1
        return
      endif
    enddo
    nharderrors=-1
  end subroutine a7d

  subroutine a7_msg(call_1,std_1,call_2,std_2,grid4,i,msg,count_msg)
! the i-th likely message for a pair (MSHV SetAp7Msg): bare, RRR, RR73, 73,
! CQ+grid, grid, then reports -26..+49 without and with R
    character*13 call_1,call_2
    character*4 grid4
    character*37 msg
    logical std_1,std_2
    integer i,count_msg,isnr
    character*3 rpt
    msg=trim(call_1)//' '//trim(call_2)
    if(call_1.eq.'CQ' .and. i.ne.4) msg='QU1RK '//trim(call_2)
    if(.not.std_1) then
      if(i.eq.0 .or. i.ge.5) msg='<'//trim(call_1)//'> '//trim(call_2)
      if(i.ge.1 .and. i.le.3) msg=trim(call_1)//' <'//trim(call_2)//'>'
    elseif(.not.std_2) then
      if(call_1.eq.'CQ' .and. i.ne.4) then; msg='QU1RK '//trim(call_2)
      else
        if(i.le.3 .or. i.eq.5) msg='<'//trim(call_1)//'> '//trim(call_2)
        if(i.ge.6) msg=trim(call_1)//' <'//trim(call_2)//'>'
      endif
    endif
    if(i.eq.1) msg=trim(msg)//' RRR'
    if(i.eq.2) msg=trim(msg)//' RR73'
    if(i.eq.3) msg=trim(msg)//' 73'
    if(i.eq.4) then
      if(std_2) then
        msg='CQ '//trim(call_2)
        if(grid4.ne.'RR73') msg=trim(msg)//' '//grid4
      else
        msg='CQ '//trim(call_2)
      endif
    endif
    if(i.eq.5 .and. std_2) msg=trim(msg)//' '//grid4
    if(i.ge.6 .and. i.lt.158) then
      if(i.gt.12 .and. msg(1:6).eq.'QU1RK ') then; count_msg=i; return; endif
      isnr=-26+(i-6)/2
      if(isnr.ge.0) then; write(rpt,'(a1,i2.2)') '+',isnr; else; write(rpt,'(a1,i2.2)') '-',abs(isnr); endif
      if(mod(i+1,2).eq.1) then; msg=trim(msg)//' '//rpt
      else; msg=trim(msg)//' R'//rpt; endif
    endif
  end subroutine a7_msg

end module ft2_decode
