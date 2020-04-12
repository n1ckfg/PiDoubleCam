"use strict";

var liveView;

function loadFile(filepath, callback) { 
    // https://codepen.io/KryptoniteDove/post/load-json-file-locally-using-pure-javascript  
    var xobj = new XMLHttpRequest();
    xobj.overrideMimeType("text/plain");
    xobj.open('GET', filepath, true);
    xobj.onreadystatechange = function() {
        if (xobj.readyState == 4 && xobj.status == "200") {
            callback(xobj.responseText);
        }
    };
    xobj.send(null);  
}

function main() {

    liveView = document.getElementById("live_view");
    liveView.download = "test.jpg";

}

window.onload = main;
