from flask import Flask, request, redirect, url_for, send_from_directory
from werkzeug.utils import secure_filename
import time
import random
from detectVoiceInWave import *
import downsample
import vad2
import os
import sys
#import getGender
# Setup Flask app.
app = Flask(__name__)
app.debug = True

# Routes
@app.route('/')
def root():
  return app.send_static_file('index.html')

@app.route('/<path:path>')
def static_proxy(path):
  # send_static_file will guess the correct MIME type
  return app.send_static_file(path)

@app.route('/upload', methods = ['GET', 'POST'])
def upload_file():
   if request.method == 'POST':
      f = request.files['file']
      #TimeStamp So we can track a file
      timeStamp = str(time.time())
      #Output Name of file to save.
      outputName = "audio" + timeStamp + ".wav"
      #Saving file for testing.. Later, should not save!
      f.save(secure_filename(outputName))
      thereIsSpeech = False
      # todo: PROCESSING on audio.wav
      #gen = getGender.genderCalc("audio.wav")
      #print gen
      #print outputName
      thereIsSpeech = containsSpeech(outputName)
      if(thereIsSpeech):
        #Then let us double check as we want to be aggressive
        #So first we need to downsample the audio
        #print outputName
        pathForDownSample = os.getcwd() + "/"+outputName
        outPathForDownSample = os.getcwd() + "/"+outputName +"_down.wav"
        downsample.downsampleWav(pathForDownSample, outPathForDownSample)
        #We are overwriting, we should not do that.. so we will change that later
        #First argument is aggressiveness and second is path to wab file
        thereIsSpeech = vad2.voiceDetect([3,outPathForDownSample])
        
      print "There is Speech:", thereIsSpeech
      return str(thereIsSpeech);
      #return 'Your predicted Gender = ' + str(random.randint(20,45))

if __name__ == '__main__':
  #app.run("0.0.0.0")
  app.run()

  
