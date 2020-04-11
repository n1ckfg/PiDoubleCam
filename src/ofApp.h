#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxCvPiCam.h"
#include "ofxOMXPlayer.h"
#include "ofxXmlSettings.h"

// This openFrameworks example is designed to demonstrate how to access the webcam.
// For more information regarding this example take a look at the README.md.

class ofApp : public ofBaseApp{

    public:

        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);        

        ofxXmlSettings settings;

        ofVideoGrabber vidGrabber;
        ofPixels videoInverted;
        ofTexture videoTexture;
        int camWidth;
        int camHeight;

        ofxCvPiCam cam;
        cv::Mat frame;

        // for more camera settings, see:
        // https://github.com/orgicus/ofxCvPiCam/blob/master/example-ofxCvPiCam-allSettings/src/testApp.cpp

        int camShutterSpeed; // 0 to 330000 in microseconds, default 0
        int camSharpness; // -100 to 100, default 0
        int camContrast; // -100 to 100, default 0
        int camBrightness; // 0 to 100, default 50
        int camIso; // 100 to 800, default 300
        int camExposureCompensation; // -10 to 10, default 0;

        // 0 off, 1 auto, 2 night, 3 night preview, 4 backlight, 5 spotlight, 6 sports, 7, snow, 8 beach, 9 very long, 10 fixed fps, 11 antishake, 12 fireworks, 13 max
        int camExposureMode; // 0 to 13, default 0
};
