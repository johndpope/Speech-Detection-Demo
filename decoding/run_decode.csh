#!/bin/csh -f

if ($#argv < 2) then
    echo "Usage: $0 part npart [startskip]"
    exit
endif


set ctlfn     = ../decoding/decode.ctl
set mdef      = ../model_architecture/ads.80-7200-40f.1-3.6000.mdef
set modeldir  = ../model_parameters/ads.80-7200-40f.1-3.cd_continuous.6000sen_8gau
#set quantizedmodels = ./hub4opensrc.cd_continuous_8gau/8gau.6000sen.quant

set outname   = decoded_output
set cepdir = ../features/
set cepext    = 80-7200_40filts.mfc
set varnorm   = no
#set MLLRctlfn = /net/minnie/usr3/rsingh/decoding/MLLRsetup/controlfiles.new/baseline_fixed.mllrctl2

#set lmfile    = ../language_modeling/lm_files/wsj0.lm.DMP
set lmfile    = ../language_modeling/lm_files/youtube0.lm.DMP

set dictfn    = ../decoding/decode.dict
set fdictfn   = ../decoding/decode.fillerdict

#set outlatdir = ./lattices
set PGM = ../decoding/s3decoder/bin/linux/s3decode-anytopo
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

set echo
limit coredumpsize 0
unlimit datasize

onintr docleanup

set part  = $1
set npart = $2
if ($#argv > 2) then
  set startskip = $4:q
else
  set startskip = 0
endif

set nlines = `wc $ctlfn | awk '{print $1}'`
echo "$nlines"
set ctloffset ctlcount
@ ctloffset = $startskip + ( ( $nlines * ( $part - 1 ) ) / $npart )
@ ctlcount = ( ( $nlines * $part ) / $npart ) - $ctloffset
echo "Doing $ctlcount segments starting at number $ctloffset"



if ( $startskip == 0 ) then
  set logfile = ./result/${outname}-${part}.log
  set matchfile = ./result/${outname}-${part}.match
else
  set logfile = ./result/${outname}-${part}-${startskip}.log
  set matchfile = ./result/${outname}-${part}-${startskip}.match
endif

if (! -d $cepdir) then
  echo "ERROR: $cepdir does not exist" >> $logfile
  echo "ERROR: $cepdir does not exist"
  exit 1
endif

if (! -f $lmfile) then
  echo "ERROR: $lmfile does not exist" >> $logfile
  echo "ERROR: $lmfile does not exist"
  exit 1
endif

if (! -f $ctlfn) then
  echo "ERROR: $ctlfn does not exist" >> $logfile
  echo "ERROR: $ctlfn does not exist"
  exit 1
endif
$PGM \
        -logbase    1.0001 \
        -bestpath   0 \
        -mdeffn     $mdef \
        -senmgaufn  .cont. \
        -meanfn     $modeldir/means \
        -varfn      $modeldir/variances \
        -mixwfn     $modeldir/mixture_weights \
        -tmatfn     $modeldir/transition_matrices \
        -langwt     10.5  \
#        -feat       s3_1x39 \
        -feat       1s_c_d_dd \
#        -ceplen     13 \
        -topn       8 \
        -beam       1e-80 \
        -nwbeam     1e-40 \
        -dictfn     $dictfn \
        -fdictfn    $fdictfn \
        -lmfn       $lmfile \
        -inspen     0.02 \
        -ctlfn      $ctlfn \
        -ctloffset  $ctloffset \
        -ctlcount   $ctlcount \
        -cepdir     $cepdir \
        -cepext     $cepext \
        -bptblsize  400000 \
        -matchsegfn    $matchfile.seg \
        -matchfn    $matchfile \
#       -outlatdir  $outlatdir \
#       -mllrctlfn  $MLLRctlfn \
        -agc        none \
        -varnorm    no \
#        -outlatdir   ./lattices \
            -cmn  current \
>& $logfile


#$PGM \
#	-logbase 1.0003 \
#	-beam 1e-90  -wbeam 1e-35 \
#        -mdef $mdef -dict $dictfn -fdict $fdictfn -lm $lmfile \
#        -hypseg $matchfile \
#        -mean $modeldir/means -var $modeldir/variances \
#        -mixw $modeldir/mixture_weights  -tmat $modeldir/transition_matrices \
#	-subvq $quantizedmodels \
#        -ctl $ctlfn -ctloffset $ctloffset -ctlcount $ctlcount -cepdir $cepdir \
#        -agc none -varnorm no \
#	-cepext $cepext \
#	-lw 9.5 \
#>& $logfile

docleanup:
set savedstatus=$status:q

onintr
echo '================' >> $logfile
echo "exited with status $savedstatus" >> $logfile

exit $savedstatus                                  

exit 0
