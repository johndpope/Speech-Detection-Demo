#!/bin/csh -f

source ../variables.def
unlimit
limit core 0k

#**************************************************************************
# This script assignes means and unit-variances to the gaussians that
# are associated with each state listed in the ci-mdef file.
#**************************************************************************

set scriptdir = $base_dir/c_scripts/02.cd-untied
cd  $scriptdir

set modarchdir    = $base_dir/model_architecture
set ci_mdeffile   = $modarchdir/$exptname.ci.mdef 
set cd_mdeffile   = $modarchdir/$exptname.untied.mdef 

# MAKE untied mdef file
set logdir = $base_dir/logdir/02.cd_untied
if ( ! -e $logdir ) mkdir -p $logdir                                       

set MAKE_MDEF = $bindir/mk_mdef_gen

$MAKE_MDEF \
        -phnlstfn     $phonefile  \
	-dictfn		$dictionary \
	-lsnfn		$transcriptfile \
        -ountiedmdef    $cd_mdeffile  \
	-n_state_pm	$statesperhmm \
    >&! $logdir/${exptname}.make_untied_mdef.log
#..

if ($type == ".semi.") then
   set modeltype = semi
else
   set modeltype = continuous
endif

set modeldir 	   = $base_dir/model_parameters
set cimodeldir     = $modeldir/${exptname}.ci_continuous
set cdmodeldir     = $modeldir/${exptname}.cd_untied

if ( ! -e $cdmodeldir  ) mkdir -p $cdmodeldir


set cd_hmmdir         = $base_dir/model_parameters/${exptname}.cd_continuous_untied
if ( ! -e $cd_hmmdir ) mkdir -p $cd_hmmdir

set logfile        = $logdir/${exptname}.initialize_untied_models_from_ci.log

$bindir/init_mixw \
        -src_moddeffn $ci_mdeffile \
	-src_ts2cbfn  $type \
        -src_mixwfn $cimodeldir/mixture_weights \
        -src_tmatfn $cimodeldir/transition_matrices \
        -src_meanfn $cimodeldir/means \
        -src_varfn  $cimodeldir/variances \
        -dest_moddeffn $cd_mdeffile \
	-dest_ts2cbfn  $type \
        -dest_mixwfn $cdmodeldir/mixture_weights \
        -dest_tmatfn $cdmodeldir/transition_matrices \
        -dest_meanfn $cdmodeldir/means \
        -dest_varfn  $cdmodeldir/variances \
        -feat 	     $feature \
        -ceplen      $vector_length \
>&! $logfile     

cp -r $cdmodeldir/* $cd_hmmdir
echo copied all models in $cdmodeldir/ to $cd_hmmdir/ ...


exit 0
