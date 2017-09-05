#!/bin/csh

source ../variables.def
unlimit
limit core 0k
#*************************************************************************
# The script builds a tree for ONE state of ONE basephone. We therefore
# need to call it in a loop to build trees for all the states of all the
# basephones
#*************************************************************************

if ($#argv != 2) then
    echo "usage: $0 <phone> <state>"
    exit -1
endif

set phone = $1
set state = $2

set BUILDTREE = $bindir/bldtree

set scriptdir = $base_dir/c_scripts/03.buildtrees
set mdef_file = $base_dir/model_architecture/${exptname}.untied.mdef 

#Long complicated bit of nonsense to set state weights for different
#numbers of states/hmm
if ($statesperhmm == 5) then
    set stwt = "1.0 0.2 0.05 0.0 0.0"
else if ($statesperhmm == 9) then
    set stwt = "0.0 0.0 0.1 0.2 0.5 0.0 0.0 0.0 0.0"
else if ($statesperhmm == 8) then
    set stwt = "0.0 0.1 0.2 0.5 0.2 0.0 0.0 0.0"
else if ($statesperhmm == 7) then
    set stwt = "0.0 0.1 0.2 0.5 0.0 0.0 0.0"
else if ($statesperhmm == 6) then
    set stwt = "0.1 0.2 0.5 0.2 0.0 0.0"
else if ($statesperhmm == 4) then
    set stwt = "1.0 0.1 0.0 0.0"
else if ($statesperhmm == 3) then
    set stwt = "1.0 0.05 0.0"
else if ($statesperhmm == 2) then
    set stwt = "1.0 0.025"
else if ($statesperhmm == 1) then
    set stwt = 1
else
    echo "$statesperhmm states per hmm not implemented in buildtree script"
    echo "Fix the script and run it"
    exit 0
endif

set mixture_wt_file   = $base_dir/model_parameters/${exptname}.cd_continuous_untied/mixture_weights
set means   = $base_dir/model_parameters/${exptname}.cd_continuous_untied/means
set variances   = $base_dir/model_parameters/${exptname}.cd_continuous_untied/variances
#set question_set   = $scriptdir/linguistic_questions
set question_set   = $scriptdir/linguistic_questions.automatic
set unprunedtreedir  = $base_dir/trees/${exptname}.unpruned
if ( ! -e $unprunedtreedir ) mkdir -p $unprunedtreedir

set logdir = $base_dir/logdir/03.buildtrees
if ( ! -e $logdir ) mkdir -p $logdir
set logfile = $logdir/${exptname}.buildtree.${phone}.${state}.log


$BUILDTREE \
	-treefn   $unprunedtreedir/$phone-$state.dtree \
	-moddeffn $mdef_file \
	-mixwfn   $mixture_wt_file \
        -ts2cbfn  .cont. \
        -meanfn   $means \
        -varfn    $variances \
	-mwfloor  1e-8 \
	-psetfn   $question_set \
	-phone    $phone \
	-state    $state \
	-stwt     $stwt \
	-ssplitmin 1 \
	-ssplitmax 7 \
	-ssplitthr 0 \
	-csplitmin 1 \
	-csplitmax 2000 \
	-csplitthr 0 \
	-cntthresh 1 \
>&! $logfile

exit 0

