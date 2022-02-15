
# SEL32 Concept/32 Simulator

This is a working simulator for the SEL Concept/32 line of computers.  The
current version is for the SEL 32/27, 32/67, 32/87, 32/97, V6, and V9
computers.  Support for 32/55, and 32/75 computers may be added in the future.

All of the processors except for the 32/77 can run the Gould diags.  MPX 1.X
support for the 32/77 computers may be added in the future.  The level one
diags are run as part of the SIMH build of SEL32 to verify that the simulator
is operating correctly.  The diags are located in the tests directory.  Diag.tap
contains the diagnostic programs and diag.ini contains the directives to
configure and run the SEL32 simulator.

This simulator is capable of running UTX2.1A, UTX2.1B, MPX 1.5F, MPX 3.4,
MPX 3.5 and MPX 3.6.  It is capable of creating a disk image for the
O/S from a UTX or MPX SDT tape. The disk image can be booted, initialized,
and can run many of the UTX and MPX utilities and programs. Ethernet is
supported on UTX and will be added to MPX in the future.  Eight terminals
can be used to access MPX via Telnet port 4747. The sumulator has support
for excess 64 floating point arithmetic and passes the 32/27 and 32/67 FP
diags.  UTX is the SEL version of System V Unix and BSD Unix ported to the
V6 and V9 processors.  UTX utilizes the basemode instruction set and a
virtual memory system supported by the V6 & V9 CPUs.  The system needs
further testing to solidify the SEL32 simulator code in all of the
supported environmenets.

Available tap tools in taptools directory:
./taptools   - set of tools to work with .tap formatted tapes.  Also tools
               to convert between MPX and UNIX file formats.  See README
               file in the taptools directory and source for descriptions.

Available Level One Diagnostic boot tape in tests directory:
diag.ini     - command file to start diags. Type "./sel32 tests/diag.ini"
diag.tap     - bootable level one diagnostic tape w/auto testing.
               Set cpu type to 32/27, 32/67, 32/87, 32/97, V6 or V9.  All
               cpu models now run all diagnostics provided on the
               diagnostic tape.  Running DEXP stand alone causes input
               to stop after a few characters are entered.  More testing
               is still required.

               CV.CSL - Firmware control diag.  Disabled in auto testing.
               CV.CP1 - CPU diag part 1 runs OK.
               CV.CP2 - CPU diag part 2 runs OK.
               CV.CP3 - CPU diag part 3 runs OK.
               CV.EAD - Effective address diag runs OK.
               CV.BRD - Base register instruction diag runs OK, except 32/27.
               CV.INT - Interrupt diag runs OK.
               CV.TRP - Traps diag runs OK.
               CV.CMD - Cache/Shadow diag.  Disabled in auto testing.
               CN.MMM - Non virtual memory diag runs OK.
               VM.MMM - Virtual memory diag for V6 & V9 runs OK.
               CV.IPT - IPU trap diag.  Disabled in auto testing.
               CV.CSD - WCS read/write trap diag.  Disabled in auto testing.
               CV.CON - Operators Console runs all tests for all CPUs.
               CV.DXP - Diagnostic executive for level 2 diags. OK in batch.
               67.FPT - Level two floating point diag runs under DXP OK.
               CV.ITD - Level two interval timer diag runs under DXP OK.

               Set GPR[0] = 0xffffffff before booting from tape to disable the
               auto test and go to the Diagnostic Overlay Loader (DOL>) prompt.
               Testing is extremely difficult without any source for the
               diagnostics.  Updates to follow as tests are corrected.

Other MPX versions support:
               I have recently received some old MPX 3.X save tapes.  Using these
               I have been able to hand build a MPX3.6 SDT tape that can be used
               to install MPX3.6.  Once installed, the system can be used to build
               a new user SDT tape and install it elsewhere.  Both based and non-
               based O/S images can be created.  More images for installation will
               be made available in the future as I work my way through the save
               tapes. I still do not have a master SDT tape for any of the MPX 1.X
               or MPX 3.X systems.  I have a 1600/6250 BPI tape drive that can read
               9 track tapes and convert them to .tap files.  If you have a master
               SDT, I would be very thankfull.  Please keep looking.

James C. Bevier
12/31/2021 

