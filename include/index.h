const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.min.js"></script>
</head>
<style>


header {
	width: 100%;
	height: auto;
	overflow: hidden;
}
header img {
  margin-left: 50%;
  transform: translateX(-50%);
}
.pagebody {
  width: 100%;
  padding: 20px;
  margin: 5%;
}

.card{
  width: 400px;
  height: 250px;
  background: #02b875;
  border-radius: 10px;
  padding: 30px;
  box-sizing: border-box;
  color: #FFF;
  margin:20px;
  box-shadow: 0px 2px 18px -4px rgba(189, 31, 31, 1);
  display: block;
  /* flex-direction: column; */
}

.container{
  display: flex;
  flex-wrap: wrap;
  justify-content: space-around;
}
.content {
    position: relative;

}


.split {
  width: 50%;
  position: absolute;
  z-index: 1;
  top: 150px;
  overflow-x: hidden;
  padding-top: 20px;
  padding-bottom: 20px;
}


/* Control the left side */
.left {
  left: 0;
  background-color: rgb(255, 255, 255);
}

/* Control the right side */
.right {
  right: 0;
  background-color: rgb(255, 255, 255);
}

/* If you want the content centered horizontally and vertically */
.centered {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  text-align: center;
}




</style>


<body>

<header>
  <img src="https://assets.website-files.com/6007f53a4f491002dd93b886/600809d62d5efc3657a3b114_logo_mayht.svg" />
<header/>


<div class="pagebody">

  <div class="split left">
    <div class="container">
      <div class="card" style="background: rgb(214, 122, 16);">
        <h1>Temperature reading</h1><br>
        <h2><span id="temp">0</span> *C</h2><br>
      </div>
      <div class="card">
        <h1>Pressure reading in Pascals</h1><br>
        <h2><span id="pressure">0</span> Pa</h2><br>
      </div>
        <div class="card" style="background: rgb(139, 14, 14);">
      <h1>Voltage reading in Volts</h1><br>
      <h2><span id="voltage">0</span> V</h2><br>
    </div>
    <div class="card" style="background: rgb(204, 218, 8);">
      <h1>Current reading in mili Amps</h1><br>
      <h2><span id="current">0</span> mA</h2><br>
    </div>

    <div class="card" style="background: rgb(27, 156, 92);">
      <h1>Pressure reading in dB[SPL]</h1><br>
      <h2><span id="soundLevel">0</span> dB</h2><br>
    </div>

    <div class="card" style="background: rgb(129, 107, 119);">
      <h1>Power reading in Wats</h1><br>
      <h2><span id="power">0</span> W</h2><br>
    </div>

    <div class="card" style="background: rgb(43, 231, 206);">
      <h1>Frequency response reading in Hz</h1><br>
      <h2><span id="frequency">0</span> Hz</h2><br>
    </div>
  </div>
  </div>

  <div class="split right">
    <div class="container">
      <div class="card" style="background: rgb(255, 255, 255);     width: 600px;  height:350px;">
        <canvas id="myChartTemp" style="width: 100%; height: 100%;"></canvas>
      </div>
      <div class="card" style="background: rgb(255, 255, 255);     width: 600px;  height:350px;">
        <canvas id="myChartPressure" style="width: 100%; height: 100%;"></canvas>
      </div>
      <div class="card" style="background: rgb(255, 255, 255);     width: 600px;  height:350px;">
        <canvas id="myChartSPL" style="width: 100%; height: 100%;"></canvas>
      </div>
      <div class="card" style="background: rgb(255, 255, 255);     width: 600px;  height:350px;">
        <canvas id="myChartFFT" style="width: 100%; height: 100%;"></canvas>
      </div>
  </div>


</div>


<script>

  let myChartTemp, myChartPressure, myChartSPL, myChartFFT;
  var maxDataPoints = 90;

  function removeTempData(){
    myChartTemp.data.labels.shift();
    myChartTemp.data.datasets[0].data.shift();
  }

  function addTempData(label, data) {
    let tempName = "myChartTemp";
    if(myChartTemp.data.labels.length > maxDataPoints) removeTempData();
    myChartTemp.data.labels.push(label);
    myChartTemp.data.datasets[0].data.push(data);
    myChartTemp.update();
  }
  function removePressureData(){
    myChartPressure.data.labels.shift();
    myChartPressure.data.datasets[0].data.shift();
  }

  function addPressureData(label, data) {
    let tempName = "myChartTemp";
    if(myChartPressure.data.labels.length > maxDataPoints) removePressureData();
    myChartPressure.data.labels.push(label);
    myChartPressure.data.datasets[0].data.push(data);
    myChartPressure.update();
  }
  function removeSPLData(){
    myChartSPL.data.labels.shift();
    myChartSPL.data.datasets[0].data.shift();
  }

  function addSPLData(label, data) {
    let tempName = "myChartTemp";
    if(myChartSPL.data.labels.length > maxDataPoints) removeSPLData();
    myChartSPL.data.labels.push(label);
    myChartSPL.data.datasets[0].data.push(data);
    myChartSPL.update();
  }

  function removeFFTData(){
    myChartFFT.data.labels.shift();
    myChartFFT.data.datasets[0].data.shift();
  }

  function addFFTData(label, data) {
    let tempName = "myChartFFT";
    if(myChartTemp.data.labels.length > maxDataPoints) removeFFTData();
    myChartFFT.data.labels.push(label);
    myChartFFT.data.datasets[0].data.push(data);
    myChartFFT.update();
  }


  InitWebSocket()
  function InitWebSocket()
  {
    websock = new WebSocket('ws://'+window.location.hostname+':88/');
    websock.onmessage=function(evt)
    {
       JSONobj = JSON.parse(evt.data);
       var Temp = parseInt(JSONobj.temp);
       var Pressure = parseInt(JSONobj.pressure);
       var Spl = parseInt(JSONobj.spl);
       document.getElementById("temp").innerHTML = Temp;
       document.getElementById("pressure").innerHTML = Pressure;
       document.getElementById("soundLevel").innerHTML = Spl;
       var today = new Date();
       var t = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
       addTempData(t, Temp);
       addPressureData(t, Pressure);
       addSPLData(t, Spl);
    } // end of onmessage
      
  } // end of InıtWebSocket



let ctx = document.getElementById('myChartTemp').getContext('2d');
myChartTemp = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [0],
        datasets: [{
            label: '*C',
            data: [0],
            backgroundColor: [],
            borderColor: [],
            borderWidth: 1,
            fill: false
        }]
    },
    options: {
      title: {
        display: false
      },
      legend: {
        display: false
      },
      scales: {
					xAxes: [{
						scaleLabel: {
							display: true,
							labelString: 'Time'
						}
					}],
					yAxes: [{
						stacked: true,
						scaleLabel: {
							display: true,
							labelString: 'Temp'
						}
					}]
				}
    }
});

let ctx1 = document.getElementById('myChartPressure').getContext('2d');
myChartPressure = new Chart(ctx1, {
    type: 'bar',
    data: {
        labels: [0],
        datasets: [{
            label: 'Pressure',
            data: [0],
            backgroundColor: [],
            borderColor: [],
            borderWidth: 1,
            fill: false
        }]
    },
    options: {
      title: {
        display: false
      },
      legend: {
        display: false
      },
      scales: {
					xAxes: [{
						scaleLabel: {
							display: true,
							labelString: 'Time'
						}
					}],
					yAxes: [{
						stacked: true,
						scaleLabel: {
							display: true,
							labelString: 'hPa'
						}
					}]
				}
    }
});

let ctx2 = document.getElementById('myChartSPL').getContext('2d');
myChartSPL = new Chart(ctx2, {
    type: 'line',
    data: {
        labels: [0],
        datasets: [{
            label: 'SPL',
            data: [0],
            backgroundColor: [],
            borderColor: [],
            borderWidth: 1
        }]
    },
    options: {
      title: {
        display: false
      },
      legend: {
        display: false
      },
      scales: {
					xAxes: [{
						scaleLabel: {
							display: true,
							labelString: 'Time'
						}
					}],
					yAxes: [{
						stacked: true,
						scaleLabel: {
							display: true,
							labelString: 'dB'
						}
					}]
				}
    }
});


let ctx3 = document.getElementById('myChartFFT').getContext('2d');
myChartFFT = new Chart(ctx3, {
    type: 'bar',
    data: {
        labels: [0],
        datasets: [{
            label: 'Freq',
            data: [0],
            backgroundColor: [],
            borderColor: [],
            borderWidth: 1,
            fill: false
        }]
    },
    options: {
      title: {
        display: false
      },
      legend: {
        display: false
      },
      scales: {
					xAxes: [{
						scaleLabel: {
							display: true,
							labelString: 'Amplitude'
						}
					}],
					yAxes: [{
						stacked: true,
						scaleLabel: {
							display: true,
							labelString: 'hPa'
						}
					}]
				}
    }
});


</script>
</body>
</html>
)=====";