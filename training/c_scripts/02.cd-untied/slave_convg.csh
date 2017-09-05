#!/bin/csh -f

source ../variables.def

#***************************************************************************
# This script launches all the cd-continuous untied training jobs.
#***************************************************************************

if ($#argv > 1) then
    echo "USAGE: $0 <iteration number (def 1)>"
    exit 0
endif

if ($#argv == 0) then
    echo "USAGE: $0 <iteration number (def 1)>"
    echo "Setting iter value to 1"
    set iter = 1
else
    set iter = $1
endif

set scriptdir = $base_dir/c_scripts/02.cd-untied
set qmgrdir   = $base_dir/qmanager
if (! -e $qmgrdir) mkdir -p $qmgrdir
set logdir = $base_dir/logdir/02.cd_untied
if ( ! -e $logdir ) mkdir -p $logdir        
set bwaccumdir = $base_dir/bwaccumdir
set modeldir  = $base_dir/model_parameters

#Read npart_untied from variables.def

# Clean up junk from earlier runs
echo "Cleaning up accumulator directories..."
/bin/rm  -f $bwaccumdir/${exptname}_buff_?/* $bwaccumdir/${exptname}_buff_??/*

if ($iter == 1) then
    echo "Cleaning up log directories..."
    /bin/rm -rf $logdir
    mkdir -p $logdir
    echo "Cleaning up qmanager directories..."
    /bin/rm -rf $qmgrdir
    mkdir -p $qmgrdir
endif


   set errfn = $qmgrdir/$exptname.initialize_cd_untied.err
    set outfn = $qmgrdir/$exptname.initialize_cd_untied.out



if ( $iter == 1 ) then
$scriptdir/initialize_untied.csh
echo "initialization done..."
endif

foreach part (`$bindir/interval 1 $npart`)
$scriptdir/baum_welch.csh $iter $part $npart
echo baum_welch $iter $part done...
end
echo baum_welch $iter done...
echo launching norm for $iter...
$scriptdir/norm_and_launchbw.csh $iter


exit 0

