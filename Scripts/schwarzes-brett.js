// Script für Schwarzes Brett (copy of ioBroker script)
// Creates a HTML-Table (var=htmlID(Public)) from a JSON-object (var=jsonID) to display messages as they get added to another object (var=inputID)
// updates the JSON object when a checkbox gets clicked to mark a message as "Done" (does not update HTML-Table on click, as of: 02.09.24)

const jsonID = "0_userdata.0.msg.JSON";
const inputID = "0_userdata.0.msg.msgInput";
const htmlID = "0_userdata.0.msg.HTMLTable";
const htmlIDPublic = "0_userdata.0.msg.HTMLTablePUBLIC";
const topicID = "0_userdata.0.msg.msgTopics";

const json = getState(jsonID).val;
const obj = JSON.parse(json);

// READ json (example)
for (var msgID in obj) {
    for (var key in obj[msgID]) {
        console.log(msgID + ', ' + key + " => " + obj[msgID][key]);
    }
}

// A inline script that will update the JSON-object when a message is checked off
// Only seemed to work this way, as js-events can only be accessed when they are inline or in a script tag (couldn't get that to work)
const clickScript = 
`const target = arguments[0].target;
const solvedMsgID = arguments[0].target.id;
const checked = arguments[0].target.checked;
const oid = '0\_userdata\.0\.msg\.JSON\'
vis.conn.getStates(oid, (error, states) => {
    const json = states[oid].val;
    let data = JSON.parse(json);
    data[solvedMsgID].resolved = checked;
    vis.setValue('0\_userdata\.0\.msg\.JSON\',JSON.stringify(data));
});`;
// This won't generate a new table, so the changes might not visualy persist
// TODO -> can't access script from inline-event and can't really access widget from inside script (at least don't know how)

// Parse json to html table (+checkbox) + save in separate object --> to display in vis
function parseToTable(data, isAdmin, objectID) {
    let adminHeader = "";
        if(isAdmin) {
            adminHeader = `<th align="left" style="position: sticky; top: 0; background-color: white; color:black;">Done</th>`;
    }

    const header = 
    `<table>
        <tr>
            <th align="left" style="position: sticky; top: 0; background-color: white; color:black;">Topic</th>
            <th align="left" style="position: sticky; top: 0; background-color: white; color:black;">Date</th>
            <th align="left" style="position: sticky; top: 0; background-color: white; color:black;">Message</th>
            ${adminHeader}
        </tr>
    `;
    let body = header;

    for (var i = Object.keys(data).length; i > 0; i--) {
        const msgID = i - 1;
    
        // Change color depending on resolved issues
        let color = "#fdedec";
        let checked = "";
        if (data[msgID].resolved == true) {
            color = "#eafaf1";
            checked = "checked";
        }

        // Different colors for topics
        let topicColor = "#AAD5EE"; // "#2274A5";
        if (data[msgID].topic == "admin") {
            topicColor = "#A5A9DF"; // "#93047C";
        } else if (data[msgID].topic == "tech") {
            topicColor = "#D6B1FC"; // "#373D9A";
        } else if (data[msgID].topic == "study") {
            topicColor = "#FED7E9";// "#4C058F";
        }

        let checkboxScript = "";
        if(isAdmin) {
            checkboxScript = `
            <td style="border-top: solid 1px #999; background-color:${color}; color:black; padding-left: 5px;" align="center">
                <input id=${msgID} type="checkbox" name="resolved" ${checked} onclick="${clickScript}">
            </td>`
        }

        const row = 
        `<tr>
            <td style="border-top: solid 1px #999; background-color:${topicColor}; color:black; padding: 0 7px 3px 7px; border-radius:20px;" align="center">${data[msgID].topic}</td>
            <td style="border-top: solid 1px #999; background-color:${color}; color:black; padding-left: 5px;">${data[msgID].date}</td>
            <td style="border-top: solid 1px #999; background-color:${color}; color:black; padding-left: 5px; width: 70%">${data[msgID].msg}</td>
            ${checkboxScript}            
        </tr>`;
        body = body + row;
    }
    const table = body + "</table>";

    //console.log("table created");
    setState(objectID, table);
}


// Add msg to JSON on change
on(inputID, function(obj) {
    console.log("new msg: " + getState(inputID).val); // Read
    const newMsg = getState(inputID).val; 
    const json = getState(jsonID).val;
    let data = JSON.parse(json);

    // console.log("keys: " + Object.keys(data).length)
    const newID = Object.keys(data).length;
    const today = new Date();
    let newDate = today.getDate() +'.'+(today.getMonth()+1)+'.'+ today.getFullYear() ;
    const newTopic = getState(topicID).val;
    // console.log("topic " + newTopic);

    
    data[newID] = {
        topic: newTopic,
        date: newDate,
        msg: newMsg,
        resolved: false, 
    };
    console.log(data)

    // Ovveride to existing json
    setState(jsonID, JSON.stringify(data));
    
    parseToTable(data, true, htmlID);
    parseToTable(data, false, htmlIDPublic)

});

// update table, when the Tabs-Widget is clicked ($(...).click is not a funktion)
/*$('#w000067').click(function () { 
    console.log("clicked tabs widget");
    const json = getState(jsonID).val;
    let data = JSON.parse(json);
    parseToTable(data);
});
*/


/* Example-JSON
{
  "0": {
    "topic": "all",
    "date": "23.08.2024",
    "msg": "Drucker defekt",
    "resolved": false
  },
  "1": {
    "topic": "all",
    "date": "23.08.2024",
    "msg": "Filter wechseln",
    "resolved": true
  }
}
*/
