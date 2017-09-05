#!/bin/csh -f

source ../variables.def
limit core 0k

if ($#argv != 1) then
   echo "usage: $0 <ngau>"
   exit 1
endif

set ngau = $1

set finhmm_dir = $base_dir/model_parameters/${exptname}.cd_continuous_${ngau}gau

$bindir/printp -gaufn ${finhmm_dir}/means >! ${finhmm_dir}/means.ascii
$bindir/printp -gaufn ${finhmm_dir}/variances >! ${finhmm_dir}/variances.ascii
$bindir/printp -tmatfn ${finhmm_dir}/transition_matrices >! ${finhmm_dir}/transition_matrices.ascii
$bindir/printp -mixwfn ${finhmm_dir}/mixture_weights >! ${finhmm_dir}/mixture_weights.ascii

echo "convert2ascii.csh $ngau $finhmm_dir"

exit 0