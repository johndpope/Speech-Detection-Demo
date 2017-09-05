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

if ($#argv != 4) then
   echo "usage: $0 <ngau> <iter> <part> <npart>"
   exit 1
endif

set ngau   = $1
set iter   = $2
set part   = $3
set npart  = $4

set BW = $bindir/bw.mop64
#changethis
set output_buffer_dir = $base_dir/bwaccumdir/${exptname}_buff_${part}

if ( ! -e $output_buffer_dir ) mkdir -p $output_buffer_dir 

#read the train_mfc_dir, feat, veclen, cepext, agc and cmn values and n_seno
#from variables file

if ($iter == 1) then
    set hmm_dir           = $base_dir/model_parameters/${exptname}.cd_continuous_flatinitial
    set var2pass	  = no
else
    set hmm_dir           = $base_dir/model_parameters/${exptname}.cd_continuous
    set var2pass	  = yes
endif

set mdef_file = $base_dir/model_architecture/${exptname}.${n_tied_states}.mdef
set statepdeffn = .cont. # indicates that the HMMs are continuous

set mixwfn  = $hmm_dir/mixture_weights
set mwfloor = 1e-5     # Floor set on mixture weights 
set tpfloor = 1e-5     # Floor set on transition probabilities
set tmatfn  = $hmm_dir/transition_matrices
set meanfn  = $hmm_dir/means
set varfn   = $hmm_dir/variances
set minvar  = 1e-4     # Floor set on variances


#$dictionary and $fillerdict obtained from variables.def

if ($iter == 1) then
    set topn  = 99
else
    set topn  = 16
endif

set logdir            = $base_dir/logdir/05.cd_chmm
if ( ! -e $logdir ) mkdir -p $logdir          
set logfile        = $logdir/${exptname}.${ngau}gau.$iter-$part.bw.log

hostname >! $logfile

$BW \
	-moddeffn 	$mdef_file \
	-ts2cbfn 	$statepdeffn \
	-mixwfn 	$mixwfn \
	-mwfloor 	$mwfloor \
	-tmatfn 	$tmatfn \
        -tpfloor        $tpfloor \
	-meanfn 	$meanfn \
	-varfn 		$varfn \
	-dictfn 	$dictionary \
	-fdictfn 	$fillerdict \
	-ctlfn 		$listoffiles \
	-part 		$part \
	-npart 		$npart \
	-cepdir 	$featfiles_dir \
	-cepext 	$featfile_extension \
	-lsnfn 		$transcriptfile \
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

exit 0
