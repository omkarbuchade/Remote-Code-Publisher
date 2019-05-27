var style;

function toggleDisplay(classname) {
    var elements = document.getElementsByClassName(classname);
    var i;
    for (i = 0; i < elements.length; i++)
        style = elements[i].style.display;

    if (style == "none") {
        for (i = 0; i < elements.length; i++)
            elements[i].style.display = "block";
        if (classname == "comments")
            document.getElementById(classname).innerHTML = "Hide Comments";
        else
            document.getElementById(classname).innerHTML = "-";
    }
    else {

        for (i = 0; i < elements.length; i++)
            elements[i].style.display = "none";

        if (classname == "comments")
            document.getElementById(classname).innerHTML = "Show Comments";
        else
            document.getElementById(classname).innerHTML = "+";
    }
}