#include "testApp.h"

testApp::~testApp(){
	// finalize the movie and write it out to disk
	vidSaver.finishMovie();
}

//--------------------------------------------------------------
void testApp::setup(){
	img.loadImage("test.jpg");
	finder.setup("haarcascade_frontalface_alt3.xml");
    path = "/Users/fabrica197/Documents/of_v0073_osx_release/apps/faceVideoRecord/faceVideoCroppingFiles/bin/data/input";
    
    fileName = ofGetElapsedTimef();
    facePresentAvg.push_back(0);
    
    threshold = 20;
    
   // cam2Width = 640;
  //  cam2Height = 480;
    learnBackground = true;
    
  
    //createFileName();
    timeStamp = ofGetElapsedTimef();
    
    vidSaver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_LOSSLESS);
    
   
   
    
    avgPos = ofVec3f(0,0,0);
    avgTotal = ofVec3f(0,0,0);
    
    dir.open(path);
    
    cout <<  dir.listDir();
   
    cout <<  dir.size();
    
    //files = dir.getFiles();

    
    for(int i = 0; i < dir.size(); i ++){
        cout<<  dir.getFile(i).getExtension();
        if(dir.getFile(i).getExtension() == "mov"){
            files.push_back(dir.getFile(i));
            cout << dir.getFile(i).getFileName();
        }
      
    }
    cout << files.size();
    
    newMovie();
    
    
    
    
    colorImage.allocate( vidWidth, vidHeight);
    grayImage.allocate( vidWidth, vidHeight);
    backImage.allocate( vidWidth, vidHeight);
    
    fbo.allocate(vidWidth, vidHeight);
    screen.allocate(vidWidth, vidHeight, OF_IMAGE_COLOR);
    img.allocate(vidWidth, vidHeight, OF_IMAGE_COLOR);
    
    //averageSampleSize = 30;
    tex.allocate(vidWidth,vidHeight, GL_RGB);
    
    facePositionAvgLength = 20;
    
    m = 1.7;
}

//--------------------------------------------------------------
void testApp::update(){
   
    
    
    avgPos.set(0,0,0);
    avgTotal.set(0,0,0);
    
    if(facePositions.size() > facePositionAvgLength){
        facePositions.erase(facePositions.begin());
        
    }
    
    
    for(int i = 0 ; i < facePositions.size(); i ++){
        ofVec3f  v = facePositions[i];
        
        avgTotal.x += v.x;
        avgTotal.y += v.y;
        avgTotal.z += v.z;
        
    }
    
    
    avgPos.x = avgTotal.x/facePositions.size();
    avgPos.y = avgTotal.y/facePositions.size();
    avgPos.z = avgTotal.z/facePositions.size();
    
    
    
    
    
    vidPlayer.getCurrentFrame();
    colorImage.setFromPixels(vidPlayer.getPixels(), vidWidth,vidHeight);
    grayImage = colorImage;
    
    finder.findHaarObjects(grayImage.getPixelsRef());
    
    ofVec3f v;
    
    img.setFromPixels(vidPlayer.getPixels(), vidWidth, vidHeight, OF_IMAGE_COLOR);
    
    if( finder.blobs.size() > 0){
	for(int i = 0; i < finder.blobs.size(); i++) {
        
		ofRectangle cur = finder.blobs[i].boundingRect;
    
        v = ofVec3f(cur.x, cur.y, cur.height * m);
       
        screen.cropFrom(img, avgPos.x - (avgPos.z *0.33333333), avgPos.y -(avgPos.z * .16),  avgPos.z * 1.33, avgPos.z);
        
	}
    
    
    screen.resize(vidWidth, vidHeight);
    
    vidSaver.addFrame(screen.getPixels(), 0.03); // ofGetElapsedTimef()-timeStamp
    timeStamp = ofGetElapsedTimef();
    
    if(v.x != 0){
      facePositions.push_back(v);
        }
    
    }
    
    bool isMovieDone = vidPlayer.getIsMovieDone();
    
    if(!isMovieDone){
        vidPlayer.nextFrame();
    }else{
        cout<< "movie done";
        if(vidSaver.bAmSetupForRecording()){
        vidSaver.finishMovie();
        }
        
        newMovie();
        
    }
    
    
    //cout << avgPos.x;
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    screen.draw(0, 0);
    
	
    ofNoFill();
   
       if(facePresent){
        ofPushStyle();
        //ofFill();
        ofSetColor(255, 0, 0);
        ofEllipse(50, 50, 50, 50);
        ofPopStyle();
    }
     
}

void testApp::createFileName(void){
    ostringstream oss;
    oss << ofGetYear() << "-";
	oss << setw(2) << setfill('0') << ofGetMonth() << "-";
	oss << setw(2) << setfill('0') << ofGetDay() << "-";
	oss << setw(2) << setfill('0') << ofGetHours() << "-";
	oss << setw(2) << setfill('0') << ofGetMinutes() << "-";
	oss << setw(2) << setfill('0') << ofGetSeconds() << ".mov";
    fileName = oss.str();
    

}

void testApp::newMovie(){
    
    
    
    if(files.size() > 0 && fileCounter < files.size()){
        //cout << "file counter";
        cout << dir.getPath(fileCounter); //+  files[fileCounter].getFileName();
        vidPlayer.loadMovie(dir.getPath(fileCounter));
    
    }
    
    vidWidth  = vidPlayer.width;
    vidHeight = vidPlayer.height;
    
    if(fileCounter < files.size()){
        fileCounter ++;
    }else{
        std::exit(1);
    }
    
    
    
    createFileName();
    
    //get new timestamp
    timeStamp = ofGetElapsedTimef();
    
    //setup recorder with new file name
    vidSaver.setup(vidWidth, vidHeight, fileName);
    
    facePositions.clear();
    
}




//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case ' ':
            learnBackground = true;
            break;
        case OF_KEY_UP:
            threshold += 1;
            break;
        case OF_KEY_DOWN:
            threshold -= 1;
            break;
            
       
    }
       
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}