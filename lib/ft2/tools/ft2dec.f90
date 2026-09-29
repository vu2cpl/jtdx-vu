! JTDX-VU: FT2 - FT4's protocol at twice the speed (IU8LMC, Martino, ARI
! Caserta; C++ reference in MSHV by LZ2HV).  Same LDPC(174,91), same 103
! channel symbols with four 4x4 Costas arrays, GFSK BT=1; 288 samples per
! symbol at 12000 S/s (41.67 baud), 3.75 s T/R period.  Ported from JTDX's
! FT4 chain (K1JT/K9AN et al., UA3DJY) by halving every timing constant.
! ft2dec: decode FT2 from 12 kHz 16-bit wav files with JTDX's FT2 decoder.
!   ft2dec nfqso depth file.wav [file2.wav ...]
! Prints "snr dt freq message" per decode.  The first 41472 samples of each
! file (3.456 s) are used, as the GUI would.

module ft2dec_cb
  use ft2_decode
  implicit none
  type, extends(ft2_decoder) :: printing_ft2_decoder
    integer :: n = 0
  end type
contains
  subroutine ft2_print(this,snr,dt,freq,decoded,servis4)
    class(ft2_decoder), intent(inout) :: this
    integer, intent(in) :: snr
    real, intent(in) :: dt
    real, intent(in) :: freq
    character(len=26), intent(in) :: decoded
    character(len=1), intent(in) :: servis4
    write(*,'(i4,f6.2,i6,2x,a26,a1)') snr,dt,nint(freq),decoded,servis4
    select type(this)
    type is (printing_ft2_decoder)
      this%n=this%n+1
    end select
  end subroutine ft2_print
end module ft2dec_cb

program ft2dec
  use wavhdr
  use ft2dec_cb
  use ft2_mod1
  use ft8_mod1, only : sumxdtt,avexdt,mycall,hiscall,twopi
  implicit none
  type(hdr) :: h
  type(printing_ft2_decoder) :: dec
  character(len=200) :: fname
  character(len=16) :: arg
  integer :: nargs,ifile,nfqso,ndepth,nread
  integer(2) :: iwave(41472)
  nargs=command_argument_count()
  if(nargs.lt.3) then
    print*,'Usage: ft2dec nfqso depth file.wav [...]'
    stop
  endif
  call get_command_argument(1,arg); read(arg,*) nfqso
  call get_command_argument(2,arg); read(arg,*) ndepth
  mycall=' '; hiscall=' '; avexdt=0.0; sumxdtt=0.0; twopi=8.0*atan(1.0)   ! twkfreq1 reads ft8_mod1 twopi
  llagcc2=.false.; lfilter2=.false.; lhidetest2=.false.; lhidetelemetry2=.false.
  do ifile=3,nargs
    call get_command_argument(ifile,fname)
    open(10,file=trim(fname),status='old',access='stream')
    read(10) h
    nread=min(41472,h%ndata/2)
    iwave=0
    read(10) iwave(1:nread)
    close(10)
    ddf2=iwave
    nFT2decd=0
    write(*,'(a)') trim(fname)
    call dec%decode(ft2_print,0,nfqso,200,4900,ndepth,.false._1,.false._1)
    write(*,'(a,i3)') '  decodes:',dec%n
    dec%n=0
  enddo
end program ft2dec
