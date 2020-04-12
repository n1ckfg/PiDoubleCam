"use strict";

var liveView;
var hostname="test";

function main() {

    loadFile("./js/hostname", function(evt) {
        hostname = evt.replace(/^\n|\n$/g, ''); // remove line breaks
    });

    liveView = document.getElementById("live_view");
    liveView.onclick = saveToPng;

}

window.onload = main;

function saveToPng() {
    saveBase64Image(liveView, hostname + ".png");
}

function encodeBase64Image(img, format) {
    var c = document.createElement('canvas');
    c.width = img.width;
    c.height = img.height;
    var ctx = c.getContext('2d');
    ctx.drawImage(img, 0, 0);
    return c.toDataURL('image/' + format);
}

function saveBase64Image(img, fileName) {
    var extensionTemp = fileName.split('.');
    var format = extensionTemp[extensionTemp.length - 1];

    var imgBase64 = encodeBase64Image(img, format);

    var a = document.createElement("a"); //Create <a>
    a.href = imgBase64; //Image Base64 Goes here
    a.download = fileName; //File name Here
    a.click(); //Downloaded file
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
