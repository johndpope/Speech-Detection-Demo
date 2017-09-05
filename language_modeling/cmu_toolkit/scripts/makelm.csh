#!/bin/csh -f

if ($#argv != 4) then
   echo "USAGE: <transciptfile> <vocabfile> <N (as in N-gram)> <ARPAlmfilename>"
   exit 0
endif

set textf = $1
set vocab = $2
set Ngram = $3
set lmfile = $4

set text = text.$$
set idngm = idngram.$$
set contxt = cont.$$

onintr docleanup

set bin = ../toolkit/CMU-Cam_Toolkit_v2_linux/bin

cp $textf $text
echo '<s>' >! $contxt

$bin/text2idngram -vocab $vocab -n $Ngram -write_ascii < $text > $idngm

#$bin/idngram2lm -idngram $idngm -vocab $vocab -arpa $lmfile -context $contxt -absolute -cutoffs 0 0 -ascii_input -n $Ngram
$bin/idngram2lm -idngram $idngm -vocab $vocab -arpa $lmfile -context $contxt -absolute -ascii_input -n $Ngram

#$bin/idngram2lm -idngram $idngm -vocab $vocab -arpa $lmfile -context $contxt -good_turing -ascii_input -n $Ngram -cutoffs 1 1
#$bin/idngram2lm -idngram $idngm -vocab $vocab -arpa $lmfile -context $contxt -good_turing -ascii_input -n $Ngram

../localtools/lm3g2dmp $lmfile ../lm_files/

docleanup:
/bin/rm $text $idngm $contxt

exit 0

