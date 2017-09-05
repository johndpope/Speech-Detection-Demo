#!/bin/csh -f

if ($#argv != 1) then
    echo "USAGE: $0 ctlfile"
    exit 0
endif

set ctlfile = $1

#set indir = /usr2/databases/
set indir = /Users/samemon/desktop/DemoSpeech-master-advanced
set outdir = ./features

set w2feat =  ./tools/mfcc_computation_routines/wave2feat

set tmpfile = tmp.$$
onintr cleanup

foreach f (`cat $ctlfile`)
    echo "working on $f..."
  set dir = $f:h
  if (! -e $outdir/$dir ) mkdir -p $outdir/$dir

    set infile = $indir/$f.wav
    set infile1 = $indir/$f.flac
    echo $infile
    set outfile = $outdir/${f}.80-7200_40filts.mfc
if( -e $infile && ! -e $outfile ) then
    #sox $infile $infile1
    sox -t .wav $infile -t .sph $tmpfile
        $w2feat -i $tmpfile  \
            -o $outfile  \
            -srate 16000 \
            -frate 100   \
            -lowerf 80  \
            -upperf 7200 \
            -dither      \
            -nfilt 40    \
            -nist
        echo "done"
endif
/bin/rm -f ${tmpfile}
end

cleanup:
/bin/rm -f ${tmpfile}

exit 0

            -nist
