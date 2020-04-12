#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup() {
    settings.loadFile("settings.xml");

    debug = (bool) settings.getValue("settings:debug", 1);
    camWidth = settings.getValue("settings:width", 320);
    camHeight = settings.getValue("settings:height", 240);
    framerate = settings.getValue("settings:framerate", 60);

    ofSetFrameRate(framerate);
    ofSetVerticalSync(true);
    ofHideCursor();

    //get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for (size_t i = 0; i < devices.size(); i++) {
        if (devices[i].bAvailable) {
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        } else {
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(framerate);
    vidGrabber.initGrabber(camWidth, camHeight);

    // ~ ~ ~   rpi cam settings   ~ ~ ~
    camSharpness = settings.getValue("settings:sharpness", 0);
    camContrast = settings.getValue("settings:contrast", 0);
    camBrightness = settings.getValue("settings:brightness", 50);
    camIso = settings.getValue("settings:iso", 300);
    camExposureMode = settings.getValue("settings:exposure_mode", 0);
    camExposureCompensation = settings.getValue("settings:exposure_compensation", 0);
    camShutterSpeed = settings.getValue("settings:shutter_speed", 0);

    cam.setSharpness(camSharpness);
    cam.setContrast(camContrast);
    cam.setBrightness(camBrightness);
    cam.setISO(camIso);
    cam.setExposureMode((MMAL_PARAM_EXPOSUREMODE_T)camExposureMode);
    cam.setExposureCompensation(camExposureCompensation);
    cam.setShutterSpeed(camShutterSpeed);

    cam.setup(camWidth, camHeight, false); // color/gray;

    // * stream video *
    // https://github.com/bakercp/ofxHTTP/blob/master/libs/ofxHTTP/include/ofx/HTTP/IPVideoRoute.h
    // https://github.com/bakercp/ofxHTTP/blob/master/libs/ofxHTTP/src/IPVideoRoute.cpp
    fbo.allocate(camWidth*2, camHeight, GL_RGBA);
    pixels.allocate(camWidth*2, camHeight, OF_IMAGE_COLOR);
    fboScaleW = ofGetWidth();
    fboScaleH = int(((float) ofGetWidth() / (float) fbo.getWidth()) * (float) ofGetHeight());
    fboPosX = 0;
    fboPosY = abs((ofGetHeight() - fboScaleH))/2;
   
    streamPort = settings.getValue("settings:stream_port", 7111);
    streamSettings.setPort(streamPort);
    streamSettings.ipVideoRouteSettings.setMaxClientConnections(settings.getValue("settings:max_stream_connections", 5)); // default 5
    streamSettings.ipVideoRouteSettings.setMaxClientBitRate(settings.getValue("settings:max_stream_bitrate", 512)); // default 1024
    streamSettings.ipVideoRouteSettings.setMaxClientFrameRate(settings.getValue("settings:max_stream_framerate", 30)); // default 30
    streamSettings.ipVideoRouteSettings.setMaxClientQueueSize(settings.getValue("settings:max_stream_queue", 10)); // default 10
    streamSettings.ipVideoRouteSettings.setMaxStreamWidth(camWidth*2); // default 1920
    streamSettings.ipVideoRouteSettings.setMaxStreamHeight(camHeight); // default 1080
    streamSettings.fileSystemRouteSettings.setDefaultIndex("index.html");
    streamServer.setup(streamSettings);
    streamServer.start();

    cam1Ready = false;
    cam2Ready = false;
}


//--------------------------------------------------------------
void ofApp::update() {
    vidGrabber.update();
    if (vidGrabber.isFrameNew()) cam1Ready = true;

    frame = cam.grab();
    if (!frame.empty()) cam2Ready = true;

    if (cam1Ready && cam2Ready) {
        updateStreamingVideo();
        cam1Ready = false;
        cam2Ready = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (debug) {
        ofBackground(0);
        fbo.draw(0, 0, fboScaleW, fboScaleH);
    }
}

void ofApp::updateStreamingVideo() {
    fbo.begin();
    vidGrabber.draw(0, 0);
    drawMat(frame, camWidth, 0);
    fbo.end();

    fbo.readToPixels(pixels);
    streamServer.send(pixels);
}