#!/bin/csh -f

if ($#argv < 1 || $#argv > 2) then
    echo "Usage: $0 <no of states per hmm> (allow skips yes/no)"
    echo "       Default: allow skips yes"
    exit 0
endif
set nstates = $1
if ( $#argv != 2 ) then
  set allowskips = yes
else 
  set allowskips = $2
endif

echo "#"
echo "# ${nstates}-state Bakis topology HMM with non-emitting last state"
echo "# These values are normalized so that rows sum to one."
echo "#"
echo "# NO COMMENTS BETWEEN # OF STATES AND TRANSITION MATRIX"
echo "#"
echo "#"
echo "#Version number"
echo "0.1"
echo "# Number of states per model followed by transition matrix"

@ no = $nstates + 1
echo $no

set n = 1
while ($n <= $nstates)
    set j = 1
    if ( $allowskips == no ) then 
@     nnn = $n + 1
    else 
@     nnn = $n + 2
    endif

    while ($j <= $no)
        if ($j < $n || $j > $nnn) then
            echo -n "0.0	"
@	    j = $j + 1
        else
            echo -n "1.0	"
@	    j = $j + 1
        endif
    end
    echo ""
@   n = $n + 1
end
echo "# Last state has no outgoing arcs unless"
echo "# embedded in a sentence hmm structure"
