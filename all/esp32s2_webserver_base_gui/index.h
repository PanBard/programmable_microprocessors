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
        <button class="button_style" type="button"onclick="postRequest(125)">Option 1</button><br><br>
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

</script>

</body>
</html>












)=====";