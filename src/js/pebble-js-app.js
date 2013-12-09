Pebble.addEventListener("ready",
	function(e) {			
		var req = new XMLHttpRequest();
  		req.open('GET', 'http://www.netfonds.no/quotes/paperhistory.php?paper=EVRY.OSE&csv_format=csv', true);
  		req.onload = function(e) {
			if (req.readyState == 4 && req.status == 200) {
				if(req.status == 200) {
					var response = req.responseText.split(/\r\n|\r|\n/g);
					var lastClosing = response[1].split(",")[6];
					var previousDayClosing = response[2].split(",")[6];
					
					if(previousDayClosing > lastClosing) {
						Pebble.sendAppMessage({ "increasedValue":1, "stock_quote": "" + lastClosing + " | " + (Math.round((lastClosing - previousDayClosing) * 100) / 100) });
					} else if(previousDayClosing < lastClosing) {
						Pebble.sendAppMessage({ "increasedValue":2, "stock_quote": "" + lastClosing + " | +" + (Math.round((lastClosing - previousDayClosing) * 100) / 100) });
					} else {
						Pebble.sendAppMessage({ "increasedValue":0, "stock_quote": "" + lastClosing + " | 0.0" });
					}					
				} else { 
					console.log("Error"); 
				}
			}
  		}
	
  		req.send(null);
	}									
);