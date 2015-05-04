var ws;

window.onload = function() {
  var messageContainer = document.getElementById("chat");
  if("WebSocket" in window) {
	messageContainer.appendChild(createChatEntry("[ezChat]", "Welcome to ezChat!<p> </p>"));
	openWS(messageContainer);
  }
  else {
	messageContainer.appendChild(createChatEntry("[ezChat]", "Sorry, ezChat is NOT supported by your browser!"));
  }
}

function sendMessage() {
  var data = {  author: document.getElementById("username").value,
			   message: document.getElementById("message").value};
  
  if(data.author && data.message) {
	ws.send(JSON.stringify(data));
  }
}

function openWS(messageContainer) {
  ws = new WebSocket("ws://" + window.location.host + /*192.168.0.101*/ ":8080/ws");
  ws.onmessage = function(e) {
	var data = JSON.parse(e.data);
	messageContainer.appendChild(createChatEntry(data.author, data.message));
	var chatContainer = document.getElementById("chat");
	chatContainer.scrollTop = chatContainer.scrollHeight;          
  };
  ws.onclose = function(e) {
	openWS(messageContainer);
  };
}

function createChatEntry(username, message) {
  var entry = document.createElement("div");
  entry.class = "chat_entry";
  var usermsg = document.getElementById("username").value;
  if ( usermsg == "") {
  	entry.className = "red";
  } else if ( usermsg == username) {
  	entry.className = "green";
  } else {
  	entry.className = "blue";
  }
  var dom_uname = document.createElement("span");
  dom_uname.className = "chatUser";
  dom_uname.class = "chat_username";
  dom_uname.innerHTML = username+": ";
  entry.appendChild(dom_uname);
  var dom_msg = document.createElement("span");
  dom_msg.className = "chatMsg";
  dom_msg.class = "chat_message";
  dom_msg.innerHTML = message;
  entry.appendChild(dom_msg);
  return entry;		 
}


