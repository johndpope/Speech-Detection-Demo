#!/bin/csh

source ../variables.def
unlimit
limit core 0k

#*************************************************************************
# This script prunes the trees computed earlier to have the desired number
# of leaves. Each leaf corresponds to one tied state
#*************************************************************************


if ($#argv != 1) then
    echo "usage: $0 <n-state>"
    exit 0
endif

set n_tied_states  = $1
set occurance_threshold = 0

set mdef_file = $base_dir/model_architecture/${exptname}.untied.mdef 
set question_set   = ../03.buildtrees/linguistic_questions.automatic

set unprunedtreedir  = $base_dir/trees/${exptname}.unpruned

set prunedtreedir  = $base_dir/trees/${exptname}.$n_tied_states
if ( ! -e $prunedtreedir ) mkdir -p $prunedtreedir

set PRUNETREE = $bindir/prunetree

set logdir = $base_dir/logdir/04.tiestate
if ( ! -e $logdir ) mkdir -p $logdir
set logfile = $logdir/${exptname}.prunetree.${n_tied_states}.log

$PRUNETREE \
	-itreedir $unprunedtreedir     \
	-nseno    $n_tied_states       \
	-otreedir $prunedtreedir       \
	-moddeffn $mdef_file           \
	-psetfn   $question_set        \
	-minocc   $occurance_threshold \
>&! $logfile

exit 0
