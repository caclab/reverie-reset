#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(30);

	//Config file
	if( configXML.load("config.xml") ){
    	cout<< "Configuration Loaded!"<< endl;
    	screenResX = configXML.getValue<int>("screenResX");
    	screenResY = configXML.getValue<int>("screenResY");
    	totalScreens = configXML.getValue<int>("totalScreens");
    	testMode = configXML.getValue<bool>("testMode");
    	showGrid = configXML.getValue<bool>("showGrid");
    	sAddress = configXML.getValue<string>("sAddress");
    	sPort = configXML.getValue<int>("sPort");
    	sTimeout = configXML.getValue<int>("sTimeout");
	}

	//WEBSOCKETS CONFIG
	ofSetLogLevel(OF_LOG_VERBOSE);
	client.connect(sAddress, sPort);
	ofSetLogLevel(OF_LOG_ERROR);
	client.addListener(this);

	messageArrived = false;

	//socket timer (connection timeout)
	lastReconnectTime = ofGetElapsedTimeMillis();

	//Check if testmode is active, if so make window smaller scale
	//else if testmode is innactive, make the window real scale
	if(testMode){
		windowSizeX = (screenResX * totalScreens) / 10;
		windowSizeY = screenResY / 10;
	}else{
		windowSizeX = screenResX * totalScreens;
		windowSizeY = screenResY;
	}

	cout<< "Screen Size: " + ofToString(windowSizeX) +
		" x " + ofToString(windowSizeY)<< endl;

	//Set the new size to output window
	ofSetWindowShape(windowSizeX, windowSizeY);

}

//--------------------------------------------------------------
void ofApp::update(){
	imageRenderer.update();

	if(client.isConnected()){
		if(messageArrived){
			cout<< message<< endl;
			messageArrived = false;
		}
    }else{
        uint64_t now = ofGetElapsedTimeMillis();
        if (now - lastReconnectTime > sTimeout) {
            cout<< "Reconnecting..."<< endl;
            client.close();
            client.connect(sAddress, sPort); 
            lastReconnectTime = ofGetElapsedTimeMillis();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(showGrid){
    	ofSetColor(255);
    	ofNoFill();
    	for(int i=0; i<totalScreens; i++){
    		ofRect(i*(windowSizeX/totalScreens), 0, (windowSizeX/totalScreens), windowSizeY);
    	}
    }

  	imageRenderer.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == ' '){
		imageRenderer.setup(totalScreens,(windowSizeX/totalScreens), windowSizeY, (windowSizeX/totalScreens), 0);		
	}
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;

}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message: "<<args.message<<endl;
    messageArrived = true;
    message = args.message;
  
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;    
}

//--------------------------------------------------------------
void ofApp::exit(){
	imageRenderer.exit();
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
