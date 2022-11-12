//var user = {
//    title: "",
//    alias: "",
//    mode: "",
//    course: ""
//};

function loadSchedule() {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200 && xmlhttp.responseText) {
            fillingSchedule(xmlhttp.responseText);
        }
    }
    xmlhttp.open("POST", "schedule", true);
    xmlhttp.send("schedule=load");
}
function fillingSchedule(response) {
    var groups = JSON.parse(response);

    var out = "";
    groups.forEach(function (group) {

        //user.title = group.title;
        //user.alias = group.alias;
        //user.mode = group.mode;
        //user.course = group.course;
        group.tasks.weeks.forEach(function (week) {
            out += '<div class="small-12 medium-6" id="week'+week.number+'">';
            out += '<h2 class="text-week">' + week.title + '</h2>';
            out += '<div class="panel panel-info">';
            week.days.forEach(function (day) {
                out += '<div class="panel-heading">';
                out += '<h3 class="panel-title">' + day.title + '</h3>';
                out += '</div>';
                out += '<div class="panel-body lesson"><ol>';
                day.lessons.forEach(function (lesson) {
                    out += '<li value="' + lesson.count + '" ><div class="text-lesson-room">';
                    if (lesson.title) out += '<div class="text-title">' + lesson.title + '</div>';
                    if (lesson.type) out += '<div class="text-lecture text-lesson-item">' + lesson.type + '</div>';
                    if (lesson.teachers) out += '<div class="text-teachers text-lesson-item">' + lesson.teachers + '</div>';
                    if (lesson.room) out += '<div class="text-room text-lesson-item">' + lesson.room + '</div>';
                    out += '</div></li>';
                })
                out += '</ol></div>';
            })
            out += '</div>';
            out += '</div>';
        })
    })
    document.getElementById("schedule-load").innerHTML = out;
}
