const char MAIN_page[] PROGMEM = R"=====(














<!DOCTYPE html>
<html>
<style>
body{
  background: #808080;
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
.active_button_style{
	 background: #999999;
 	border-radius: 10px;
 	width: 90px;
 	height: 50px;
 	border: 2px solid green;
}
.sender_button_style{
	 background: #999999;
 	border-radius: 10px;
 	width: 80px;
 	height: 30px;
    margin: 2px;
}
#element_2{
 	background: #4d4d4d;
}

#element_404{
	 width: 400px;
     height: 50px;
     background: #404040;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
     display:block;
     display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;
}
#row{
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;
}
div.ex3 {
  	background-color:#595959;
  	//width: 500px;
  height: 300px;
  overflow: auto;
  padding: 5px;
}
div.ex3 > p{
	background-color: #404040;
	padding: 1px;
	padding-left: 10px;
    margin: 3px;
}
#co{
	border: 2px solid black;
 	background: #404040;
    color: #FFF;
    padding: 10px;
}
input {
  	width: 100%;
  	margin: 2px;
}

</style>



<body>

<div id="co">
	<div id="row" > 
        <h2>LoRa Console</h2>
        <button id="autoScroll" class="button_style" type="button"onclick="autoScroll()">autoscroll ON</button>
	</div>
    <div id="console" class="ex3"> </div>
	<div id="row" >
        <button class="sender_button_style"  type="button" onclick="sendMessage()">Send</button>
        <input type="text" id="message_input" name="lname" >        
	</div>
   
	<div id="row" >
      <h4>Received packets: <span id="counter">0</span> </h4><br>
      <h4>Connected devices: <span id="connected">0</span> </h4>
	</div>
</div>

<div id="element_1" class="hide">
    <h1>Settings</h1><br>
    <div id="row" >
    	<h4>Bandwidth: </h4>
        <button id="125" class="button_style" type="button"onclick="postRequest(125)">125E3</button><br><br>
        <button id="250" class="button_style" type="button"onclick="postRequest(250)">250E3</button><br><br>
        <button id="500" class="button_style" type="button"onclick="postRequest(500)">500E3</button><br><br>
    </div>
        <div id="row" >
        <h4>Power: </h4>
        <button id="17" class="button_style" type="button"onclick="postRequest(17)">17</button>
        <button id="19"  class="button_style" type="button"onclick="postRequest(19)">19</button>
        <button id="20" class="button_style" type="button"onclick="postRequest(20)">20</button>
	</div>
   	<button class="button_style" type="button"onclick="postRequest()">Ping</button>
</div><br>
  
<button class="button_style" id="changer" type="button" onclick="buttonFunction()">Settings</button>

<br><br>

<!--
<button class="button_style" type="button"onclick="addElement(342,1)">Add</button>
<button class="button_style" type="button"onclick="clearConsole()">remove</button>
-->

  <script>
  kopo = true;
  
function sendMessage(){
  	LoRaMessage = "";
  	LoRaMessage = document.getElementById('message_input').value;
  	fetch("loraMessageSender", {
  		method: "POST",
  		body: JSON.stringify( {message: LoRaMessage} ),
        headers: { "Content-type": "application/json; charset=UTF-8"}
        });
}
  
function clearConsole() {
  	const element = document.getElementById("console");
	element.innerHTML = '';
}
  
function autoScroll(){
  	if(kopo){
    kopo = false;
    document.getElementById("autoScroll").innerHTML = "Autoscroll OFF";
    }
    else{
    kopo = true;
    document.getElementById("autoScroll").innerHTML = "Autoscroll ON";
    }
}
  
function addElement(mes,nr){
  	const para = document.createElement("p");
    para.id = "record";
	const node = document.createTextNode("< "+nr.toString()+" >   "+mes.toString());
	para.appendChild(node);
	const element = document.getElementById("console");
	element.appendChild(para);
    if(kopo){
        var objDiv = document.getElementById("console");
  		objDiv.scrollTop = objDiv.scrollHeight;
        }
}
  
  
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

function changeClass(mes){
	document.getElementById(mes).className = "active_button_style";
}

function postRequest(number){
	message = "";
   const array2 = [17, 19, 20];
   const array3 = [125,250,500];
	if(number == array2[0] || number == array2[1] || number == array2[2]){
    	array2.forEach((element) => document.getElementById(element).className = "button_style");
    }
    else{
    array3.forEach((element) => document.getElementById(element).className = "button_style");
    }
    switch (number) {
  case 125:
    message = "Bandwidth["+number.toString()+"E3]" ;
    document.getElementById(number).className = "active_button_style";
    break;
  case 250:
  	message = "Bandwidth["+number.toString()+"E3]" ;
    document.getElementById(number).className = "active_button_style";
    break;
  case 500:
  	message = "Bandwidth["+number.toString()+"E3]" ;
    document.getElementById(number).className = "active_button_style";
    break;
  case 17:
  	message = "Power["+number.toString()+"]" ;
     document.getElementById(number).className = "active_button_style";
    break;
  case 19:
  	message = "Power["+number.toString()+"]" ;
    document.getElementById(number).className = "active_button_style";
    break;
  case 20:
  	message = "Power["+number.toString()+"]" ;
    document.getElementById(number).className = "active_button_style";
    break;
  default:
    message = "Default message";
	}

     fetch("post", {
  			method: "POST",
  			body: JSON.stringify({ message: message, title: "Fix my bugs", completed: false}),
  			headers: { "Content-type": "application/json; charset=UTF-8"}
            });
}

setInterval(
	function(){
        getCounter();
        getConnectionNumber();
        getLog();
     }
    ,1000); //1000mSeconds update rate

function getLog(){
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function(){
        if (this.readyState == 4 && this.status == 200){
          originalString = this.responseText;
            separatedArray = [];
       		separatedArray = originalString.split('\n');
            clearConsole();
            for (let i = 0; i < separatedArray.length; i++) {
            		if(separatedArray[i] != "\n"){
                    	addElement(separatedArray[i],i+1);
                    }
  			}                     
        }
      }
      xhttp.open("GET", "readLoraLogs", true);
      xhttp.send();
}

function getCounter() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function(){
        if (this.readyState == 4 && this.status == 200){
          document.getElementById("counter").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "readCounter", true);
      xhttp.send();
}

function getConnectionNumber(){
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function(){
        if (this.readyState == 4 && this.status == 200){
          document.getElementById("connected").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "readConnection", true);
      xhttp.send();
}

  </script>

</body>
</html>



















)=====";