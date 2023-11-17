const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
body{
  background: #808080;
}
.card{
    max-width: 400px;
     min-height: 250px;
     background: #404040;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
</style>


<body>

  <div class="card">
    <h1>Data checker</h4><br>
    <h4>Lora data:<span id="LoraMessage">0</span></h1><br>
    <h4>Received packets:<span id="counter">0</span></h2><br>
  </div>



  <script>
    setInterval
    (function(){
        getMessage();// Call a function 
        getCounter();
      }
    ,1000); //1000mSeconds update rate

    function getMessage() 
    {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() 
      {
        if (this.readyState == 4 && this.status == 200) 
        {
          document.getElementById("LoraMessage").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "readLoraMessage", true);
      xhttp.send();
    }

     function getCounter() 
    {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() 
      {
        if (this.readyState == 4 && this.status == 200) 
        {
          document.getElementById("counter").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "readCounter", true);
      xhttp.send();
    }

  </script>

</body>
</html>
)=====";