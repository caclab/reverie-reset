#pragma once

#include "ofMain.h"
#include "Render.h"
#include "ofxLibwebsockets.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

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
		
		//Image Renderering
		Render imageRenderer;

		//Screens
		int screenResX, screenResY, totalScreens;
		int windowSizeX, windowSizeY;
		bool testMode, showGrid;

		//App config XML
        ofXml configXML;

        //WEBSOCKETS
		string sAddress;
		int sPort;
        uint64_t lastReconnectTime; //timer
        int sTimeout;
        ofxLibwebsockets::Client client;
        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );

        bool messageArrived;
        string message;
};
