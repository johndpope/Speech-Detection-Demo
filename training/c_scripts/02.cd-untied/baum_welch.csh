#!/bin/csh -f

source ../variables.def
unlimit
limit core 0k

#************************************************************************
# this script performs baum-welch training using s3 code for a 
# continuous mdef file.
# it needs as inputs an initial set of semicont models in s3 format
# a mdef file and cepstra with transcription files.
#************************************************************************

if ($#argv != 3) then
   echo "usage: $0 <iter> <part> <npart>"
   exit 1
endif

set iter   = $1
set part   = $2
set npart  = $3

set scriptdir = $base_dir/c_scripts/02.cd-untied
cd  $scriptdir

set BW = $bindir/bw.mop64

set output_buffer_dir = $base_dir/bwaccumdir/${exptname}_buff_${part}


if ( ! -e $output_buffer_dir ) mkdir -p $output_buffer_dir 

if ($type == ".semi.") then
   set modeltype = semi
else
   set modeltype = continuous
endif

#set hmm_dir  = $base_dir/model_parameters/${exptname}.cd_untied
set hmm_dir  = $base_dir/model_parameters/${exptname}.cd_${modeltype}_untied
set var2pass	  = yes

set moddeffn    = $base_dir/model_architecture/$exptname.untied.mdef
set statepdeffn = $type

set mixwfn  = $hmm_dir/mixture_weights
set mwfloor = 1e-8
set tmatfn  = $hmm_dir/transition_matrices
set meanfn  = $hmm_dir/means
set varfn   = $hmm_dir/variances
set minvar  = 1e-4

#$dictionary from variables.def
#$fillerdict from variables.def

set topn           = 20

set logdir = $base_dir/logdir/02.cd_untied
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
	-abeam 		1e-90 \
	-bbeam 		1e-10 \
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

