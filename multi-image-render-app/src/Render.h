#ifndef _Render 
#define _Render 
#include "ofMain.h"
#include "ofxThreadedImageLoader.h"

class Render {

    public:

	    // methods
	    void setup(int ts, int rx, int ry, int px, int py);
	    void update();
	    void draw(); 
	    void exit();

	    ofColor averageColor(ofImage img);

	    // variables
	    ofxThreadedImageLoader loader;
		vector<ofImage> images;
		// vector<ofColor> avrColors;
		vector<bool> imgSwitches;
		//vector<vector<ofColor>> avrColorsByLines;
		bool isColor;
		vector<string> imgPath;
		int resX, resY, posX, posY, totalScreens;

		//Timer
		vector<uint64_t> startTime;
		vector<uint64_t> timeInterval;
		// uint64_t startTime;
  		// uint64_t timeInterval;

	    Render();  
    
}; 
#endif
