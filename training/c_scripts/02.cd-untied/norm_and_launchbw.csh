#!/bin/csh -f

unlimit
limit core 0k
source ../variables.def
#*******************************************************************
#*******************************************************************

if ($#argv != 1) then
   echo "usage: $0 <iter>"
   exit 1
endif

set iter = $1

#CHANGETHIS
set scriptdir = $base_dir/c_scripts/02.cd-untied
cd  $scriptdir

set logdir = $base_dir/logdir/02.cd_untied
if ( ! -e $logdir ) mkdir -p $logdir
set log            = $logdir/${exptname}.$iter.norm.log

set num_done = `grep "Counts" $logdir/${exptname}.${iter}-*.bw.log | wc -l | awk '{print $1}'`

echo "$num_done parts of $npart of Baum Welch successfully completed" >! $log
if ($num_done != $npart) then
    echo "Some of the baum-welch jobs seem to have bombed out." >> $log
    echo "Aborting\!\!\!" >> $log
    exit 0
endif

$scriptdir/norm.csh $iter

set like = `grep "overall>" $logdir/${exptname}.${iter}-*.bw.log | awk '{X += $3;Y += $6} END {print Y/X}'`
echo "Current Overall Likelihood Per Frame = $like" >> $log

if ($iter == 1) goto launch_bw # Always perform a second iteration

@ previter = $iter - 1
set prevlike = `grep "overall>" $logdir/${exptname}.${previter}-*.bw.log | awk '{ X += $3; Y += $6} END {print Y/X}'`

if ($prevlike == "") goto launch_bw # we seem to be starting from an
				    # intermediate iter value

#hack to handle sign
set absprev = `echo "$prevlike"|awk '$1 < 0 {print -$1} $1 > 0 {print $1}'`
echo "$prevlike $like $absprev"|awk '{printf("Convergence Ratio = %f\n",($2-$1)/$3)}' >> $log
set testval = `echo "$prevlike $like $absprev"|awk -v th=$convergence_ratio '($2-$1)/$3 > th {print 1} ($2-$1)/$3 < th {print 0}'`

if ($testval == 1) goto launch_bw

# if testval != 1, the likelihoods have converged. No further jobs needed
echo "The likelihoods have converged. Baum Welch training completed\!" >> $log
echo "*********************************TRAINING COMPLETE***************************" >> $log
date >> $log
exit 0

launch_bw:
if ( $iter == $maxiter ) then
echo "Maximum iterations limit reached. Stopping" >> $log
exit 0
endif


@ newiter = $iter + 1

$scriptdir/slave_convg.csh $newiter

exit 0
