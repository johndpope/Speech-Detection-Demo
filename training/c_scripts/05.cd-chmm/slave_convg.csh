#!/bin/csh -f

source ../variables.def

#***************************************************************************
# This script launches all the cd-tied training jobs in the proper
# order.
#***************************************************************************

#if ($#argv != 2) then
#    echo "USAGE: $0 <ngau> <iteration number (def 1)>"
#    exit 0
#endif

if ($#argv == 0) then
    echo "USAGE: $0 <ngau (def 1)> <iteration number (def 1)>"
    echo "Setting ngau to 1, iteration no. to 1"
    echo -n "Continue? (y/n) : "
    set ans = $<
    if ($ans != "y") exit 0
    set ngau = 1
    set iter = 1
else
    set ngau = $1
    set iter = $2
endif
echo "initial models are $ngau gaussian per state"
echo "Setting iter value to $iter"

set scriptdir = $base_dir/c_scripts/05.cd-chmm
set qmgrdir   = $base_dir/qmanager
if (! -e $qmgrdir) mkdir -p $qmgrdir
set logdir = $base_dir/logdir/05.cd_chmm
if ( ! -e $logdir ) mkdir -p $logdir        
set bwaccumdir = $base_dir/bwaccumdir
set modeldir  = $base_dir/model_parameters

#Read npart from variables.def

# Clean up junk from earlier runs
echo "Cleaning up accumulator directories..."
/bin/rm  -f $bwaccumdir/${exptname}_buff_*/*

if ($iter == 1 && $ngau == 1) then
    echo "Cleaning up log directories..."
    /bin/rm -rf $logdir
    mkdir -p $logdir
    echo "Cleaning up qmanager directories..."
    /bin/rm -rf $qmgrdir
    mkdir -p $qmgrdir
endif

if ($iter == 1 && $ngau == 1) then
    # For the first iteration Flat initialize models.
    set errfn = $qmgrdir/$exptname.initialize_cdtied.err
    set outfn = $qmgrdir/$exptname.initialize_cdtied.out
endif



if ( $iter == 1 && $ngau == 1 ) then
$scriptdir/initialize.cd.csh
echo "initialization done..."
endif

foreach part (`$bindir/interval 1 $npart`)
$scriptdir/baum_welch.csh $ngau $iter $part $npart
echo baum_welch $ngau $iter $part done...
end
echo baum_welch $iter done...
echo launching norm for iteration no. $iter for $ngau gaussian/state models...
$scriptdir/norm_and_launchbw.csh $ngau $iter


exit 0

