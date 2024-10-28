// MQTT - IDs
const idLab = "mqtt.0.CO2-Ampel.Ampel_1";
const idWerk = "mqtt.0.CO2-Ampel.Ampel_Werkstatt";
const idStudio = "mqtt.0.CO2-Ampel.Ampel_Studio";
const inIDs = {
    "lab":    "mqtt.0.CO2-Ampel.Ampel_1",
    "werk":   "mqtt.0.CO2-Ampel.Ampel_Werkstattt",
    "studio": "mqtt.0.CO2-Ampel.Ampel_Studio"
    }

// Object IDs (write)
const outIDs = {
    "lab":    "0_userdata.0.CO2-Ampel.Ampel_Labor",
    "werk":   "0_userdata.0.CO2-Ampel.Ampel_Werkstatt",
    "studio": "0_userdata.0.CO2-Ampel.Ampel_Studio"
    }


// mit BMP gemessene und gemittelte Differenzen hier eintragen
// Temperatur (wird addiert)
const tempOffsets = {
    "lab":    6.718,
    "werk":   0,
    "studio": 0
    }

// Luftfeuchtichkeit (wird subtrahiert)
const humOffsets = {
    "lab":    35.68,
    "werk":   0,
    "studio": 0
    }

// Check and create States for the Ampeln
function checkStates(){
    
    for (const [room, id] of Object.entries(outIDs)) {
        if (existsState(id)) return;

        var AmpelJson = getState(inIDs[room]).val;
        const obj = JSON.parse(AmpelJson);
        
        for (var key in obj) {
            createState(id + "." + key , "", {name: key,  type: 'string'});
        }
    }

}
checkStates();


// Parse the incomming mqtt-json and split into objects
schedule('*/15 * * * * *', function() { // every 15 min == '*/15 * * * *'
    for (const [room, id] of Object.entries(outIDs)) {
        var AmpelJson = getState(inIDs[room]).val;
        const obj = JSON.parse(AmpelJson);

        for (var key in obj) {
            let val = obj[key];
            if (key == 'temp') { 
                val = parseInt(val) + parseInt(tempOffsets[room]);
            } else if (key == 'hum') {
                val = val - humOffsets[room];
            }
            setState(id + "." + key, val.toString(), true);
        }
    }
})