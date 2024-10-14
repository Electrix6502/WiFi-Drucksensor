function toggleButton(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/button?b="+element.id, true);
  xhr.send();
  loadButtonState();
}

function toggleButtonSw(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/buttonSw?b="+element.id, true);
  xhr.send();
  loadButtonStateSw();
}

function getButtonState(id) {
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
	  if (this.readyState == 4 && this.status == 200) {
		  if (this.responseText == '1'){
			  document.getElementById(id).style.backgroundColor = 'red';
		  } else {
			  document.getElementById(id).style.backgroundColor = '#007bff';
		  }
	  }
	};
	xhttp.open("GET", "/button?s="+id, true);
	xhttp.send();	
}

function loadButtonState() {
	for(let i = 0; i < 11; i++){
		getButtonState(i);
	}
}

function getButtonStateSw(id) {

	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
	  if (this.readyState == 4 && this.status == 200) {
		  if (this.responseText == '1'){
			  document.getElementById(id).style.backgroundColor = 'red';
		  } else {
			  document.getElementById(id).style.backgroundColor = '#007bff';
		  }
	  }
	};
	xhttp.open("GET", "/buttonSw?s="+id, true);
	xhttp.send();	
}

function loadButtonStateSw() {
	for(let i = 0; i < 17; i++){
		getButtonStateSw(i);
	}
}	 
 
function getData(id){
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
	  if (this.readyState == 4 && this.status == 200) {
		document.getElementById(id).value = this.responseText;
	  }
	};
	xhttp.open("GET", "/data?c="+id, true);
	xhttp.send();	
}

function getText(id){
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
	  if (this.readyState == 4 && this.status == 200) {
		document.getElementById(id).innerHTML = this.responseText;
	  }
	};
	xhttp.open("GET", "/data?c="+id, true);
	xhttp.send();	
}

function getCheckbox(id){
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
	  if (this.readyState == 4 && this.status == 200) {
		  if (this.responseText == '1'){
			  document.getElementById(id).checked = true;
		  } else {
			  document.getElementById(id).checked = false;
		  }
	  }
	};
	xhttp.open("GET", "/data?c="+id, true);
	xhttp.send();	
}

