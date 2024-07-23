const axios = require('axios');
const fs = require('fs');
const ical = require('ical');

const { createCanvas, loadImage } = require('canvas');

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
    } return month;
}

function createCalendarImage(events, outputPath) {
    const width = 600;
    const height = 800;
    const canvas = createCanvas(width, height);
    const ctx = canvas.getContext('2d');

    ctx.fillStyle = 'white';
    ctx.fillRect(0, 0, width, height);

    ctx.fillStyle = 'black';
    ctx.font = '16px Arial';

    ctx.fillText("Belegung FIL", 50, 30);


    let yPosition = 50;
    let todayIncluded = false;

    for (const date of Object.keys(events).sort()) {
        const today = new Date();
        let day = today.getDate();
        let month = addZero(today.getMonth() + 1);
        let year = today.getFullYear();

        let currentDate = `${year}-${month}-${day}`;
        
        if(date == currentDate) {
            todayIncluded = true;
            console.log("Date match!");
            ctx.fillText(date, 50, yPosition);
            yPosition += 30;
            if (events[date].length > 0) {
                for (const event of events[date]) {
                    const eventText = `${event.summary} (${event.start.toTimeString().split(' ')[0]} - ${event.end.toTimeString().split(' ')[0]})`;
                    ctx.fillText(eventText, 100, yPosition);
                    yPosition += 20;
                    console.log(eventText);
                } 
            } else {
                ctx.fillText("keine Termine heute.", 50, yPosition);
            }
        }
    }

    todayIncluded ? null : ctx.fillText("keine Termine heute.", 50, yPosition);

    const buffer = canvas.toBuffer('image/png');
    writeFile('0_userdata.0', '/calendar1.png', buffer, function (error) { });

}

// Usage
const calendarUrl = 'https://tools.mi.ur.de/davical/public.php/mi-lab/VR4-Labor';
const outputImagePath = './files/0_userdata.0/images/calendar.png'; 

function getRunningScript () {
    return new Error().stack.match(/([^ \n])*([a-z]*:\/\/\/?)*?[a-z0-9\/\\]*\.js/ig)[0]
}

fetchCalendar(calendarUrl)
    .then(data => {
        const events = readCalendar(data);
        const organizedEvents = organizeEvents(events);
        createCalendarImage(organizedEvents, outputImagePath);
    })
    .catch(error => {
        console.error('Error fetching or processing calendar:', error);
    });
