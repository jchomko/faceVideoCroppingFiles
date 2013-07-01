#include "testApp.h"

testApp::~testApp(){
	// finalize the movie and write it out to disk
	vidSaver.finishMovie();
}

//--------------------------------------------------------------
void testApp::setup(){
	img.loadImage("test.jpg");
	finder.setup("haarcascade_frontalface_alt3.xml");
   
    fileName = ofGetElapsedTimef();
    facePresentAvg.push_back(0);
    
    threshold = 20;
    
   // cam2Width = 640;
  //  cam2Height = 480;
    learnBackground = true;
    
  
    //createFileName();
    timeStamp = ofGetElapsedTimef();
    
    vidSaver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_LOSSLESS);
   
   
    facePositionAvgLength = 3;
    
    avgPos = ofVec3f(0,0,0);
    avgTotal = ofVec3f(0,0,0);
    
    dir.open("");
    
    cout<< dir.listDir();
    dir.size();
    files = dir.getFiles();

    
    for(int i = 0; i < dir.size(); i ++){
        if(dir.getFile(i).getExtension() == ".mov"){
            files.push_back(dir.getFile(i));
            cout << dir.getFile(i).getFileName();
        }
      
    }
    
    fileCounter = 0;
    
    if(files.size() > 0){
        vidPlayer.loadMovie(files[fileCounter].getFileName());
    }
    
    vidWidth  = vidPlayer.width;
    vidHeight = vidPlayer.height;
    
    
    colorImage.allocate( vidWidth, vidHeight);
    grayImage.allocate( vidWidth, vidHeight);
    backImage.allocate( vidWidth, vidHeight);
    
    averageSampleSize = 5;
    tex.allocate(vidWidth,vidHeight, GL_RGB);
    

}

//--------------------------------------------------------------
void testApp::update(){
   
    vidPlayer.getCurrentFrame();
    
  
    colorImage.setFromPixels(vidPlayer.getPixels(), vidWidth,vidHeight);
    grayImage = colorImage;
    finder.findHaarObjects(grayImage.getPixelsRef());
    
    
    
    
    //    contourFinder.findContours(grayImage, 20, (320*180)/3, 10,false);
        
//    if(contourFinder.nBlobs > 0){
//        facePresentAvg.push_back(1);
//       
//     }else{
//        facePresentAvg.push_back(0);
//       
//    }
//    
//    if(facePresentAvg.size() > 30){
//        facePresentAvg.erase(facePresentAvg.begin());
//    }
//    
//    
//    float fp = 0;
//    
//    for(int i = 0; i < facePresentAvg.size(); i ++){
//        fp += facePresentAvg[i];
//        
//    }
//    
//    fp = fp/facePresentAvg.size();
////        cout<< fp;
////        cout << ",";
//    
//    if(fp > 0.5){
//        facePresent = true;
//        
//    }else{
//        facePresent = false;
//    }
//    
//    //On recording start / face gone
//    if(facePresent && isRecording == false){
//        isRecording = true;
//        //get new file name
//        createFileName();
//        //get new timestamp
//        timeStamp = ofGetElapsedTimef();
//        //
//         movieStart = ofGetElapsedTimef();
//    
//        //setup recorder with new file name
//        vidSaver.setup(camWidth, camHeight, fileName);
//       
//        
//        
//    }
//    
//    //On recording end/ face gone
//    if(facePresent == false && isRecording == true){
//        isRecording = false;
//        //finalize movie
//        vidSaver.finishMovie();
//        learnBackground = true;
//    }
//    
//    
//    if(facePresent && isRecording){
//        float time = ofGetElapsedTimef() - timeStamp;
//        // vidSaver.addFrame(vidGrabber2.getPixels(),time);
//        vidSaver.addFrame(colorImage.getPixels(), 0.075);
//        timeStamp = ofGetElapsedTimef();
//        
//    }
//   }
//   
//    if(isRecording && ofGetElapsedTimef() - movieStart > 10){
//        movieStart = ofGetElapsedTimef();
//        learnBackground = true;
//        cout << "new backgroud";
//    }
    
    avgPos.set(0,0,0);
    avgTotal.set(0,0,0);
    
    if(facePositions.size() > facePositionAvgLength){
        facePositions.erase(facePositions.begin());
        
    }
    
    //cout <<facePositions.size();
    
    for(int i = 0 ; i < facePositions.size(); i ++){
        ofVec3f  v = facePositions[i];
         
         avgTotal.x += v.x;
         avgTotal.y += v.y;
         avgTotal.z += v.z;
        
    }
    
    
    avgPos.x = avgTotal.x/facePositions.size();
    avgPos.y = avgTotal.y/facePositions.size();
    avgPos.z = avgTotal.z/facePositions.size();
    //cout << avgPos.x;
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    grayImage.draw(0, 0);
	
    ofNoFill();
    ofVec3f v;
    //vidGrabber2.draw(camWidth,0,camWidth,camHeight);
    //ofxCvColorImage face;
    
    
	for(int i = 0; i < finder.blobs.size(); i++) {
        
		ofRectangle cur = finder.blobs[i].boundingRect;
        //face.allocate(cur.width,cur.height);
        v = ofVec3f(cur.x, cur.y, cur.height);
        //tex.loadScreenData(cur.x, cur.y,  cur.width, cur.height);
        tex.loadScreenData(avgPos.x, avgPos.y,  avgPos.z, avgPos.z);
        
		//ofRect(cur.x, cur.y, cur.width, cur.height);
	}
    
   
    tex.draw(0,0, 320, 320);
    ofSetColor(255, 255,255);
    ofRect(avgPos.x, avgPos.y,  avgPos.z, avgPos.z);
    if(v.x != 0){
    facePositions.push_back(v);
    }
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
    
    createFileName();
    
    //get new timestamp
    timeStamp = ofGetElapsedTimef();
    
    //setup recorder with new file name
    vidSaver.setup(vidWidth, vidHeight, fileName);
    
    fileCounter ++;
    
    if(files.size() > 0 && fileCounter < files.size()){
        
        vidPlayer.loadMovie(files[fileCounter].getFileName());
    
    }

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