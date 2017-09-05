#!/bin/csh -f

source ../variables.def
unlimit
limit core 0k

if ($#argv != 2) then
   echo "usage: $0 <ngau> <iter>"
   exit 1
endif

set ngau = $1
set iter = $2

set scriptdir = $base_dir/c_scripts/05.cd-chmm
set logdir = $base_dir/logdir/05.cd_chmm
if ( ! -e $logdir ) mkdir -p $logdir
set log            = $logdir/${exptname}.${ngau}gau.$iter.norm.log

set num_done = `grep "MLLR regmat" $logdir/${exptname}.${ngau}gau.${iter}-*.bw.log | wc -l | awk '{print $1}'`

echo "$num_done parts of $npart of Baum Welch were successfully completed" >! $log
if ($num_done != $npart) then
    echo "Some of the baum-welch jobs seem to have bombed out." >> $log
    echo "Aborting\!\!\!" >> $log
    exit 0
endif

$scriptdir/norm.csh $ngau $iter

set like = `grep "overall>" $logdir/${exptname}.${ngau}gau.${iter}-*.bw.log | awk '{X += $3;Y += $6} END {print Y/X}'`
echo "Current Overall Likelihood Per Frame = $like" >> $log

if ($iter == 1) goto launch_bw # Always perform a second iteration

@ previter = $iter - 1
set prevlike = `grep "overall>" $logdir/${exptname}.${ngau}gau.${previter}-*.bw.log | awk '{ X += $3; Y += $6} END {print Y/X}'`

if ($prevlike == "") goto launch_bw # we seem to be starting from an
				    # intermediate iter value

#hack to handle sign
set absprev = `echo "$prevlike"|awk '$1 < 0 {print -$1} $1 > 0 {print $1}'`
echo "$prevlike $like $absprev"|awk '{printf("Convergence Ratio = %f\n",($2-$1)/$3)}' >> $log
set testval = `echo "$prevlike $like $absprev"|awk -v th=$convergence_ratio '{cvr=($2-$1)/$3; if (cvr > th) {print 1;} else {print 0;}}'`

if ($testval == 1) goto launch_bw

# if testval != 1, the likelihoods have converged. 
# Either split gaussians (if more are required) or end training

if ($ngau < $gaussiansperstate) then
    echo "*** Baum Welch converged for $ngau Gaussians/State. Splitting.. ***" >> $log
@   tmp  = $gaussiansperstate - $ngau
    if ($tmp >= $ngau) then
        set numtosplit = $ngau
    else
        set numtosplit = $tmp
    endif

    $scriptdir/convert2ascii.csh $ngau >> $log
    $scriptdir/split_gaussians.csh ${numtosplit}
@   ngau = $ngau + $numtosplit
    set iter = 0
else
    # No further jobs needed
    echo "The likelihoods have converged. Baum Welch training completed\!" >> $log
    echo "Desired number of Gaussians/state obtained"
    echo "*********************************TRAINING COMPLETE***************************" >> $log
    date >> $log
    exit 0
endif

launch_bw:
if ( $iter == $maxiter ) then
    if ($ngau >= $gaussiansperstate) then
        echo "Maximum iterations limit reached." >> $log
        echo "Desired number of Gaussians/state obtained"
        echo "*********************************TRAINING COMPLETE***************************" >> $log
        date >> $log
	$scriptdir/convert2ascii.csh $ngau >> $log
	exit 0
    endif
    echo "Maximum iterations limit reached. Splitting..." >> $log
@   tmp  = $gaussiansperstate - $ngau
    if ($tmp >= $ngau) then
        set numtosplit = $ngau
    else
        set numtosplit = $tmp
    endif
    $scriptdir/convert2ascii.csh $ngau >> $log
    $scriptdir/split_gaussians.csh ${numtosplit}
@   ngau = $ngau + $numtosplit
    set iter = 0
endif


@ newiter = $iter + 1

$scriptdir/slave_convg.csh $ngau $newiter

exit 0
