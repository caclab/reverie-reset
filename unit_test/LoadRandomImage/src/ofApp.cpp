#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	mLayer = std::make_shared<Layer>();
	mLayer->setup(WINDOW_NUM, glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();
	options.host = "192.168.1.190";
	options.port = 8001;
	
	mClient.connect(options);
	mClient.addListener(this);
	ofSetLogLevel(OF_LOG_NOTICE);
}

//--------------------------------------------------------------
void ofApp::update(){
	mLayer->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 0, 255);

	mLayer->draw(0);

	ofSetColor(255, 255, 0);
	ofDrawCircle(20, 20, 20 * (std::sinf(ofGetElapsedTimef() * 3) * 0.5f + 0.5f));
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 0, 60);
}

void ofApp::drawWindow01(ofEventArgs & args) {
	ofBackground(20);
	mLayer->draw(1);
}

void ofApp::drawWindow02(ofEventArgs & args) {
	ofBackground(40);
	mLayer->draw(2);
}

void ofApp::drawWindow03(ofEventArgs & args) {
	ofBackground(60);
	mLayer->draw(3);
}

void ofApp::drawWindow04(ofEventArgs & args) {
	ofBackground(80);
	mLayer->draw(4);
}

void ofApp::drawWindow05(ofEventArgs & args) {
	ofBackground(100);
	mLayer->draw(5);
}

void ofApp::drawWindow06(ofEventArgs & args) {
	ofBackground(120);
	mLayer->draw(6);
}

void ofApp::drawWindow07(ofEventArgs & args) {
	ofBackground(140);
	mLayer->draw(7);
}

void ofApp::drawWindow08(ofEventArgs & args) {
	ofBackground(160);
	mLayer->draw(8);
}

void ofApp::drawWindow09(ofEventArgs & args) {
	ofBackground(180);
	mLayer->draw(9);
}

void ofApp::drawWindow10(ofEventArgs & args) {
	ofBackground(200);
	mLayer->draw(10);
}

void ofApp::drawWindow11(ofEventArgs & args) {
	ofBackground(220);
	mLayer->draw(11);
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
	cout<<"on connected"<<endl;
	mClient.send("c1$ready");
	std::cout << "sending ready" << std::endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
	cout<<"on open"<<endl;
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
	auto visJson = ofJson::parse(args.message);
	try {
		auto c1Json = ofJson::parse(visJson.at("c1").dump());
		
		int index = 0;
		for (auto& meta : c1Json) {
			std::cout << index++ << ": " << std::endl;
			auto metaJson = ofJson::parse(meta.dump());
			try {
				std::cout << "\tcaption:" << metaJson.at("caption") << std::endl;
				std::cout << "\timage_id:" << metaJson.at("image_id") << std::endl;
			} catch (std::domain_error& e) {
				std::cout << e.what() << std::endl;
			}
		}
	} catch (std::domain_error& e) {
		std::cout << e.what() << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
	cout<<"got broadcast "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case 'a':
		{
			ImageMetaBundle metaBundle;
			metaBundle.add("images/aurora.jpg", "aurora in the sky", false);
			metaBundle.add("images/boat.jpg", "fishing boat on beach", false);
			metaBundle.add("images/bridge.jpg", "a bridge, that's huge", false);
			metaBundle.add("images/city.jpg", "city skyline on river bank", false);
			metaBundle.add("images/desert.jpg", "sunset of desert", false);
			metaBundle.add("images/eagleview.jpg", "a town in eagles eye", false);
			metaBundle.add("images/grass.jpg", "forrest wake up in spring", false);
			metaBundle.add("images/island.jpg", "abandoned islands", false);
			metaBundle.add("images/lake.jpg", "lake surrounded by moutains", false);
			metaBundle.add("images/road.jpg", "road with dim light", false);
			metaBundle.add("images/sunset.jpg", "chilly sunset", false);
			metaBundle.add("images/swamp.jpg", "swamp looks like mirror", false);
			
			mLayer->load(metaBundle);
		}
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
