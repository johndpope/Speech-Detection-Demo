#Author - Ankit Shah ankit.tronix@gmail.com for any issues
import os
import sys
import subprocess
from shutil import copyfile

def create_decode_ctl(input_audio_ctl,output_decode_ctl):	
	try:
		file_read = open(input_audio_ctl,"r")
		line = file_read.readline().strip()
		print line
		audio_absolute_name = os.getcwd() + '/' +   line.strip() + '.wav'
		print audio_absolute_name
		cmdstring = "soxi -D %s" %(audio_absolute_name) 
		duration = subprocess.check_output(cmdstring , shell="True").strip()
		print duration
		duration_int = int(float(duration))
		print duration_int
		for i in range(duration_int):
			with open(output_decode_ctl,'a') as f1:
				f1.write(line + ' ' + str(100*i) + ' ' + str(100*(i+1)) + ' ' + line + '_' + str(100*i) + '_' + str(100*(i+1)) + '\n')
				f1.close()
	except Exception as ex:
		print ex
	

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print 'Takes arg1 as input_audio_control_file, arg2 = output file'
	else:
		create_decode_ctl(sys.argv[1],sys.argv[2])
