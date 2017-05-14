#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	initConfig();
	
	mLayer = std::make_shared<Layer>();
	mLayer->setup(WINDOW_NUM, glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();
	options.host = pHost;
	options.port = pSocketPort;
	
	mClient.connect(options);
	mClient.addListener(this);
	ofSetLogLevel(OF_LOG_NOTICE);

	ofAddListener(mLayer->mReadyEvent, this, &ofApp::onReady);
	ofAddListener(mLayer->mStandbyEvent, this, &ofApp::onStandby);
	
	mLastReconnectTime = ofGetElapsedTimef();
	
#ifdef _WIN32
	ShowCursor(false);
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
	mLayer->update();
	
	if (ofGetElapsedTimef() - mLastReconnectTime >= 5) {
		if (!mClient.isConnected()) {
			ofSetLogLevel(OF_LOG_VERBOSE);
			ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();
			options.host = pHost;
			options.port = pSocketPort;
			
			mClient.close();
			mClient.connect(options);
			ofSetLogLevel(OF_LOG_NOTICE);
			ofLogNotice("Client") << "Server unreachable, Reconnecting..";
		}
		
		mLastReconnectTime = ofGetElapsedTimef();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	mLayer->draw(0);
}

void ofApp::drawWindow01(ofEventArgs & args) {
	ofBackground(15);
	mLayer->draw(1);
}

void ofApp::drawWindow02(ofEventArgs & args) {
	ofBackground(30);
	mLayer->draw(2);
}

void ofApp::drawWindow03(ofEventArgs & args) {
	ofBackground(45);
	mLayer->draw(3);
}

void ofApp::drawWindow04(ofEventArgs & args) {
	ofBackground(60);
	mLayer->draw(4);
}

void ofApp::drawWindow05(ofEventArgs & args) {
	ofBackground(75);
	mLayer->draw(5);
}

void ofApp::drawWindow06(ofEventArgs & args) {
	ofBackground(90);
	mLayer->draw(6);
}

void ofApp::drawWindow07(ofEventArgs & args) {
	ofBackground(105);
	mLayer->draw(7);
}

void ofApp::drawWindow08(ofEventArgs & args) {
	ofBackground(120);
	mLayer->draw(8);
}

void ofApp::drawWindow09(ofEventArgs & args) {
	ofBackground(135);
	mLayer->draw(9);
}

void ofApp::drawWindow10(ofEventArgs & args) {
	ofBackground(150);
	mLayer->draw(10);
}

void ofApp::drawWindow11(ofEventArgs & args) {
	ofBackground(165);
	mLayer->draw(11);
}

void ofApp::drawWindow12(ofEventArgs & args) {
	ofBackground(180);
	mLayer->draw(12);
}

void ofApp::drawWindow13(ofEventArgs & args) {
	ofBackground(195);
	mLayer->draw(13);
}

void ofApp::drawWindow14(ofEventArgs & args) {
	ofBackground(210);
	mLayer->draw(14);
}

void ofApp::drawWindow15(ofEventArgs & args) {
	ofBackground(225);
	mLayer->draw(15);
}

void ofApp::exit() {
	ofRemoveListener(mLayer->mReadyEvent, this, &ofApp::onReady);
	ofRemoveListener(mLayer->mStandbyEvent, this, &ofApp::onStandby);
}

void ofApp::sendMessage(const std::string& msg) {
	mClient.send("c" + pId.get() + "$" + msg);
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
	cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
	cout<<"on open"<<endl;
	sendMessage("ready");
	ofLogNotice("Client") << "sending ready";
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
	if (args.message == "BOOM") {
		ofLogNotice("Client") << "BOOM\n";
		mLayer->boom();
	} else {
		auto visJson = ofJson::parse(args.message);
		try {
			auto cJson = ofJson::parse(visJson.at(pId.get()).dump());
			
			int index = 0;
			ImageMetaBundle metaBundle;
			
			for (auto& meta : cJson) {
				ofLogNotice("Client") << index++ << ": ";
				auto metaJson = ofJson::parse(meta.dump());
				
				std::string text = metaJson.at("caption");
				std::string id = metaJson.at("image_id");
				
				ofLogNotice("Client") << "\tcaption:" << text;
				ofLogNotice("Client") << "\timage_id:" << id;
				
				std::string path = "http://" + pHost.get() + ":" +
				ofToString(pHttpPort.get()) + "/imgs/captioned/" + id + ".jpg";
				
				ofLogNotice("Client") << "\tpath:" << path;
				
				metaBundle.add(path, text, true);
			}
			
			if (index == 1) {
				mLayer->loadNew(metaBundle);
			} else if (index > 1) {
				mLayer->loadRandom(metaBundle);
			} else {
			
			}
		} catch (std::domain_error& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
	cout<<"got broadcast "<<args.message<<endl;
}

// Layer event
void ofApp::onReady() {
	ofLogNotice("Client") << "layer ready";
	sendMessage("ready");
	ofLogNotice("Client") << "sending ready";
}

void ofApp::onStandby() {
	ofLogNotice("Client") << "layer standby";
	sendMessage("standby");
	ofLogNotice("Client") << "sending standby";
}

// deserialize
void ofApp::initConfig() {
	pParameters.setName("Network");
	pParameters.add(pHost.set("Host", "192.168.1.150"));
	pParameters.add(pSocketPort.set("Socket Port", 8001));
	pParameters.add(pHttpPort.set("HTTP Port", 8080));
	pParameters.add(pId.set("ID", "1"));
	
	ofXml xml;
	if (xml.load("settings/network.xml")) {
		ofDeserialize(xml, pParameters);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		default:
			break;
	}
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
