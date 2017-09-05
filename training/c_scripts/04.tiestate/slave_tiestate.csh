#!/bin/csh -f

source ../variables.def

set scriptdir = $base_dir/c_scripts/04.tiestate

echo "$scriptdir/prunetree.csh  $n_tied_states"
$scriptdir/prunetree.csh  $n_tied_states
echo "Prune Tree Done"
echo "$scriptdir/tiestate.csh $n_tied_states"
$scriptdir/tiestate.csh $n_tied_states
echo "Tie state Done"

exit 0




