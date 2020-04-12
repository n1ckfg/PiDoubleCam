"use strict";

var liveView;

function main() {

    liveView = document.getElementById("live_view");
    liveView.onclick = saveAsPng(liveView, "test.png");

}

window.onload = main;

function encodeAsPng(img) {
    var c = document.createElement('canvas');
    c.width = img.width;
    c.height = img.height;
    var ctx = c.getContext('2d');
    ctx.drawImage(img, 0, 0);
    return c.toDataURL('image/png');
}

function saveAsPng(img, fileName) {
    var imgBase64 = encodeAsPng(img);

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
