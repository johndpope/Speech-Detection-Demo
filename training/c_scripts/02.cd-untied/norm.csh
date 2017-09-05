#!/bin/csh -f

source ../variables.def
unlimit
limit core 0k

if ($#argv != 1) then
   echo "usage: $0 <iter>"
   exit 1
endif

set iter = $1

set scriptdir = $base_dir/c_scripts/02.cd-untied
cd  $scriptdir

set NORM   = $bindir/norm.mop64

# cover up to 100 different buffer directories....
set buffer_dirs = ($base_dir/bwaccumdir/${exptname}_buff_? $base_dir/bwaccumdir/${exptname}_buff_??)

if ($type == ".semi.") then
   set modeltype = semi
else
   set modeltype = continuous
endif

#set hmm_dir   = $base_dir/model_parameters/${exptname}.cd_untied
set hmm_dir   = $base_dir/model_parameters/${exptname}.cd_${modeltype}_untied
if ( ! -e $hmm_dir ) mkdir -p $hmm_dir

#new models to be produced after normalization
set mixwfn         = $hmm_dir/mixture_weights
set tmatfn         = $hmm_dir/transition_matrices
set meanfn         = $hmm_dir/means
set varfn          = $hmm_dir/variances


set logdir = $base_dir/logdir/02.cd_untied
if ( ! -e $logdir ) mkdir -p $logdir        
set log            = $logdir/${exptname}.$iter.norm.log



$NORM \
	-accumdir   $buffer_dirs \
	-mixwfn     $mixwfn \
	-tmatfn     $tmatfn \
	-meanfn     $meanfn \
	-varfn      $varfn \
#        -ceplen     $vector_length \
#        -feat       $feature \
	>&! $log
date >> $log

exit 0
