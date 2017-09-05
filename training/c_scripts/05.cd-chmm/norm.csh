#!/bin/csh -f

source ../variables.def
unlimit
limit core 0k

#************************************************************************

if ($#argv != 2) then
   echo "usage: $0 <ngau> <iter>"
   exit 1
endif

set ngau = $1
set iter = $2

# cover up to 100 different buffer directories....
set buffer_dirs = ($base_dir/bwaccumdir/${exptname}_buff_? $base_dir/bwaccumdir/${exptname}_buff_?? $base_dir/bwaccumdir/${exptname}_buff_???)

set hmm_dir           = $base_dir/model_parameters/${exptname}.cd_continuous
set finhmm_dir = ${hmm_dir}_${ngau}gau
if ( ! -e $hmm_dir ) mkdir -p $hmm_dir
if ( ! -e $finhmm_dir ) mkdir -p $finhmm_dir

#new models to be produced after normalization
set mixwfn         = $hmm_dir/mixture_weights
set tmatfn         = $hmm_dir/transition_matrices
set meanfn         = $hmm_dir/means
set varfn          = $hmm_dir/variances

set logdir            = $base_dir/logdir/05.cd_chmm
if ( ! -e $logdir ) mkdir -p $logdir          
set log            = $logdir/${exptname}.${ngau}gau.$iter.norm.log

hostname >! $log

set mach = linux

#CHANGETHIS
#set NORM   = YOURTRAINER/bin/norm.mop64
set NORM = $bindir/norm.mop64

$NORM \
	-accumdir   $buffer_dirs \
	-mixwfn     $mixwfn \
	-tmatfn     $tmatfn \
	-meanfn     $meanfn \
	-varfn      $varfn \
#        -ceplen     $vector_length \
#        -feat       $feature \
	>>& $log

date >> $log

cp -p ${hmm_dir}/means ${finhmm_dir}/means
cp -p ${hmm_dir}/variances ${finhmm_dir}/variances
cp -p ${hmm_dir}/transition_matrices ${finhmm_dir}/transition_matrices
cp -p ${hmm_dir}/mixture_weights ${finhmm_dir}/mixture_weights

exit 0
