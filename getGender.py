import pyaudio
import wave
import sys

from matplotlib.mlab import find
import pyaudio
import numpy as np
import math


chunk = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
RECORD_SECONDS = 20

def Pitch(signal):
    signal = np.fromstring(signal, 'Int16');
    crossing = [math.copysign(1.0, s) for s in signal]
    index = find(np.diff(crossing));
    f0=round(len(index) *RATE /(2*np.prod(len(signal))))
    return f0;

def genderCalc(filename):
    # length of data to read.


    wf = wave.open(filename, 'rb')
    # create an audio object
    p = pyaudio.PyAudio()

    # open stream based on the wave object which has been input.
    stream = p.open(format =
                    p.get_format_from_width(wf.getsampwidth()),
                    channels = wf.getnchannels(),
                    rate = wf.getframerate(),
                    output = True)

    # read data (based on the chunk size)
    data = wf.readframes(chunk)

    # play stream (looping from beginning of file to the end)
    freqs = []
    while data != '':
        # writing to the stream is what *actually* plays the sound.
        #stream.write(data)
        data = wf.readframes(chunk)
        Frequency=Pitch(data)
        #print "%f Frequency" %Frequency
        if (Frequency != 0.0):
            freqs.append(Frequency)
    #print np.sort(freqs)
    f0 = min(freqs)
    print f0
    if f0 <= 200:
        return "Male"
    else:
        return "Female"

genderCalc("hira2.wav")
'''
# validation. If a wave file hasn't been specified, exit.
if len(sys.argv) < 2:
    print "Plays a wave file.\n\n" +\
          "Usage: %s filename.wav" % sys.argv[0]
    sys.exit(-1)


************************************************************************
      This is the start of the "minimum needed to read a wave"
************************************************************************
'''
# open the file for reading.
#wf = wave.open(sys.argv[1], 'rb')





