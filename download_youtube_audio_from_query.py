#Author - Ankit Shah ankit.tronix@gmail.com for any issues
import pafy
import os
import sys
import subprocess
from shutil import copyfile

def format_audio(input_audio_file,output_audio_file):
	print input_audio_file
	temp_audio_file = output_audio_file.split('.wav')[0] + '_temp.wav'
	cmdstring = "ffmpeg -i %s -ac 1 -ar 16000 %s" %(input_audio_file,temp_audio_file)
	os.system(cmdstring)
	cmdstring1 = "sox %s -G -b 16 -r 16000 %s" %(temp_audio_file,output_audio_file)
	os.system(cmdstring1)
	cmdstring2 = "rm -rf %s" %(temp_audio_file)
	os.system(cmdstring2)

def split_audio(input_audio_file,split_interval,output_audio_dir):
	duration = subprocess.check_output("soxi -D %s" %(input_audio_file),shell=True).split("\n")[0]
	start = 0
	num_files = int(float(duration))/int(split_interval)
	print num_files
	for i in range(num_files+1):
		print i
		if i == num_files:
			end = int(float(duration))
			split_interval = end - start
		else:
			end = start + split_interval
		output_audio_file = output_audio_dir + input_audio_file.split('.wav')[0] + "_" + str(start) + "_" + str(end) + ".wav"
		print output_audio_file
		output_cmd = "sox %s -G -b 16 -r 16000 %s trim %s %s" %(input_audio_file,output_audio_file,start,split_interval)
		os.system(output_cmd)
		start = start + split_interval


def download_youtube_audio_from_query(query_id,download_root_path):	
	url = "https://www.youtube.com/watch?v=" + query_id
	try:
		video = pafy.new(url)
		bestaudio = video.getbestaudio()
		path_to_download = download_root_path + "/Y" + query_id + "." + str(bestaudio.extension)
		bestaudio.download(path_to_download)
		audio_name = "Y" + query_id + '.' + str(bestaudio.extension)
	except Exception as ex:
		print ex
	
	output_audio_name = "Y" + query_id + '.wav'
	format_audio(audio_name,output_audio_name)
	output_dir = download_root_path + "/audio_output/"
	if not os.path.exists(output_dir):
		os.makedirs(output_dir)
	#split_audio(output_audio_name,3,output_dir)
	output_file_name = output_dir + '/' + output_audio_name
	copyfile(output_audio_name,output_file_name)

	return audio_name


if __name__ == "__main__":
	if len(sys.argv) != 3:
		print 'Takes arg1 as query id arg 2 as the root path'
	else:
		download_youtube_audio_from_query(sys.argv[1],sys.argv[2])
