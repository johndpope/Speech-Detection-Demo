#!/bin/csh -f

source ../variables.def

#**************************************************************************
# SCRIPT TO INCREMENT THE NUMBER OF GAUSSIANS PER STATE IN A MODEL BY ONE
# TAKES THE RETRAINED mixw mean AND var FILES, SPLITS THE HEAVIEST GAUSSIAN
# (THE ONE WITH THE LARGEST mixw) AND STORES THE RESULT IN THE INITIAL
# MODEL DIRECTORY FOR THE NEXT BUNCH OF BAUM-WELCH ITERATIONS
#**************************************************************************

if ($#argv != 1) then
   echo "usage: $0 <num to split>"
   exit 1
endif 


set num_to_split = $1

set inmodeldir    = $base_dir/model_parameters/${exptname}.cd_continuous
set outmodeldir   = $base_dir/model_parameters/${exptname}.cd_continuous_flatinitial

set backmodeldir  = $base_dir/model_parameters/${exptname}.cd_continuous_${num_to_split}gau
if ( ! -e $backmodeldir ) mkdir $backmodeldir

cp -f $inmodeldir/* $backmodeldir/
cp -f $inmodeldir/transition_matrices $outmodeldir/

set logdir	  = $base_dir/logdir/05.cd_chmm
if ( ! -e $logdir ) mkdir -p $logdir

set inmixwfn      = $inmodeldir/mixture_weights
set inmeanfn      = $inmodeldir/means
set invarfn       = $inmodeldir/variances

set outmixwfn     = $outmodeldir/mixture_weights
set outmeanfn     = $outmodeldir/means
set outvarfn      = $outmodeldir/variances

set logfile = $logdir/${exptname}.split_gaussians.log
touch $logfile
echo "SPLITTING GAUSSIANS" >> $logfile
echo "" >> $logfile


set obj = $bindir/inc_comp

$obj \
    -ninc      $num_to_split \
    -dcountfn  $inmixwfn \
    -inmixwfn  $inmixwfn \
    -outmixwfn $outmixwfn \
    -inmeanfn  $inmeanfn \
    -outmeanfn $outmeanfn \
    -invarfn   $invarfn \
    -outvarfn  $outvarfn \
    -feat      $feature \
    -ceplen    $vector_length \
>&! $logfile

echo "DONE SPLITTING" >> $logfile
date >> $logfile

exit 0
