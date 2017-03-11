#pragma once

#include "ofMain.h"

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
};
