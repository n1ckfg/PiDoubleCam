"use strict";

var liveView;

function saveImage() {
    saveBase64Image(liveView, "test.jpg");
}

function saveImageStereo() {
    saveBase64ImageStereo(liveView, "sbs", "test.jpg");
}

function main() {

    liveView = document.getElementById("live_view");
    liveView.onclick = saveImageStereo;

}

window.onload = main;

