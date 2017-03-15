#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// list monitors
	int count;
	const auto monitors = glfwGetMonitors(&count);
	int x = 0, y = 0;
	int widthMM = 0, heightMM = 0;
	
	for (int i = 0; i<count; i++) {
		auto monitor = monitors[i];
		glfwGetMonitorPos(monitor, &x, &y);
		glfwGetMonitorPhysicalSize(monitor, &widthMM, &heightMM);
		auto mode = glfwGetVideoMode(monitor);
		ofLogNotice() << i << ": " << glfwGetMonitorName(monitor) <<
		" at " << x << ", " << y <<
		", physical: " << widthMM << ", " << heightMM <<
		", size: " << mode->width << ", " << mode->height;
	}
	
	auto primaryMonitor = glfwGetPrimaryMonitor();
	glfwGetMonitorPos(primaryMonitor, &x, &y);
	glfwGetMonitorPhysicalSize(primaryMonitor, &widthMM, &heightMM);
	ofLogNotice() << "primary monitor: " << glfwGetMonitorName(primaryMonitor);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
