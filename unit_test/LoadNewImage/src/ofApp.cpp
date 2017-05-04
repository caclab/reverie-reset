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
}

//--------------------------------------------------------------
void ofApp::update(){
	mLayer->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	mLayer->draw(0);

	ofSetColor(255, 255, 0);
	ofDrawCircle(20, 20, 20 * (std::sinf(ofGetElapsedTimef() * 3) * 0.5f + 0.5f));
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 0, 60);
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
	std::cout << "sending ready" << std::endl;
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
		std::cout << "\nBOOM\n" << std::endl;
		mLayer->boom();
	} else {
		auto visJson = ofJson::parse(args.message);
		try {
			auto cJson = ofJson::parse(visJson.at(pId.get()).dump());
			
			int index = 0;
			ImageMetaBundle metaBundle;
			
			for (auto& meta : cJson) {
				std::cout << index++ << ": " << std::endl;
				auto metaJson = ofJson::parse(meta.dump());
				
				std::string text = metaJson.at("caption");
				std::string id = metaJson.at("image_id");
				
				std::cout << "\tcaption:" << text << std::endl;
				std::cout << "\timage_id:" << id << std::endl;
				
				std::string path = "http://" + pHost.get() + ":" +
				ofToString(pHttpPort.get()) + "/imgs/captioned/" + id + ".jpg";
				
				std::cout << "\tpath:" << path << std::endl;
				
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
	std::cout << "layer ready" << std::endl;
	sendMessage("ready");
	std::cout << "sending ready" << std::endl;
}

void ofApp::onStandby() {
	std::cout << "layer standby" << std::endl;
	sendMessage("standby");
	std::cout << "sending standby" << std::endl;
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
		case 'a':
		{
			ImageMetaBundle metaBundle;
			metaBundle.add("images/aurora.jpg", "aurora in the sky", false);
			metaBundle.add("images/boat.jpg", "fishing boat on beach", false);
//			metaBundle.add("images/bridge.jpg", "a bridge, that's huge", false);
//			metaBundle.add("images/city.jpg", "city skyline on river bank", false);
//			metaBundle.add("images/desert.jpg", "sunset of desert", false);
//			metaBundle.add("images/eagleview.jpg", "a town in eagles eye", false);
//			metaBundle.add("images/grass.jpg", "forrest wake up in spring", false);
//			metaBundle.add("images/island.jpg", "abandoned islands", false);
//			metaBundle.add("images/lake.jpg", "lake surrounded by moutains", false);
//			metaBundle.add("images/road.jpg", "road with dim light", false);
//			metaBundle.add("images/sunset.jpg", "chilly sunset", false);
//			metaBundle.add("images/swamp.jpg", "swamp looks like mirror", false);
			
			mLayer->loadRandom(metaBundle);
		}
			break;
		case 'z':
		{
			ImageMetaBundle metaBundle;
			metaBundle.add("images/bridge.jpg", "a bridge, that's huge", false);
			metaBundle.add("images/city.jpg", "city skyline on river bank", false);
			
			mLayer->loadRandom(metaBundle);
		}
			break;
		case 's':
		{
			ImageMetaBundle metaBundle;
			metaBundle.add("images/desert.jpg", "sunset of desert", false);
			metaBundle.add("images/eagleview.jpg", "a town in eagles eye", false);
			
			mLayer->loadNew(metaBundle);
		}
			break;
		case 'x':
		{
			ImageMetaBundle metaBundle;
			metaBundle.add("images/grass.jpg", "forrest wake up in spring", false);
			metaBundle.add("images/island.jpg", "abandoned islands", false);
			
			mLayer->loadNew(metaBundle);
		}
			break;
		case 'b':
			mLayer->boom();
			break;
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