#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	const float WINDOW_WIDTH = 1280;
	const float WINDOW_HEIGHT = 720;
	const int WINDOW_NUM = 12;
	
	std::vector<std::shared_ptr<ofAppBaseWindow>> windows;
	
	ofGLFWWindowSettings settings;
	
	for (int i = 0; i < WINDOW_NUM; i++) {
		
		settings.width = WINDOW_WIDTH;
		settings.height = WINDOW_HEIGHT;
		settings.setPosition(ofVec2f(i * WINDOW_WIDTH, 0));
		settings.resizable = false;
		settings.decorated = false;
		std::shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
		
		windows.push_back(window);
	}
	
	std::vector<void(ofApp::*)(ofEventArgs &)> funcs;
	funcs.push_back(nullptr);
	funcs.push_back(&ofApp::drawWindow01);
	funcs.push_back(&ofApp::drawWindow02);
	funcs.push_back(&ofApp::drawWindow03);
	
	funcs.push_back(&ofApp::drawWindow04);
	funcs.push_back(&ofApp::drawWindow05);
	funcs.push_back(&ofApp::drawWindow06);
	
	
	funcs.push_back(&ofApp::drawWindow07);
	funcs.push_back(&ofApp::drawWindow08);
	funcs.push_back(&ofApp::drawWindow09);
	
	
	funcs.push_back(&ofApp::drawWindow10);
	funcs.push_back(&ofApp::drawWindow11);
	
	std::shared_ptr<ofApp> mainApp(new ofApp);
	for (int i = 1; i < WINDOW_NUM; i++) {
		ofAddListener(windows[i]->events().draw, mainApp.get(), funcs[i]);
	}
	
	
	ofRunApp(windows[0], mainApp);
	ofRunMainLoop();
}
