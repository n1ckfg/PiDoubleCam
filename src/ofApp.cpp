#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup() {
    settings.loadFile("settings.xml");
    ofHideCursor();
    debug = (bool) settings.getValue("settings:debug", 1);

    camWidth = settings.getValue("settings:width", 320);
    camHeight = settings.getValue("settings:height", 240);

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
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);

    ofSetVerticalSync(true);

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
    streamSettings.ipVideoRouteSettings.setMaxStreamWidth(camWidth); // default 1920
    streamSettings.ipVideoRouteSettings.setMaxStreamHeight(camHeight); // default 1080
    streamSettings.fileSystemRouteSettings.setDefaultIndex("index.html");
    streamServer.setup(streamSettings);
    streamServer.start();
}


//--------------------------------------------------------------
void ofApp::update() {
    ofBackground(100, 100, 100);
    vidGrabber.update();

    if (vidGrabber.isFrameNew()) {
        //
    }

    // ~ ~ ~ ~

    frame = cam.grab();

    if (!frame.empty()) {
        //
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    fbo.draw(0, 0, fboScaleW, fboScaleH);
}

void ofApp::updateStreamingVideo() {
    fbo.begin();
    vidGrabber.draw(0, 0);
    drawMat(frame, camWidth, 0);
    fbo.end();

    fbo.readToPixels(pixels);
    streamServer.send(pixels);
}