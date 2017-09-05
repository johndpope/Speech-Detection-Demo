/* Copyright 2013 Chris Wilson

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   */

   window.AudioContext = window.AudioContext || window.webkitAudioContext;
   navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia;
   window.URL = window.URL || window.webkitURL;
   navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia;


   var audioContext = new AudioContext();
   var audioInput = null,
   realAudioInput = null,
   inputPoint = null,
   audioRecorder = null;
   var rafID = null;
   var analyserContext = null;
   var canvasWidth, canvasHeight;
   var recIndex = 0;

   var g = null;
   var g2 = null;

   var spectro;
   var microphoneButton;
   var songButton;
   var songSelect;
   var selectedMedia;

   function gotBuffers( buffers ) {
    var canvas = document.getElementById( "wavedisplay" );

    //drawBuffer( canvas.width, canvas.height, canvas.getContext('2d'), buffers[0] );

    // the ONLY time gotBuffers is called is right after a new recording is completed - 
    // so here's where we should set up the download.
    audioRecorder.exportMonoWAV( doneEncoding );

  }

  function doneEncoding( blob ) {
    var link = document.getElementById("save");
    link.className = "active";

    var fd = new FormData();
    fd.append('file', blob);
    $.ajax({
      type: 'POST',
      url: '/upload',
      data: fd,
      processData: false,
      contentType: false
    }).done(function(data) {
        //console.log(data);
        //alert(data)
        if(data == "False"){
          start = true;
          redraw(settings,"fakeData.json");
          redraw(settings2,"fakeData3.json");
          redraw(settings3,"fakeData2.json");
          gauge2.update(0);
          gauge3.update(0);
          gauge4.update(0);
          gauge5.update(0);
          gauge6.update(0);
          gauge7.update(0);
          gauge8.update(0);
          gauge9.update(0);
          gauge10.update(0);
          gauge11.update(0);
          gauge12.update(0);
          gauge13.update(0);
          gauge14.update(0);
          gauge15.update(0);
          gauge16.update(0);
          gauge17.update(0);
          gauge18.update(0);
          gauge19.update(0);
          gauge20.update(0);
          gauge21.update(0);
          gauge22.update(0);
          gauge23.update(0);
          gauge24.update(0);
          gauge25.update(0);
          document.getElementById('speech').innerHTML = "";
          // Enter a speed between 0 and 180
          
        }
        else{
          start = false; //that means there is no speech
          redraw(settings,"fakeData.json");
          redraw(settings2,"fakeData3.json");
          redraw(settings3,"fakeData2.json");
          gauge2.update(Math.random()*100);
          gauge3.update(Math.random()*100);
          gauge4.update(Math.random()*100);
          gauge5.update(Math.random()*100);
          gauge6.update(Math.random()*100);
          gauge7.update(Math.random()*100);
          gauge8.update(Math.random()*100);
          gauge9.update(Math.random()*100);
          gauge10.update(Math.random()*100);
          gauge11.update(Math.random()*100);
          gauge12.update(Math.random()*100);
          gauge13.update(Math.random()*100);
          gauge14.update(Math.random()*100);
          gauge15.update(Math.random()*100);
          gauge16.update(Math.random()*100);
          gauge17.update(Math.random()*100);
          gauge18.update(Math.random()*100);
          gauge19.update(Math.random()*100);
          gauge20.update(Math.random()*100);
          gauge21.update(Math.random()*100);
          gauge22.update(Math.random()*100);
          gauge23.update(Math.random()*100);
          gauge24.update(Math.random()*100);
          gauge25.update(Math.random()*100);
          //redrawChart(settings, data)
          document.getElementById('speech').innerHTML = data;
          //guage2.update(0);
        }

      });

  }
  var INTERVALID = 0
  function toggleRecording( e ) {
    if (e.classList.contains("recording")) {
        // stop recording
        
        e.classList.remove("recording");
        clearInterval(INTERVALID)
      } else {
        // start recording
        if (!audioRecorder)
          return;
        e.classList.add("recording");
        var recording = false;
        
        INTERVALID = setInterval(function(){
          if (recording){
            audioRecorder.stop();
            audioRecorder.getBuffers( function(buffer){
              gotBuffers(buffer)
              audioRecorder.clear();
              audioRecorder.record();
            });
          }
          else{
            audioRecorder.clear();
            audioRecorder.record();
          }
          recording = true;
        }, 5000)
        
      }
    }

    function convertToMono( input ) {
      var splitter = audioContext.createChannelSplitter(2);
      var merger = audioContext.createChannelMerger(2);

      input.connect( splitter );
      splitter.connect( merger, 0, 0 );
      splitter.connect( merger, 0, 1 );
      return merger;
    }

    function cancelAnalyserUpdates() {
      window.cancelAnimationFrame( rafID );
      rafID = null;
    }
    /*
    function updateAnalysers(time) {
      if (!analyserContext) {
        var canvas = document.getElementById("analyser");
        canvasWidth = canvas.width;
        canvasHeight = canvas.height;
        analyserContext = canvas.getContext('2d');
      }

    // analyzer draw code here
    {
      var SPACING = 3;
      var BAR_WIDTH = 1;
      var numBars = Math.round(canvasWidth / SPACING);
      var freqByteData = new Uint8Array(analyserNode.frequencyBinCount);

      analyserNode.getByteFrequencyData(freqByteData); 

      analyserContext.clearRect(0, 0, canvasWidth, canvasHeight);
      analyserContext.fillStyle = '#F6D565';
      analyserContext.lineCap = 'round';
      var multiplier = analyserNode.frequencyBinCount / numBars;

        // Draw rectangle for each frequency bin.
        for (var i = 0; i < numBars; ++i) {
          var magnitude = 0;
          var offset = Math.floor( i * multiplier );
            // gotta sum/average the block, or we miss narrow-bandwidth spikes
            for (var j = 0; j< multiplier; j++)
              magnitude += freqByteData[offset + j];
            magnitude = magnitude / multiplier;
            var magnitude2 = freqByteData[i * multiplier];
            analyserContext.fillStyle = "hsl( " + Math.round((i*360)/numBars) + ", 100%, 50%)";
            analyserContext.fillRect(i * SPACING, canvasHeight, BAR_WIDTH, -magnitude);
          }
        }

        rafID = window.requestAnimationFrame( updateAnalysers );
      }*/

      function toggleMono() {
        if (audioInput != realAudioInput) {
          audioInput.disconnect();
          realAudioInput.disconnect();
          audioInput = realAudioInput;
        } else {
          realAudioInput.disconnect();
          audioInput = convertToMono( realAudioInput );
        }

        audioInput.connect(inputPoint);
      }

      function gotStream(stream) {
        inputPoint = audioContext.createGain();

    // Create an AudioNode from the stream.
    realAudioInput = audioContext.createMediaStreamSource(stream);
    audioInput = realAudioInput;
    audioInput.connect(inputPoint);

//    audioInput = convertToMono( input );

analyserNode = audioContext.createAnalyser();
analyserNode.fftSize = 2048;
inputPoint.connect( analyserNode );

audioRecorder = new Recorder( inputPoint );

zeroGain = audioContext.createGain();
zeroGain.gain.value = 0.0;
inputPoint.connect( zeroGain );
zeroGain.connect( audioContext.destination );
//updateAnalysers();
handleMicStream(stream);
    //setInterval(function(){ gotBuffers(stream)}, 3000);
  }

  function loadMedia(selectedMedia, callback) {
    songButton.disabled = false;

    var request = new XMLHttpRequest();
    request.open('GET', selectedMedia.file, true);
    request.responseType = 'arraybuffer';

    request.onload = function() {
      audioContext.decodeAudioData(request.response, function(buffer) {
        var slice = selectedMedia.slice;
        AudioBufferSlice(buffer, slice.start, slice.end, function(error, buf) {
          callback(buf);
        });
      });
    };

    request.send();
  }

  function selectMedia() {
    songButton.disabled = false;
    selectedMedia = media[songSelect.value];
  }

  function playSong() {
    loadMedia(selectedMedia, function(songBuffer) {
      spectro.connectSource(songBuffer, audioContext);
      spectro.start();
    });

    removeControls();
  }

  function requestMic() {
    navigator.getUserMedia({
      video: false,
      audio: true
    },
    function(stream) {
      handleMicStream(stream);
      removeControls();
    }, handleMicError);
  }

  function handleMicStream(stream) {
    var input = audioContext.createMediaStreamSource(stream);
    var analyser = audioContext.createAnalyser();

    analyser.smoothingTimeConstant = 0;
    analyser.fftSize = 2048;

    input.connect(analyser);

    spectro.connectSource(analyser, audioContext);
    spectro.start();
  }

  function handleMicError(error) {
    alert(error);
    console.log(error);
  }

  function removeControls() {
    songSelect.parentNode.removeChild(songSelect);
    songButton.parentNode.removeChild(songButton);
    microphoneButton.parentNode.removeChild(microphoneButton);
  }

  var settings
  var settings2

  function initAudio() {
    if (!navigator.getUserMedia)
      navigator.getUserMedia = navigator.webkitGetUserMedia || navigator.mozGetUserMedia;
    if (!navigator.cancelAnimationFrame)
      navigator.cancelAnimationFrame = navigator.webkitCancelAnimationFrame || navigator.mozCancelAnimationFrame;
    if (!navigator.requestAnimationFrame)
      navigator.requestAnimationFrame = navigator.webkitRequestAnimationFrame || navigator.mozRequestAnimationFrame;

    navigator.getUserMedia(
    {
      "audio": {
        "mandatory": {
          "googEchoCancellation": "false",
          "googAutoGainControl": "false",
          "googNoiseSuppression": "false",
          "googHighpassFilter": "false"
        },
        "optional": []
      },
    }, gotStream, function(e) {
      alert('Error getting audio');
      console.log(e);
    });
    settings = setup('#chartForBar');
    settings2 = setup('#chartForBar2');
    settings3 = setup('#chartForBar3')
    redraw(settings,"fakeData.json");
    redraw(settings2,"fakeData3.json");
    redraw(settings3,"fakeData2.json");
    

      
    init();
    /*
    g = new JustGage({
    id: "viz6",
    value: 67,
    min: 0,
    max: 100,
    title: "Hira Yasin",
    label: "Speaker 1"
  });

    g2 = new JustGage({
    id: "viz7",
    value: 20,
    min: 0,
    max: 100,
    title: "Shahan Ali Memon",
    label: "Speaker 2"
  });*/
  
  
  
}

function init() {
  spectro = Spectrogram(document.getElementById('spectrogram'), {
    canvas: {
      width: function() {
        return window.innerWidth;
      },
      height: 300
    },
    audio: {
      enable: true
    },
    colors: function(steps) {
      //var baseColors = [[32,32,32,0], [0,255,255,1], [0,255,0,1], [255,255,0,1], [ 255,0,0,1]];
      //var baseColors = [[32,32,32,0], [32,32,32,0], [32,32,32,0], [32,32,32,0], [ 255,0,0,1]];
      var baseColors = [[0,0,0,0], [0,0,0,0], [0,0,0,0], [0,0,0,0], [ 255,0,0,1]];
      var positions = [0, 0.15, 0.30, 0.50, 0.75];

      var scale = new chroma.scale(baseColors, positions)
      .domain([0, steps]);

      var colors = [];

      for (var i = 0; i < steps; ++i) {
        var color = scale(i);
        colors.push(color.hex());
      }

      return colors;
    }
  });

  try {
    audioContext = new AudioContext();
  } catch (e) {
    alert('No web audio support in this browser!');
  }
  
}

// gauges
var config1 = liquidFillGaugeDefaultSettings();
config1.circleColor = "#FF7777";
config1.textColor = "#FF4444";
config1.waveTextColor = "#FFAAAA";
config1.waveColor = "#FFDDDD";
config1.circleThickness = 0.2;
config1.textVertPosition = 0.2;
config1.waveAnimateTime = 1000;
var gauge2= loadLiquidFillGauge("fillgauge2", 0, config1);
var gauge3= loadLiquidFillGauge("fillgauge3", 0, config1);
var gauge4= loadLiquidFillGauge("fillgauge4", 0, config1);
var gauge5= loadLiquidFillGauge("fillgauge5", 0, config1);
var gauge6= loadLiquidFillGauge("fillgauge6", 0, config1);
var gauge7= loadLiquidFillGauge("fillgauge7", 0, config1);
var gauge8= loadLiquidFillGauge("fillgauge8", 0, config1);
var gauge9= loadLiquidFillGauge("fillgauge9", 0, config1);
var gauge10= loadLiquidFillGauge("fillgauge10", 0, config1);
var gauge11= loadLiquidFillGauge("fillgauge11", 0, config1);
var gauge12= loadLiquidFillGauge("fillgauge12", 0, config1);
var gauge13= loadLiquidFillGauge("fillgauge13", 0, config1);
var gauge14= loadLiquidFillGauge("fillgauge14", 0, config1);
var gauge15= loadLiquidFillGauge("fillgauge15", 0, config1);
var gauge16= loadLiquidFillGauge("fillgauge16", 0, config1);
var gauge17= loadLiquidFillGauge("fillgauge17", 0, config1);
var gauge18= loadLiquidFillGauge("fillgauge18", 0, config1);
var gauge19= loadLiquidFillGauge("fillgauge19", 0, config1);
var gauge20= loadLiquidFillGauge("fillgauge20", 0, config1);
var gauge21= loadLiquidFillGauge("fillgauge21", 0, config1);
var gauge22= loadLiquidFillGauge("fillgauge22", 0, config1);
var gauge23= loadLiquidFillGauge("fillgauge23", 0, config1);
var gauge24= loadLiquidFillGauge("fillgauge24", 0, config1);
var gauge25= loadLiquidFillGauge("fillgauge25", 0, config1);
function NewValue(){
    if(Math.random() > .5){
        return Math.round(Math.random()*100);
    } else {
        return (Math.random()*100).toFixed(1);
    }
}

window.addEventListener('load', initAudio );
