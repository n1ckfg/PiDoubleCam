"use strict";

var liveView, liveViewDl;

function main() {

    liveView = document.getElementById("live_view");
    liveViewDl = document.getElementById("live_view_dl");
    var img = saveAsPng(liveView);
    console.log(img);
    liveViewDl.onclick = loadFile(img, function(evt) { });

}

window.onload = main;

function saveAsPng(img) {
    var c = document.createElement('canvas');
    c.width = img.width;
    c.height = img.height;
    var ctx = c.getContext('2d');
    ctx.drawImage(img, 0, 0);
    return c.toDataURL('image/png');
}

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
