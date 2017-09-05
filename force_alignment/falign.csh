#!/bin/csh -f

unlimit
limit core 0k

set exptname = myfalign

set base_dir = ../training
set featfiles_dir = ../features

set transcriptfile = $base_dir/lists/train.transcripts.forFalignment
set listoffiles = $base_dir/lists/train.ctl
set dictionary = $base_dir/lists/train.dict.forFalignment
set fillerdict = $base_dir/lists/fillerdict.forFalignment


set hmmdir    = $base_dir/model_parameters/MODELSET1.ci_continuous
set cepdir    = $featfiles_dir
#set segdir    = REF_PHSEGS_NEW
set mdeffile  = $base_dir/model_architecture/MODELSET1.ci.mdef

#machine type
set mach = linux

#************************************************************************
if ($#argv < 2) then
    echo "Usage: $0 part n_part"
    exit
endif
set echo
limit coredumpsize 0
set part   		= $1
set n_part 		= $2

set partialresults 	= $base_dir/falignout
if ( ! -e $partialresults ) mkdir -p $partialresults
set newtranscripts 	= $partialresults/$exptname.${part}.newtranscripts

set logdir              = $base_dir/falignout/log
if ( ! -e $logdir ) mkdir -p $logdir
set logfile   		= $logdir/$exptname.timealign.${part}.log

#-------------------------------------------------------------------------
# From part and n_parts figure out the first file in the listoffilesxs
# that has to be processed, and the number of files that has to be
# processed
#-------------------------------------------------------------------------
set total = `wc -l $listoffiles | awk '{print $1}'`

@ run = $total / $n_part + 1
echo $run
@ part --
@ offset = $part * $run
@ part ++

#set S3ALIGN = ~/09..sphinx3code/trainer/bin.alpha/s3align
####set S3ALIGN = ~rsingh/09..sphinx3code/s3decoder/bin/linux/s3align.fulldirname
#set S3ALIGN =  /net/alf30/usr3/bhiksha/work/sphinx-3/timealign.fulldir/bin/$mach/timealign
#set S3ALIGN =  /net/alf30/usr3/bhiksha/work/sphinx-3/decoder/bin/alpha/s3align
set S3ALIGN =  ../decoding/s3decoder/bin/linux/s3align


$S3ALIGN \
	-outsentfn  	$newtranscripts \
	-insentfn   	$transcriptfile \
	-ctlfn       	$listoffiles \
	-ctloffset   	$offset \
	-ctlcount    	$run \
	-cepdir      	$cepdir \
	-cepext	        mfc \
	-dictfn      	$dictionary \
	-fdictfn     	$fillerdict \
	-mdeffn      	$mdeffile \
        -senmgaufn      .cont. \
	-meanfn 	$hmmdir/means \
	-varfn  	$hmmdir/variances \
	-mixwfn 	$hmmdir/mixture_weights \
	-tmatfn 	$hmmdir/transition_matrices \
	-topn             1 \
#        -feat           s2_4x \
        -feat           s3_1x39 \
#        -feat           1s_c_d_dd \
	-beam 		1e-80 \
#	 -phsegdir      $segdir,CTL \
#	 -stsegdir      $segdir,CTL \
	-agc 		none \
	-cmn 		current \
>&!       $logfile

exit 0
