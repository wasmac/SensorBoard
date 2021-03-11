const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
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
  justify-content: space-evenly;
}
.content {
    position: relative;
}
.content img {
    position: absolute;
    top: 0px;
    right: 0px;
}

</style>
<body>

<div id="content">
    <img src="https://assets.website-files.com/6007f53a4f491002dd93b886/600809d62d5efc3657a3b114_logo_mayht.svg" class="ribbon"/>
</div>

<div class="container">
  <div class="card" style="background: rgb(214, 122, 16);">
    <h1>Temperature reading</h1><br>
    <h2>Temp: <span id="temp">0</span> *C</h2><br>
  </div>

  <div class="card">
    <h1>Pressure reading in Pascals</h1><br>
    <h2><span id="pressure">0</span> Pa</h2><br>
  </div>

  <div class="card" style="background: rgb(139, 14, 14);">
    <h1>Voltage reading in Volts</h1><br>
    <h2><span id="pressure">0</span> V</h2><br>
  </div>

  <div class="card" style="background: rgb(204, 218, 8);">
    <h1>Current reading in mili Amps</h1><br>
    <h2><span id="pressure">0</span> mA</h2><br>
  </div>

  <div class="card" style="background: rgb(27, 156, 92);">
    <h1>Pressure reading in dB[SPL]</h1><br>
    <h2><span id="soundLevel">0</span> dB</h2><br>
  </div>

  <div class="card" style="background: rgb(129, 107, 119);">
    <h1>Power reading in Wats</h1><br>
    <h2><span id="pressure">0</span> W</h2><br>
  </div>

  <div class="card" style="background: rgb(43, 231, 206);">
    <h1>Frequency response reading in Hz</h1><br>
    <h2><span id="pressure">0</span> Hz</h2><br>
  </div>
</div>
<script>

  InitWebSocket()
  function InitWebSocket()
  {
    websock = new WebSocket('ws://'+window.location.hostname+':88/');
    websock.onmessage=function(evt)
    {
       JSONobj = JSON.parse(evt.data);
       var temp = parseInt(JSONobj.temp);
       var pressure = parseInt(JSONobj.pressure);
       var spl = parseInt(JSONobj.spl);
       document.getElementById("temp").innerHTML = temp;
       document.getElementById("pressure").innerHTML = pressure;
       document.getElementById("soundLevel").innerHTML = spl;
       
    } // end of onmessage
      
  } // end of InıtWebSocket


</script>
</body>
</html>
)=====";