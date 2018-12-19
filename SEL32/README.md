
# SEL32 Development Simulator

This is a working copy of a simulator for the SEL Concept/32 computer.
The current test version is for the SEL 32/27, 32/67, 32/87, 32/97 computers.

# SEL Concept/32 

This simulator is running a test version of MPX-32 1.5F.  It is capable of
creating a disk image of the O/S from a SDT tape.  The disk image can be
booted, initialized, and run many of the MPX utilities; including OPCOM & TSM.
Eight terminals can be used to access TSM via Telnet port 4747.  Initial
support has been added for excess 64 floating point arithmetic.  More testing
is still required.  The sim32disk.gz can be uncompressed and booted with the
sel32.ini.27.sim32.disk initialization file.  The sim32sdt.tap.gz file can
also be uncompressed and started with the sel32.ini.27.sim32.tape initialization
file to install from tape.

Available tap tools:
taptools.tgx - set of tools to work with .tap formatted tapes.  Also tools
               to convert between MPX and UNIX file formats.  See README
               file and source for descriptions.

Available disk images:
sim32disk.gz - bootable 300mb disk with MPX1.5F install

Available configuration SDT tapes:
sim32sdt.tap - MPX 1.5f user SDT install tape.  Uses 300mb disk, IOP 8-line
               serial terminals via telnet port 4747.  Line printer, 4 mag
               tapes and console terminal.  Terminal wakeup chan is ^G and
               console wakeup chars are @@A.  Install using TSM from console
               after exiting opcom.  Use filemgr to restore files from tape.
               TSM> A3 IN=M91000,TAP
               TSM> A4 SLO=UT
               TSM> FILEMGR
               FIL> RESTORE
               FIL> X

James C. Bevier
 
