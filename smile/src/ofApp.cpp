#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    //ofSetDataPathRoot("./data/");
    ofSetVerticalSync(true);
    //setup camera feed
    cam.initGrabber(640, 480);
    //setup face tracker
    tracker.setup();
    tracker.setRescale(.5);
    //load smile recognition data
    classifier.load("expressions");
    for(int i = 0; i < classifier.size(); i++){
        if(classifier.getDescription(i) == "smile") smileID=i;
        if(classifier.getDescription(i) == "eyebrows") eyebrowsID=i;
    }
    threshold = 0.9;
    //load images
    
    // find all the pics and load them
    ofDirectory dir;
    int n = dir.listDir("pics");
    for (int i=0; i<n; i++) {
        horriblePics.push_back(ofImage(dir.getPath(i)));
    }
    
    //load video
    myVideo.load("movies/video.mp4");
    myVideo.setLoopState(OF_LOOP_NORMAL);
    myVideo.play();
    
    //load tone
    tone.load("sounds/tone.mp3");

    
//    for(int i = 1; i < 4; i++){
//        //load images in image vector from URL
//        ofImage img;
//        string imgName = ofToString(i) + ".jpg";
//        img.loadImage(imgName);
//        horriblePics.push_back(img);
//    }
    index = 0;
    //bools
    smile = false;
    start = true;
}
//------------------------------------------------------------
void ofApp::update() {
    cam.update();
    if(cam.isFrameNew()) {
        if(tracker.update(toCv(cam))) {
            classifier.classify(tracker);
        }
    }
    //update video
    myVideo.update();
    // update the sound playing system:
    ofSoundUpdate();
}
//------------------------------------------------------------
void ofApp::draw() {
    
    ofSetColor(255);
    posx =  ofGetWidth()/2 - (cam.getWidth());
    posy = ofGetHeight()/2 - (cam.getHeight()/2);
    
    
    
    //loading screen
    if(!start){
        
        cam.draw(posx,posy);
        
        ofPushMatrix();
        ofTranslate(posx, posy);
        tracker.draw();
        ofPopMatrix();
        
        
//        //detect face or not
//        if(tracker.getFound()){
//            ofSetColor(250, 50, 50, 100);
//            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
//            ofDrawBitmapStringHighlight("smile "+ofToString(3-smileStep)+" times to begin\nthe LUDOVICO TREATMENT", ofGetWidth()/2-60, ofGetHeight()/2-10);
//            
//        }
//        else{
//            ofSetColor(50, 50, 50, 220);
//            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
//            ofDrawBitmapStringHighlight("searching for face...", (ofGetWidth()/2) - (cam.getWidth()/2) - 10, ofGetHeight()/2-10);
//        }
//        
//        //trigger start with smile 
//        if(classifier.getProbability(smileID) > 0.9 /*&& classifier.getProbability(eyebrowsID) > 0.1*/ && (tracker.getFound())){
//            smile = true;
//            oldSmile = true;
//            
//            ofSetColor(250, 50, 50, 200);
//            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
// ofDrawBitmapStringHighlight("smile "+ofToString(3-smileStep)+" times to begin\nthe LUDOVICO TREATMENT", ofGetWidth()/2-60, ofGetHeight()/2-10);
//            
//            //            ofSetColor(250, 50, 50, 255);
//            //            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
//            //            ofDrawBitmapStringHighlight("START!", ofGetWidth()/2-10, ofGetHeight()/2-10);
//            //            ofSleepMillis(5000);
//        }
//        else {
//            smile = false;
//        }
//        
//        if(oldSmile == true and smile == false){
//            smileStep++;
//            oldSmile = false;
//            //ofSleepMillis(500);
//        }
//        if(smileStep ==3)start = true;
//        
//        
////        cout<<"smile = "<<classifier.getProbability(smileID)<<endl;
////        cout<<"eyebrows = "<<classifier.getProbability(eyebrowsID)<<endl;
        }
    
    
    
    
    //we begin the treatment
    else{
        
        //IF SMILE
        //cout<<classifier.getProbability(smileID)<<endl;
        if(classifier.getProbability(smileID) > threshold && (tracker.getFound())){
            
            //stop video
            if(myVideo.isPlaying())  myVideo.stop();
            //play tone
            if(!tone.isPlaying()) tone.play();

            
            //draw camera
            cam.draw(posx,posy);
            //draw tracker
            ofPushMatrix();
            ofTranslate(posx, posy);
            tracker.draw();
            ofPopMatrix();
            
            //draw image
            posImgx = ofGetWidth()/2 ;
            posImgy = ofGetHeight()/2 - (horriblePics[index].getHeight()/2);

            horriblePics[index].draw(posImgx, posImgy);
            smile = true;
        }
        
        //OTHERWISE SHOW THE CAMERA AND ASK FOR A SMILE
        else{
            //play video
            if(!myVideo.isPlaying())  myVideo.play();
        //stop tone sound
            if(tone.isPlaying()) tone.stop();
            
            //draw camera
            cam.draw(posx,posy);
            
            
            //draw video
            posImgx = ofGetWidth()/2 ;
            posImgy = ofGetHeight()/2 - (myVideo.getHeight()/2);
            
            myVideo.draw(posImgx,posImgy);
            
            ofPushMatrix();
            ofTranslate(posx, posy);
            tracker.draw();
            ofPopMatrix();
            
            if(smile){
                if(index >= horriblePics.size()-1)index=0;
                else index++;
                smile = false;
            }
            //draw text "smile!"
            if(tracker.getFound()){
                //ofDrawBitmapStringHighlight("SMILE!", (ofGetWidth()/2) - (cam.getWidth()/2) - 10, ofGetHeight()/2);
            }
            else{
                ofSetColor(50, 50, 50, 200);
                //ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
                ofDrawBitmapStringHighlight("searching for face...", (ofGetWidth()/2) - (cam.getWidth()/2) - 10, ofGetHeight()/2-10);
            }
        }
        
        
        /////////////////////////////
        int w = 100, h = 12;
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(5, 10);
        int n = classifier.size();
        int primary = classifier.getPrimaryExpression();
        
        for(int i = 0; i < n; i++){
            if(classifier.getDescription(i) == "smile"){
                ofSetColor(i == primary ? ofColor::red : ofColor::black);
                ofDrawRectangle(0, 0, w * classifier.getProbability(i) + .5, h);
                ofSetColor(255);
                ofDrawBitmapString(classifier.getDescription(i), 5, 9);
                ofTranslate(0, h + 5);
            }
        }
        ofPopMatrix();
        ofPopStyle();
    }
    //	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 20, ofGetHeight() - 10);
    //	ofDrawBitmapStringHighlight(
    //		string() +
    //		"r - reset\n" +
    //		"e - add expression\n" +
    //		"a - add sample\n" +
    //		"s - save expressions\n"
    //		"l - load expressions",
    //		14, ofGetHeight() - 7 * 12);
}

void ofApp::keyPressed(int key) {
    //	if(key == 'r') {
    //		tracker.reset();
    //		classifier.reset();
    //	}
    //	if(key == 'e') {
    //		classifier.addExpression();
    //	}
    //	if(key == 'a') {
    //		classifier.addSample(tracker);
    //	}
    //	if(key == 's') {
    //		classifier.save("expressions");
    //	}
    //	if(key == 'l') {
    //		classifier.load("expressions");
    //	}
}
