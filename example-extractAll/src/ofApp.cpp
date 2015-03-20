#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    zip.open("archive.zip");
    bool success = zip.unzipTo("");
  
    
    if(!success) {
        ofLogNotice() << "Couldn't open zipfile!";
        ofExit();
    }
    
    dir.listDir("archive");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    int y = 40;
    for (int i=0; i<dir.numFiles(); i++) {
        
        ofDrawBitmapString(dir.getName(i), 20, y);
        y += 20;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
