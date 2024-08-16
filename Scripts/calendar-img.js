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

const fs_larger = "32px";
const fs_large  = "28px";
const fs_reg    = "24px";
const font      = 'monospace';
const space     = 40;
const bigSpace  = 50;

let yPosition;
let currentDate;
let upcomingString1 = '';
let upcomingString2 = '';

schedule("*/15 * * * *", function() {
/*******************/
/*  Datumsabfrage  */
/*******************/

const today = new Date();

let upcoming = new Date(today);
upcoming.setDate(today.getDate() + 14);
upcoming = upcoming.toISOString().substring(0, 10);

let day    = addZero(today.getDate());
let month  = addZero(today.getMonth() + 1);
let year   = today.getFullYear();
let hour   = today.getHours();
let minute = today.getMinutes();

currentDate = `${year}-${month}-${day}`;
//currentDate = `2024-02-02`; // Zu Testzwecken Datum im Format 'yyyy-mm-dd' überschreiben

ctx.fillStyle = 'white';
ctx.fillRect(0, 0, width, height);


/*****************/
/*  Überschrift  */
/*****************/

yPosition = 80;

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
    'https://tools.mi.ur.de/davical/public.php/mi-lab/VR4-Labor',
    'https://tools.mi.ur.de/davical/public.php/mi-lab/VR4-Werkstatt',
    'https://tools.mi.ur.de/davical/public.php/mi-lab/VR4-Studio'
];
const names = [
    'Labor',
    'Werkstatt',
    'Studio'
];


/******************/
/*  Zeichen-Loop  */
/******************/

for (let i = 0; i < calendarURLs.length; i++) {
    fetchCalendar(calendarURLs[i])
        .then(data => {
            const events = readCalendar(data);
            const organizedEvents = organizeEvents(events);
            createCalendarImage(organizedEvents, names[i], i);
        })
        .catch(error => {
            console.error('Error fetching or processing calendar:', error);
        });
}
});


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
    ctx.font = `${fs_large} ${font}`;
    ctx.fillText(name, 50, yPosition);
    yPosition += space;

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
                }
                yPosition += bigSpace - space ;
            } else {
                ctx.fillText("keine Termine heute.", 50, yPosition);
                yPosition += bigSpace;
            }
        } else if (date > currentDate && !nextDateKnown) {
            if (date <= upcoming) {
                upcoming = date;
                events[date].sort((a, b) => a.start - b.start);
                nextEvent = events[date][0];
                upcomingString1 = "Nächste Reservierung: ";
                upcomingString2 = name + ", " + upcoming.substring(8, 10) + '.' + upcoming.substring(5, 7) + '.' + upcoming.substring(0, 4);;
            }
            nextDateKnown = true;
        }
    }

    if (!todayIncluded) {
        ctx.fillText("keine Termine heute.", 50, yPosition);
        yPosition += bigSpace;
    }

    if (upcomingString1.length > 0 && iteration == 2) {
        ctx.font = `${fs_reg} ${font}`;
        ctx.fillText(upcomingString1, 50, yPosition);
        yPosition += space;
        ctx.fillText(upcomingString2, 50, yPosition);
        yPosition += space;
        writeEvent(nextEvent.summary, nextEvent)
    }

    buffer = canvas.toBuffer('image/png');
    writeFile('web', '/calendar1.png', buffer, function (error) { }) // ioBroker-compliant
    //fs.writeFileSync('calendar1.png', buffer); // Zu Testzwecken lokal ausgeben lassen
}

function writeEvent(text, event) {
    if (text.length > 15) {
        if (text.includes(' ')) {
            textarr = text.split(' ');
            line1 = textarr[0];
            line2 = textarr[1];
        } else {
            line1 = text.substring(0, 15);
            line2 = text.substring(15);
        }
        if (line2.length > 35) {
            line2 = line2.substring(0, 30) + "..."
        }
        let eventText = `${addZero(event.start.getHours())}:${addZero(event.start.getMinutes())}–${addZero(event.end.getHours())}:${addZero(event.end.getMinutes())} Uhr: ${line1}`;
        ctx.fillText(eventText, 50, yPosition);
        yPosition += space;
        ctx.fillText(line2, 50, yPosition);
    } else {
        let eventText = `${addZero(event.start.getHours())}:${addZero(event.start.getMinutes())}–${addZero(event.end.getHours())}:${addZero(event.end.getMinutes())} Uhr: ${event.summary}`;
        ctx.fillText(eventText, 50, yPosition);
    }
    yPosition += space;
}