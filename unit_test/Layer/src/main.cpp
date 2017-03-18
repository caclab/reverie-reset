#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.width = WINDOW_WIDTH;
	settings.height = WINDOW_HEIGHT;
	settings.setPosition(glm::vec2(0, 0));
	settings.resizable = false;
	settings.decorated = false;
	
	std::shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	
	std::vector<std::shared_ptr<ofAppBaseWindow>> windows;
	
	for (int i = 1; i < WINDOW_NUM; i++) {
		settings.setGLVersion(3, 2);
		settings.width = WINDOW_WIDTH;
		settings.height = WINDOW_HEIGHT;
		settings.setPosition(glm::vec2(i * WINDOW_WIDTH, 0));
		settings.resizable = false;
		settings.decorated = false;
		settings.shareContextWith = mainWindow;
		std::shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
		
		windows.push_back(window);
	}
	
	std::vector<void(ofApp::*)(ofEventArgs &)> funcs;
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
	for (int i = 0; i < WINDOW_NUM - 1; i++) {
		ofAddListener(windows[i]->events().draw, mainApp.get(), funcs[i]);
	}
	
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
}
