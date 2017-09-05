#! /bin/csh -f

source ../variables.def

#*************************************************************************
# This script runs the build_tree script for each state of each basephone
#*************************************************************************

set scriptdir = $base_dir/c_scripts/03.buildtrees
set qmgrdir = $base_dir/qmanager
set logdir = $base_dir/logdir/03.buildtrees
if ( ! -e $logdir ) mkdir -p $logdir
/bin/rm -rf $logdir
/bin/rm -rf $qmgrdir
mkdir -p $logdir
mkdir -p $qmgrdir

echo "Generating linguistic questions... wait..."
$scriptdir/make_questions.csh
echo "Linguistic questions generated..."
#echo "Linguistic questions generation manually bypassed..."

foreach phone (`cat $phonefile`)
    set state = 0
    while ( $state < $statesperhmm )
        set errfile = $qmgrdir/buildtree.$phone.$state.err
        set outfile = $qmgrdir/buildtree.$phone.$state.out
        $scriptdir/build_tree.csh $phone $state
echo "Tree for $phone $state constructed..."
        @   state = $state + 1
    end
end

exit 0
