// Script für Schwarzes Brett

const jsonID = "0_userdata.0.msg.JSON";
const archiveID = "0_userdata.0.msg.JSON-log";
const inputID = "0_userdata.0.msg.msgInput";
const htmlID = "0_userdata.0.msg.HTMLTable";
const htmlIDPublic = "0_userdata.0.msg.HTMLTablePUBLIC";
const topicID = "0_userdata.0.msg.msgTopics";


// A inline script that will update the JSON-object when a message is checked off
// Only seemed to work this way, as js-events can only be accessed when they are inline or in a script tag (couldn't get that to work)
const clickScript = `
const target = arguments[0].target;
const solvedMsgID = arguments[0].target.id;
const checked = arguments[0].target.checked;
const oid = '0\_userdata\.0\.msg\.JSON\';
vis.conn.getStates(oid, (error, states) => {
    const json = states[oid].val;
    let data = JSON.parse(json);
    data[solvedMsgID].resolved = checked;
    vis.setValue('0\_userdata\.0\.msg\.JSON\',JSON.stringify(data));
});`;
// This updates the JSON-object. A new table is built, when the object changes

const deleteScript = 
`
const target = arguments[0].target;
let boxID = arguments[0].target.id;
const deletedMsgID = boxID.split('-')[1];
console.log('del ' + deletedMsgID);
const oid = '0\_userdata\.0\.msg\.JSON\'
vis.conn.getStates(oid, (error, states) => {
    const json = states[oid].val;
    let data = JSON.parse(json);

    const archive = data[deletedMsgID];

    delete data[deletedMsgID];
    vis.setValue('0\_userdata\.0\.msg\.JSON', JSON.stringify(data));

    console.log('archive: ' + archive);
    vis.setValue('0\_userdata\.0\.msg\.JSON-log',JSON.stringify(archive));
    
});`

// Parse json to html table (+checkbox) + save in separate object --> to display in vis
function parseToTable(data, isAdmin, objectID) {
    let adminHeader = "";
        if(isAdmin) {
            adminHeader = `
                <th align="left" style="position: sticky; top: 0; background-color: white; color:black;">Done</th>
                <th align="left" style="position: sticky; top: 0; background-color: white; color:black;">Delete</th>
            `;
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
            topicColor = "#FED7E9"; // "#4C058F";
        }

        let checkboxScript = "";
        if(isAdmin) {
            checkboxScript = `
            <td style="border-top: solid 1px #999; background-color:${color}; color:black; padding-left: 5px;" align="center">
                <input id="${msgID}" type="checkbox" name="resolved" ${checked} onclick="${clickScript}">
            </td>
            <td style="border-top: solid 1px #999; background-color:${color}; color:black; padding-left: 5px;" align="center">
                <input id="D-${msgID}" type="checkbox" name="delete" onclick="${deleteScript}">
            </td>
            `
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

    setState(objectID, table);
}


// Add msg to JSON on change
on(inputID, function(obj) {
    log("new msg: " + getState(inputID).val); // Read
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
    // console.log(data)

    // Ovveride to existing json
    setState(jsonID, JSON.stringify(data));
    
    parseToTable(data, true, htmlID);
    parseToTable(data, false, htmlIDPublic)

});

// doesn't visually update the widget
on(jsonID, function() {
    const json = getState(jsonID).val;
    let data = JSON.parse(json);
    parseToTable(data, true, htmlID);
    parseToTable(data, false, htmlIDPublic)
});

// add the deleted msg to the archive-file
on(archiveID, function() {
    // TODO: append msg to archived file;
    const json = getState(archiveID).val;
    let newLog = JSON.parse(json);

    readFile("0_userdata.0", "schwarzes-brett/archive.json",  function (error, data) { 
        addToArchive(data, newLog);
    });

});

function addToArchive(data, newLog) {

    let msgs = JSON.parse(data);
    const newID = Object.keys(msgs).length;

    msgs[newID] = newLog;
    writeFile("0_userdata.0", "schwarzes-brett/archive.json", JSON.stringify(msgs), function (error) { log("err: " + error) });
}

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
