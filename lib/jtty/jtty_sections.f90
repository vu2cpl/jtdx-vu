! JTDX-VU: minimal stand-in for WSJT-X 3.2's packjt77_grammar module.
!
! The JTTY code (jtty_mod, jtty_source_codec, genjtty) needs only the ARRL/RAC
! section table from that 1,500-line module, for Field Day class/section
! atoms.  Reproducing just that here - under the SAME module name - lets the
! vendored JTTY sources build unmodified against JTDX's older packjt77.
!
! Table and index function are verbatim from WSJT-X 3.2.0-rc1
! lib/77bit/packjt77_grammar.f90 (K1JT, K9AN, G4KLA et al., GPL v3).

module packjt77_grammar

  implicit none

  integer, parameter :: PACK77_NSEC=86
  character(len=3), parameter :: PACK77_ARRL_SECTIONS(PACK77_NSEC)=(/ &
       "AB ","AK ","AL ","AR ","AZ ","BC ","CO ","CT ","DE ","EB ",  &
       "EMA","ENY","EPA","EWA","GA ","GH ","IA ","ID ","IL ","IN ",  &
       "KS ","KY ","LA ","LAX","NS ","MB ","MDC","ME ","MI ","MN ",  &
       "MO ","MS ","MT ","NC ","ND ","NE ","NFL","NH ","NL ","NLI",  &
       "NM ","NNJ","NNY","TER","NTX","NV ","OH ","OK ","ONE","ONN",  &
       "ONS","OR ","ORG","PAC","PR ","QC ","RI ","SB ","SC ","SCV",  &
       "SD ","SDG","SF ","SFL","SJV","SK ","SNJ","STX","SV ","TN ",  &
       "UT ","VA ","VI ","VT ","WCF","WI ","WMA","WNY","WPA","WTX",  &
       "WV ","WWA","WY ","DX ","PE ","NB " /)

contains

  integer function pack77_arrl_section_index(section) result(isec)
    character(len=*), intent(in) :: section
    integer :: i

    isec=-1
    if(len(section).lt.3) return
    do i=1,PACK77_NSEC
       if(PACK77_ARRL_SECTIONS(i).eq.section(1:3)) then
          isec=i
          return
       endif
    enddo
  end function pack77_arrl_section_index

  character(len=3) function pack77_arrl_section_name(isec) result(section)
    integer, intent(in) :: isec

    section='   '
    if(isec.ge.1 .and. isec.le.PACK77_NSEC) section=PACK77_ARRL_SECTIONS(isec)
  end function pack77_arrl_section_name

end module packjt77_grammar
