#!/bin/csh -f

./tools/dict-find-linux compactphones.dict wordlist found.dict
gawk '1==1 {print $1;}' found.dict | sed -e 's+(.)++g' -e 's+(..)++g' | sort -u >! yy
diff wordlist yy | grep "<" | sed -e 's+\<++g' >! for_manual.dict

exit 0
