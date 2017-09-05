#!/bin/csh

source ../variables.def
unlimit
limit core 0k

#**************************************************************************
# This script reads off the leaves of the pruned trees and creates an mdef
# file
#**************************************************************************

if ($#argv != 1) then
    echo "usage: $0 <n-state>"
    exit 1
endif

set n_tied_states = $1

set  untied_mdef = $base_dir/model_architecture/${exptname}.alltriphones.mdef
set logdir = $base_dir/logdir/04.tiestate
if (! -e $logdir) mkdir -p $logdir

set MAKE_MDEF = $bindir/mk_mdef_gen

$MAKE_MDEF \
        -phnlstfn       $phonefile  \
        -dictfn         $dictionary \
        -oalltphnmdef    $untied_mdef \
        -n_state_pm     $statesperhmm \
    >&! $logdir/${exptname}.make_alltriphones_mdef.log 

set question_set   = ../03.buildtrees/linguistic_questions.automatic

set prunedtreedir  = $base_dir/trees/${exptname}.$n_tied_states
set tied_mdef_file = $base_dir/model_architecture/${exptname}.${n_tied_states}.mdef

set logfile = $logdir/${exptname}.tiestate.${n_tied_states}.log
set TIESTATE = $bindir/tiestate

$TIESTATE \
    -imoddeffn $untied_mdef \
    -omoddeffn $tied_mdef_file   \
    -treedir   $prunedtreedir    \
    -psetfn    $question_set     \
>&! $logfile

exit 0




