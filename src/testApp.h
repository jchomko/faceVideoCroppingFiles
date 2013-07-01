#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxQtVideoSaver.h"

class testApp : public ofBaseApp{
	public:
    
        ~testApp();
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void createFileName(void);
        void newMovie();
    
		ofImage img;
		ofxCvHaarFinder finder;
    
        ofxQtVideoSaver vidSaver;
        bool facePresent;
        vector<int> facePresentAvg;
        vector<ofVec3f> facePositions;
        ofVec3f avgPos;
        ofVec3f avgTotal;
        int facePositionAvgLength;
        string fileName;
        float timeStamp;
        bool isRecording;
        int averageSampleSize;
    
    
        ofxCvGrayscaleImage grayImage;
        ofxCvGrayscaleImage backImage;
    
        bool learnBackground;
        ofxCvColorImage colorImage;
        ofxCvContourFinder contourFinder;
        int threshold;
        ofVideoPlayer vidPlayer;
        ofDirectory dir;
        ofFbo fbo;
        vector<ofFile> files;
        int fileCounter;
    
        ofPixels scaledPix;
        ofPixels videoPix;
        ofImage screen;
        ofTexture tex;
        unsigned char * videoPixels;
        int vidWidth;
        int vidHeight;
        float m;
    
    string path;
       };


