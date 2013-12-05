Pebble.addEventListener("ready",
	function(e) {			
		// Create interval
		var req = new XMLHttpRequest();
  		req.open('GET', 'http://www.netfonds.no/quotes/paperhistory.php?paper=EVRY.OSE&csv_format=csv', true);
  		req.onload = function(e) {
			if (req.readyState == 4 && req.status == 200) {
				if(req.status == 200) {
					var response = JSON.parse(req.responseText); // split CSV
					var temperature = result.list[0].main.temp;
					var icon = result.list[0].main.icon;
					Pebble.sendAppMessage({ "icon":icon, "temperature":temperature + "\u00B0C"}); // Create mapping for "icon" => integer
				} else { 
					console.log("Error"); 
				}
			}
  		}
	
  		req.send(null);
	}									
);