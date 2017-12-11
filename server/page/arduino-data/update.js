var url = "https://api.thingspeak.com/channels/378145/feeds.json?api_key=GX90YAPHENFD80SJ&results=20";

var colorCode = ["#F3B2B2", "#F9F9A5", "#A5F6F9"];

$(document).ready(function() {
    (function worker() {
        $.ajax({
            url: url,
            dataType: 'json',
            type: 'get',
            cache: false,
            success: function(data) {
                // console.log(data);
                // console.log(convertTime(data.feeds[0].created_at));

                $(".time").each(function(i, obj) {
                    // Stop if there is no read left
                    if (i == data.feeds.length) return false;
                    try {
                        var j = data.feeds.length - i - 1;
                        var time = convertTime(data.feeds[j].created_at);
                        $(this).text(time);
                        var id = data.feeds[j].entry_id;
                        $(this).css("background-color", colorCode[id % 3]);
                    }
                    catch (e) {
                        console.log("Error retrieving time");
                    }
                });

                $(".temp").each(function(i, obj) {
                    // Stop if there is no read left
                    if (i == data.feeds.length) return false;
                    try {
                        var j = data.feeds.length - i - 1;
                        var temperature = data.feeds[j].field1 + String.fromCharCode(176);
                        $(this).text(temperature);
                        var id = data.feeds[j].entry_id;
                        $(this).css("background-color", colorCode[id % 3]);
                    }
                    catch (e) {
                        console.log("Error retrieving temperature");
                    }
                });

                $(".rh").each(function(i, obj) {
                    // Stop if there is no read left
                    if (i == data.feeds.length) return false;
                    try {
                        var j = data.feeds.length - i - 1;
                        var humidity = data.feeds[j].field2 + "%";
                        $(this).text(humidity);
                        var id = data.feeds[j].entry_id;
                        $(this).css("background-color", colorCode[id % 3]);
                    }
                    catch (e) {
                        console.log("Error retrieving humidity");
                    }
                });

                setTimeout(worker, 5000);
            }
        });
    })();
});

function convertTime(timeStr) {
    // Original format: yyyy-mm-dd<T>hh-mm-ss<Z>
    var yearStr = timeStr.substring(0, 4);
    var monthStr = timeStr.substring(5, 7);
    var dayStr = timeStr.substring(8, 10);
    var hourStr = timeStr.substring(11, 13);
    var minuteStr = timeStr.substring(14, 16);
    var secondStr = timeStr.substring(17, 19);

    // GMT to GMT+7
    var day = parseInt(dayStr);
    var hour = parseInt(hourStr);
    hour += 7;
    if (hour >= 17) {
        hour -= 24;
        day += 1;
    }

    hourStr = hour.toString();
    dayStr = day.toString();
    if (hourStr.length == 1) hourStr = "0" + hourStr;
    if (dayStr.length == 1) dayStr = "0" + dayStr;

    var result = dayStr + "-" + monthStr + "-" + yearStr + " ";
    result += hourStr + ":" + minuteStr + ":" + secondStr;
    return result;
}
