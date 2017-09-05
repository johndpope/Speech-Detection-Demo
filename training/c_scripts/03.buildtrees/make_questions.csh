#!/bin/csh -f

source ../variables.def
if ( ! -e $base_dir/tmp ) mkdir -p $base_dir/tmp

set scriptdir = $base_dir/c_scripts/03.buildtrees

#Change This
set mdeffn   = $base_dir/model_architecture/$exptname.ci.mdef
set hmm_dir  = $base_dir/model_parameters/${exptname}.ci_continuous
set meanfn   = $hmm_dir/means
set varfn    = $hmm_dir/variances
set mixwfn   = $hmm_dir/mixture_weights
set tempfn   = $base_dir/tmp/questions.$$
#set questfn  = $base_dir/model_architecture/${exptname}.tree_questions
set questfn  = $scriptdir/linguistic_questions.automatic

set logdir = $base_dir/logdir/03.buildtrees
if ( ! -e $logdir ) mkdir -p $logdir
set logfile = $logdir/${exptname}.make_questions.log

set MAKE_QUEST = $bindir/make_quests

$MAKE_QUEST -moddeffn  $mdeffn \
	    -meanfn    $meanfn \
	    -varfn     $varfn \
	    -mixwfn    $mixwfn \
            -npermute  12 \
            -niter     1  \
            -qstperstt 20 \
            -type      .cont. \
	    -tempfn    $tempfn \
	    -questfn   $questfn \
	>&! $logfile

exit 0
