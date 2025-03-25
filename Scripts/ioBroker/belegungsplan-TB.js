// copy of calender-img with addition of "Besprechungsraum"

const axios = require('axios');
const fs = require('fs');
const ical = require('ical');

const { createCanvas, loadImage } = require('canvas');

/******************/
/*  Canvas-Setup  */
/******************/

const width  = 600;
const height = 800;
const canvas = createCanvas(width, height);
const ctx    = canvas.getContext('2d');

const fs_icon    = "40px";
const fs_larger  = "32px";
const fs_large   = "28px";
const fs_reg     = "24px";
const fs_small   = "20px";
const fs_legend   = "18px";
const font       = 'monospace';
const fs_bold    = "bold";
const fs_caps    = "small-caps";
const smallSpace = 30; 
const space      = 40;
const bigSpace   = 50;
const text_x     = 80;
const icon_x     = 20; 

const free_icon     = "◯";
const pending_icon  = "◒";
const occupied_icon = "●"; // "◉" 
const future_icon   = "→";

let yPosition;
let currentDate;
let upcoming = '';
let upcomingString = '';
let nextEvent = '';
let isOccupied = false;
let futureEvents = [];

// schedule("00 * * * *", function() {

/*******************/
/*  Datumsabfrage  */
/*******************/

const today = new Date(); 
// const today = new Date('February 26, 2025, 14:00'); // test date

let upcomingDate = new Date(today);
upcomingDate.setDate(today.getDate() + 14);
upcoming = upcomingDate.toISOString().substring(0, 10);
console.log(upcoming)

let day    = addZero(today.getDate());
let month  = addZero(today.getMonth() + 1);
let year   = today.getFullYear();
let hour   = today.getHours();
let minute = today.getMinutes();

currentDate = `${year}-${month}-${day}`;

ctx.fillStyle = 'white';
ctx.fillRect(0, 0, width, height);


/*****************/
/*  Überschrift  */
/*****************/

yPosition = 60;

ctx.textAlign = 'center'
ctx.fillStyle = 'black';
ctx.font      = `${fs_larger} ${font}`

ctx.fillText(`Belegungsplan ${day}.${month}.${year}`, 300, yPosition);
yPosition += space;

ctx.font = `${fs_large} ${font}`
ctx.fillText(`Stand ${addZero(hour)}:${addZero(minute)} Uhr`, 300, yPosition);
yPosition += bigSpace;

ctx.textAlign = 'left'


/**********************/
/*  Kalender-Quellen  */
/**********************/

const calendarURLs = [
    'https://tools.mi.ur.de/davical/public.php/mi-lab/TB-Besprechungsraum',
    'https://tools.mi.ur.de/davical/public.php/mi-lab/VR4-Labor',
    'https://tools.mi.ur.de/davical/public.php/mi-lab/VR4-Studio',
    'https://tools.mi.ur.de/davical/public.php/mi-lab/VR4-Werkstatt',
];
const names = [
    'Besprechungsraum',
    'Labor',
    'Studio',
    'Werkstatt',
];


/******************/
/*  Zeichen-Loop  */
/******************/

async function draw() {
    for (let i = 0; i < calendarURLs.length; i++) {
        await fetchCalendar(calendarURLs[i])
        .then(data => {
            const events = readCalendar(data);
            const organizedEvents = organizeEvents(events);
            createCalendarImage(organizedEvents, names[i], i);
            // data[names[i]] = organizedEvents;
        })
        .catch(error => {
            console.error('Error fetching or processing calendar:', error);
        })
    }
    writeFutureEvents();
    writeLegend();
    saveImage();
}
draw();

// });


/****************/
/*  Funktionen  */
/****************/

async function fetchCalendar(url) {
    try {
        const response = await axios.get(url);
        return response.data;
    } catch (error) {
        throw new Error(`Failed to fetch calendar data: ${error}`);
    }
}

function readCalendar(data) {
    const events = ical.parseICS(data);
    return events;
}

function organizeEvents(events) {
    const organizedEvents = {};
    for (const key in events) {
        if (events[key].type === 'VEVENT') {
            const event = events[key];
            const date = event.start.toISOString().split('T')[0];
            if (!organizedEvents[date]) {
                organizedEvents[date] = [];
            }
            organizedEvents[date].push({
                summary: event.summary,
                start: event.start,
                end: event.end
            });
        }
    }
    return organizedEvents;
}

function addZero(month) {
    if (month < 10) {
        return '0' + month;
    }
    return month;
}

function createCalendarImage(events, name, iteration) {
    ctx.font = `${fs_large} ${fs_bold} ${font}`;
    ctx.fillText(name, text_x, yPosition);
    const name_yPos = yPosition;
    yPosition += smallSpace;

    ctx.font = `${fs_reg} ${font}`;

    let todayIncluded = false;
    let nextDateKnown = false;

    for (const date of Object.keys(events).sort()) {
        if (date == currentDate) {
            todayIncluded = true;
            events[date].sort((a, b) => a.start - b.start);

            if (events[date].length > 0) {
                for (const event of events[date]) {
                    writeEvent(event.summary, event);

                    // check if currently occupied
                    if((hour+minute) >= (event.start.getHours() + event.start.getMinutes()) &&
                       (hour+minute) <= (event.end.getHours()   + event.end.getMinutes())) {
                        isOccupied = true;
                    }
                }
                yPosition += bigSpace - smallSpace ;

            if (isOccupied) {
                ctx.font = `${fs_icon} ${font}`;
                ctx.fillText(occupied_icon, icon_x, name_yPos);
            } else {
                ctx.font = `${fs_icon} ${font}`;
                ctx.fillText(pending_icon, icon_x, name_yPos);
            }
            }
        } else if (date > currentDate && date <= upcoming) { // check next 14 days
            upcoming = date; // uncomment for only 1 future event per room
            events[date].sort((a, b) => a.start - b.start);
            nextEvent = events[date][0];
            upcomingString = name + ", " + upcoming.substring(8, 10) + '.' + upcoming.substring(5, 7) + '.' + upcoming.substring(0, 4);;
            futureEvents.push([upcomingString, nextEvent]);
        }
    }


    if (!todayIncluded) {
        yPosition -= smallSpace;
        ctx.font = `${fs_icon} ${font}`;
        ctx.fillText(free_icon, icon_x, yPosition)
        yPosition += bigSpace;
    }
}

function saveImage() {
    let buffer = canvas.toBuffer('image/png');
    // writeFile('web', '/calendarTB.png', buffer, function (error) { }) // ioBroker-compliant
    fs.writeFileSync('calendarTB.png', buffer); // Zu Testzwecken lokal ausgeben lassen
}

function writeLegend() { // at bottom right edge
    let y = 780; 
    let x = 220;
    str = `${free_icon} frei // ${pending_icon} reserviert // ${occupied_icon} belegt`; 
    ctx.font = `${fs_legend} ${font}`;
    ctx.fillText(str, x, y)
}

// Write the reservations below everything
function writeFutureEvents() {
    let obfuscated = 0;
    yPosition += smallSpace;
    ctx.font = `${fs_icon} ${font}`;
    ctx.fillText(future_icon, icon_x, yPosition);
    ctx.font = `${fs_reg} ${font}`;
    ctx.fillText("Nächste Reservierungen: ", text_x, yPosition);
    ctx.font = `${fs_small} ${font}`;
    yPosition += ( smallSpace + 5 );

    // console.log(futureEvents)
    futureEvents.forEach( e => {
        if (yPosition >= 730) {
            obfuscated += 1;
        } else {
            ctx.fillText(e[0], text_x, yPosition);
            yPosition += smallSpace;
            writeEvent(e[1].summary, e[1])
        }
    });

    if (obfuscated > 0) {
        ctx.fillText(`+ ${obfuscated}`, text_x, yPosition);
    }
}

function writeEvent(text, event) {
    let textarr;
    let line1;
    let line2;

    if(text.includes("Reservierung:")) {
        text = text.replace("Reservierung:", "")
    }
    
    if (text.length > 25) {
        if (text.includes(',')) { // split at comma
            textarr = text.split(/(, )(.*)/s);
            line1 = textarr[0] + textarr[1];
            line2 = textarr[2];
        }
        else if (text.includes(' ')) { // or split at space
            textarr = text.split(/\s(.*)/s);
            line1 = textarr[0];
            line2 = textarr[1];
        } else {
            line1 = text.substring(0, 25);
            line2 = text.substring(25);
        }
        if (line2.length > 35) {
            line2 = line2.substring(0, 30) + "..."
        }
        let eventText = `${addZero(event.start.getHours())}:${addZero(event.start.getMinutes())}–${addZero(event.end.getHours())}:${addZero(event.end.getMinutes())}: ${line1}`;
        ctx.fillText(eventText, text_x, yPosition);
        yPosition += smallSpace;
        ctx.fillText(line2, text_x, yPosition);
    } else {
        let eventText = `${addZero(event.start.getHours())}:${addZero(event.start.getMinutes())}–${addZero(event.end.getHours())}:${addZero(event.end.getMinutes())}: ${text}`;
        ctx.fillText(eventText, text_x, yPosition);
    }
    yPosition += space;
}
