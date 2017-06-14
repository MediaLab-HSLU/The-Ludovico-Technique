#pragma once

#include "ofMain.h"
#include "ofxFaceTracker.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
	ExpressionClassifier classifier;
    
    int smileID, eyebrowsID;
    float threshold;
    int index;
    
    vector <ofImage> horriblePics;
    bool smile, oldSmile;
    bool start;
    int smileStep;
    
    int posImgx, posImgy;
    int posx, posy;
    
    ofVideoPlayer myVideo;
    ofSoundPlayer tone;
};
