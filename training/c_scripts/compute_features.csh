#! /bin/csh -f

if ($#argv != 1) then
    echo "USAGE: $0 <wavefile_list>"
    exit 0
endif

set list = $1

foreach file (`cat $list`)  
    set infile = ENTERPATH/$file.wav
    set outfile = ENTERPATH/features/$file.mfc
     
    set logfile = ./LOG
    /bin/rm -f LOG; touch LOG
	

    if ( -e $infile ) then
/home/rsingh/mfcc_computation_routines/linux/wave2feat \
        -i $infile   \
        -o $outfile  \
        -srate 8000 \
        -lowerf 130  \
        -upperf 3800 \
	-nfilt 32   \
        -dither      \
	-mswav         \
	-feat sphinx \
        >> $logfile
    else 
        echo $file not found >> $logfile
    endif
