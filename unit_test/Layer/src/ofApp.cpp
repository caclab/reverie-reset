#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	mLayer = std::make_shared<Layer>();
	mLayer->setup(WINDOW_NUM, glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
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
void ofApp::keyPressed(int key){
	switch (key) {
		case 'l':
		{
			std::vector<ImageMeta> metas;
			for (int i = 0; i < 1; i++) {
				ImageMeta meta;
				meta.mText = "r: " + ofToString(i);
				meta.mPath = "http://www.openframeworks.cc/images/of_inverted.png";
				meta.mIsUrl = true;
				metas.push_back(meta);
			}
			mLayer->load(metas);
		}
			break;
		case 'a':
		{
			std::vector<ImageMeta> metas;
			ImageMeta meta;
			meta.mText = "aurora in the sky";
			meta.mPath = "images/aurora.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "fishing boat on beach";
			meta.mPath = "images/boat.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "a bridge, that's huge";
			meta.mPath = "images/bridge.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "city skyline on river bank";
			meta.mPath = "images/city.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "sunset of desert";
			meta.mPath = "images/desert.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "a town in eagles eye";
			meta.mPath = "images/eagleview.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "forrest wake up in spring";
			meta.mPath = "images/grass.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "abandoned islands";
			meta.mPath = "images/island.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "lake surrounded by moutains";
			meta.mPath = "images/lake.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "road with dim light";
			meta.mPath = "images/road.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "chilly sunset";
			meta.mPath = "images/sunset.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			meta.mText = "swamp looks like mirror";
			meta.mPath = "images/swamp.jpg";
			meta.mIsUrl = false;
			metas.push_back(meta);
			
			mLayer->load(metas);
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
