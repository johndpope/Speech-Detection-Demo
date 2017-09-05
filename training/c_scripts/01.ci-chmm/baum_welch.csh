#!/bin/csh -f

source ../variables.def
unlimit
limit core 0k

#************************************************************************
# this script performs baum-welch training for continuous CI models
# it needs as inputs an initial set of continuous models in s3 format
# a CI model defiition (mdef) file and cepstra with transcription files.
#************************************************************************

if ($#argv != 3) then
   echo "usage: $0 <iter> <part> <npart>"
   exit 1
endif

set iter   = $1
set part   = $2
set npart  = $3

set scriptdir = $base_dir/c_scripts/01.ci-chmm
cd  $scriptdir

set BW = $bindir/bw.mop64
set output_buffer_dir = $base_dir/bwaccumdir/${exptname}_buff_${part}

if ( ! -e $output_buffer_dir ) mkdir -p $output_buffer_dir 

if ($type == ".semi.") then
   set modeltype = semi
else
   set modeltype = continuous
endif

if ($iter == 1) then
    set hmm_dir  = $base_dir/model_parameters/${exptname}.ci_${modeltype}_flatinitial
    set var2pass	 = no
else
    set hmm_dir           = $base_dir/model_parameters/${exptname}.ci_${modeltype}
    set var2pass	  = yes
endif

set moddeffn    = $base_dir/model_architecture/$exptname.ci.mdef
set statepdeffn = $type

set mixwfn  = $hmm_dir/mixture_weights
set mwfloor = 1e-8
set tmatfn  = $hmm_dir/transition_matrices
set meanfn  = $hmm_dir/means
set varfn   = $hmm_dir/variances
set minvar  = 1e-4

set topn    = 20

set logdir = $base_dir/logdir/01.ci_chmm
if ( ! -e $logdir ) mkdir -p $logdir                                       
set logfile            = $logdir/${exptname}.$iter-$part.bw.log
hostname >! $logfile

$BW \
	-moddeffn 	$moddeffn \
	-ts2cbfn 	$statepdeffn \
	-mixwfn 	$mixwfn \
	-mwfloor 	$mwfloor \
	-tmatfn 	$tmatfn \
	-meanfn 	$meanfn \
	-varfn 		$varfn \
	-dictfn 	$dictionary \
	-fdictfn 	$fillerdict \
	-ctlfn 		$listoffiles \
	-part 		$part \
	-npart 		$npart \
	-cepdir 	$featfiles_dir \
	-cepext 	$featfile_extension \
	-lsnfn 		$transcriptfile\
	-accumdir 	$output_buffer_dir \
	-varfloor 	$minvar \
	-topn 		$topn \
	-abeam 		1e-100 \
	-bbeam 		1e-30 \
	-agc 		$agc \
	-cmn 		$cmn \
	-varnorm	$varnorm \
        -meanreest 	yes \
        -varreest 	yes \
	-2passvar 	$var2pass \
	-tmatreest 	yes \
	-feat 		$feature \
        -ceplen 	$vector_length \
>>& $logfile

date >> $logfile



