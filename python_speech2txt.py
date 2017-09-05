import subprocess
import sys
import os
import re

def speech2text(audio_filename,output_filename):
	cmdstring = "bash speech2text_package.sh %s" %(audio_filename)
	os.system(cmdstring)
	cmdstring1 = "cat %s" %(output_filename)
	output = subprocess.check_output(cmdstring1, shell="True")
	output1 = output.replace('\n','')
	#regex = re.compile(".*?\((.*?)\)")
	#result = re.findall(regex,output1)
	result = re.sub("[\(\[].*?[\)\]]", "", output1)
	print result
	return result

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print 'Takes arg1 = input filename, arg2 = output filename'
	else:
		speech2text(sys.argv[1],sys.argv[2])
