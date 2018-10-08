!rm sel.log
set debug sel.log
;set CPU 32/27 256K
set CPU 32/97 2M
;set debug stderr
;set mta debug=cmd;detail;exp;data
;set mta debug=inst;cmd;detail;exp
;set inq debug=cmd;detail
;set cpu debug=cmd;exp
;set cpu debug=cmd;exp;inst;detail
;set cpu debug=exp;
;set con debug=cmd;exp;data
;set mta debug=cmd;exp;inst;data;detail
;set mta debug=cmd
;set dma debug=cmd;exp;detail;data
;set dma debug=cmd;exp;detail;data
;
set comc enable
at comc 4747
;
set coml0 enable
set coml1 enable
set coml2 enable
set coml3 enable
set coml4 enable
set coml5 enable
set coml6 enable
set coml7 enable
;
;;set con debug=CMD
;at con
;
set lpr enable
at lpr lprout
;
at mta0 mpxsdt4.tap
;at mta0 mpxsdt5.tap
;at mta0 mpxsdt6r.tap
at sdb0 diskfile4
at sdb1 diskfile5
;at dma0 diskfile6r
;bo dma0
bo mta0
