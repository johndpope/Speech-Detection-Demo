#!/bin/csh -f

source ../variables.def

#**************************************************************************
# this script copies the mixw/mean/var/tmat from a ci (continuous) HMM
# to a cd (continuous) HMM. We need two mdef files, one without triphones
# for the ci hmm definition and another one with triphones for the cd
# triphone definition.
# our final goal is to train in stages, first the ci models and then
# with cd models. This should minimize initialization problems..
#**************************************************************************X4F
 
set ci_hmmdir         = $base_dir/model_parameters/${exptname}.ci_continuous
set src_moddeffn      = $base_dir/model_architecture/${exptname}.ci.mdef
set src_mixwfn        = $ci_hmmdir/mixture_weights
set src_meanfn        = $ci_hmmdir/means
set src_varfn         = $ci_hmmdir/variances
set src_tmatfn        = $ci_hmmdir/transition_matrices

set cd_hmmdir         = $base_dir/model_parameters/${exptname}.cd_continuous_flatinitial
if ( ! -e $cd_hmmdir ) mkdir -p $cd_hmmdir

set dest_moddeffn     = $base_dir/model_architecture/${exptname}.${n_tied_states}.mdef
set dest_mixwfn       = $cd_hmmdir/mixture_weights
set dest_meanfn       = $cd_hmmdir/means
set dest_varfn        = $cd_hmmdir/variances
set dest_tmatfn       = $cd_hmmdir/transition_matrices

set logdir            = $base_dir/logdir/05.cd_chmm
if ( ! -e $logdir ) mkdir -p $logdir
set logfile           = $logdir/${exptname}.copy.ci.2.cd.log

set mach = linux

set COPY = $bindir/init_mixw

$COPY \
    -src_moddeffn $src_moddeffn \
    -src_ts2cbfn  .cont. \
    -src_mixwfn   $src_mixwfn  \
    -src_meanfn   $src_meanfn  \
    -src_varfn    $src_varfn   \
    -src_tmatfn   $src_tmatfn  \
    -dest_moddeffn $dest_moddeffn \
    -dest_ts2cbfn  .cont. \
    -dest_mixwfn  $dest_mixwfn  \
    -dest_meanfn  $dest_meanfn  \
    -dest_varfn   $dest_varfn   \
    -dest_tmatfn  $dest_tmatfn  \
    -feat         $feature \
    -ceplen       $vector_length \
    >&!  $logfile

exit 0
