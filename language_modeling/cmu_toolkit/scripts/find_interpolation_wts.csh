#!/bin/csh -f

if ($#argv != 4) then
   echo "USAGE: <ARPA LM1> <ARPA LM2> <text_to_optimize_lms_on> <filename_for_writing_optimization_wts.>"
   exit 0
endif

set lm1 = $1
set lm2 = $2
set opt_text = $3
set wtsfile = $4

set prob_stream1 = prob_stream1.$$
set prob_stream2 = prob_stream2.$$

onintr docleanup

set bin = ../toolkit/CMU-Cam_Toolkit_v2_linux/bin

echo "perplexity -text $opt_text -probs $prob_stream1" | $bin/evallm -arpa $lm1

echo "perplexity -text $opt_text -probs $prob_stream2" | $bin/evallm -arpa $lm2

$bin/interpolate + $prob_stream1 + $prob_stream2 -out_lambdas $wtsfile -stop_ratio 0.999 -probs combined_probs 

#see documentation in 
#http://svr-www.eng.cam.ac.uk/~prc14/toolkit_documentation.html#interpolate

docleanup:
/bin/rm $prob_stream1 $prob_stream2

exit 0


