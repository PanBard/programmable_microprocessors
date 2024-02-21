const char MAIN_page[] PROGMEM = R"=====(














<!DOCTYPE html>
<html>
<style>
body{
  background: #808080;
  font-family: "Lucida Console", "Courier New", monospace;
}
.display{
    width: 400px;
     min-height: 250px;
     background: #404040;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
     display:block;}
.hide{
	display:none;
}
.button_style{
 	background: #999999;
 	border-radius: 10px;
 	width: 90px;
	height: 50px;
}
#row{
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;
}
#row2{
    display: flex;
    flex-direction: row;
    align-items: center;
}
#co{
	border: 2px solid black;
 	background: #404040;
    color: #FFF;
    padding: 10px;
     display: flex;
    flex-direction: row;
    flex-wrap: nowrap;
    justify-content: center;
    align-items: flex-start;
    align-content: space-between;
}
input {
  	width: 100%;
  	margin: 2px;
}
.slider {
  border-radius: 5px;
  background: #E0E0E0;
  background: linear-gradient(left top, #E0E0E0, #EEEEEE);
  width: 190px;
  height: 15px;
  margin: 5px;
}

.thumb {
  width: 10px;
  height: 25px;
  border-radius: 3px;
  position: relative;
  left: 10px;
  top: -5px;
  background: blue;
  cursor: pointer;
}

</style>



<body>

<div id="co">
    <div  class="display">
        <h1>Panel kontrolny</h1><br>
        
        <div id="row" >
            <h4></h4>
            <button  class="button_style" type="button" onclick="postRequest('up')">&uarr;</button><br><br>
        </div><br>
        
        <div id="row" >
            <button  class="button_style" type="button" onclick="postRequest('left')">&larr;</button>
            <button  class="button_style" type="button" onclick="postRequest('down')">&darr;</button>
            <button  class="button_style" type="button" onclick="postRequest('right')">&rarr;</button>
        </div>
	</div>
    
</div>


<div id="element_1" class="hide">
    <h1>Settings</h1><br>
    <div id="row" >
    	<h4>Row 1: </h4>
        <button class="button_style" type="button"onclick="postRequest('position_zero')">Wyzeruj</button><br><br>
        <button class="button_style" type="button"onclick="postRequest(250)">Option 2</button><br><br>
        <button class="button_style" type="button"onclick="postRequest(500)">Option 3</button><br><br>
    </div>
    
    <div id="row" >
        <h4>Power: </h4>
        <button class="button_style" type="button"onclick="postRequest(17)">17</button>
        <button class="button_style" type="button"onclick="postRequest(19)">19</button>
        <button class="button_style" type="button"onclick="postRequest(20)">20</button>
	</div>
   	
</div><br>
  
<button class="button_style" id="changer" type="button" onclick="buttonFunction()">Settings</button>
<br><br>

	
<div id="row2" > X axis: <p id="xaxis"></p>  	</div>
<input id="slide_x" type="range" min="0" max="180" step="1" value="0" onchange="postRequest_x_servo(this.value); display_value_x(this.value);">

<div id="row2" > Y axis: <p id="yaxis"></p>  	</div>
<input id="slide_y" type="range" min="0" max="180" step="1" value="0" onchange="postRequest_y_servo(this.value); display_value_y(this.value);">

<script>
  kopo = true;
  
function buttonFunction() {
   if(document.getElementById("element_1").className == "display"){
   		document.getElementById("element_1").className = "hide";
   		document.getElementById("changer").innerHTML = "Settings";
        }
   else{
   		document.getElementById("element_1").className = "display";
   		document.getElementById("changer").innerHTML = "Hide Settings";
        }
}


function postRequest(text){
    str_message = text.toString();
    fetch("post", {
  			method: "POST",
  			body: JSON.stringify({ message: str_message }),
  			headers: { "Content-type": "application/json; charset=UTF-8"}
            });
}

function postRequest_x_servo(text){
    str_message = text.toString();
    fetch("x_servo", {
  			method: "POST",
  			body: JSON.stringify({ message: str_message }),
  			headers: { "Content-type": "application/json; charset=UTF-8"}
            });
}

function postRequest_y_servo(text){
    str_message = text.toString();
    fetch("y_servo", {
  			method: "POST",
  			body: JSON.stringify({ message: str_message }),
  			headers: { "Content-type": "application/json; charset=UTF-8"}
            });
}


    
       function showCoordsX() {
  let slik = document.getElementById('sliderX');
  let y = slik.offsetLeft;     
  
  let thumbser = document.getElementById('iks');
  let y1 = thumbser.offsetLeft;
    
  document.getElementById("info").innerHTML = ' ANGLE X:'+(y1-y);

  postRequest_x_servo(y1-y);
}

function showCoordsY() {
  let slik = document.getElementById('sliderY');
  let y = slik.offsetLeft;     
  
  let thumbser2 = document.getElementById('igrek');
  let y3 = thumbser2.offsetLeft;
  
  document.getElementById("info2").innerHTML = 'ANGLE Y:'+(y3-y);
  postRequest_y_servo(y3-y);
}

function display_value_x(value){
  const x_display = document.querySelector("#xaxis");
  x_display.innerHTML = value;
}

function display_value_y(value){
  const y_display = document.querySelector("#yaxis");
  y_display.innerHTML = value;
}

</script>

</body>
</html>














)=====";