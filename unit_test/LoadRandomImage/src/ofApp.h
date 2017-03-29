#pragma once

#include "ofMain.h"
#include "Layer.hpp"
#include "ofxLibwebsockets.h"

const float WINDOW_WIDTH = 128;
const float WINDOW_HEIGHT = 72;
const int WINDOW_NUM = 12;

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	void drawWindow01(ofEventArgs & args);
	void drawWindow02(ofEventArgs & args);
	void drawWindow03(ofEventArgs & args);
	void drawWindow04(ofEventArgs & args);
	void drawWindow05(ofEventArgs & args);
	void drawWindow06(ofEventArgs & args);
	void drawWindow07(ofEventArgs & args);
	void drawWindow08(ofEventArgs & args);
	void drawWindow09(ofEventArgs & args);
	void drawWindow10(ofEventArgs & args);
	void drawWindow11(ofEventArgs & args);
	
	std::shared_ptr<Layer> mLayer;
	
	ofxLibwebsockets::Client mClient;
	
	// websocket methods
	void onConnect( ofxLibwebsockets::Event& args );
	void onOpen( ofxLibwebsockets::Event& args );
	void onClose( ofxLibwebsockets::Event& args );
	void onIdle( ofxLibwebsockets::Event& args );
	void onMessage( ofxLibwebsockets::Event& args );
	void onBroadcast( ofxLibwebsockets::Event& args );
};
