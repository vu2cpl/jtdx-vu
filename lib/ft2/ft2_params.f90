! JTDX-VU: FT2 - FT4's protocol at twice the speed (IU8LMC, Martino, ARI
! Caserta; C++ reference in MSHV by LZ2HV).  Same LDPC(174,91), same 103
! channel symbols with four 4x4 Costas arrays, GFSK BT=1; 288 samples per
! symbol at 12000 S/s (41.67 baud), 3.75 s T/R period.  Ported from JTDX's
! FT4 chain (K1JT/K9AN et al., UA3DJY) by halving every timing constant.
! FT2
parameter (KK=91)                     !Information bits (77 + CRC14)
parameter (ND=87)                     !Data symbols
parameter (NS=16)                     !Sync symbols
parameter (NN=103)                    !Sync and data symbols NN=NS+ND
parameter (NN2=105)                   !Total channel symbols NN2=NS+ND+2
parameter (NSPS=288)                  !Samples per symbol at 12000 S/s
parameter (NZ=29664)                  !Sync and Data samples NZ=NSPS*NN
parameter (NZ2=30240)                 !Total samples in shaped waveform NZ2=NSPS*NN2
parameter (NMAX=41472)                !Samples in iwave: 12 blocks of 3456 (3.456 s of the 3.75 s period)
parameter (NFFT1=1408, NH1=704)       !Length of FFTs for symbol spectra (NSPS + 2*128 pad), NH1=NFFT1/2
parameter (NSTEP=NSPS)                !Coarse time-sync step size
parameter (NHSYM=139)                 !Number of symbol spectra NHSYM=(NMAX-NFFT1)/NSTEP
parameter (NDOWN=9)                   !Downsample factor: 1333.33 S/s, 32 samples per symbol
