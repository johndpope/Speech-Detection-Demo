#!/bin/bash
#python download_youtube_audio_from_query.py $1 /usr1/nels/task_speech2txt/
cd audio_output
ls ${1}*.wav > audio_list.ctl
sed -ie 's/^/audio_output\//g' audio_list.ctl
sed -ie 's/.wav//g' audio_list.ctl
cd ..
./scripts/compute_mfcc_modified.csh audio_output/audio_list.ctl
rm -rf decoding/decode.ctl
python create_decode_ctl.py audio_output/audio_list.ctl decoding/decode.ctl
cd decoding
#cp ../audio_output/audio_list.ctl decode.ctl
rm -rf result/*
./run_decode.csh 1 1 
cp ../audio_output/audio_list.ctl ../audio_output/audio_list_${1}.ctl
cp decode.ctl decode_${1}.ctl
cd ..