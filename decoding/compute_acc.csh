#!/bin/csh -f

if ($#argv != 2) then
   echo "usage: $0 <correct file> <hypothesis file>"
   exit 1
endif

set correct = $1
set hypothesis = $2

set tmpf1 = /tmp/xx1.$$
set tmpf2 = /tmp/xx2.$$
onintr cleanup
cp  $hypothesis $tmpf2
set align = ../training/s3trainer/bin.linux/align
$align  -def $correct \
        -hyp $tmpf2 \
        >&! $hypothesis.align
grep -i accuracy $hypothesis.align

cleanup:
/bin/rm -f $tmpf1 $tmpf2

exit 0
