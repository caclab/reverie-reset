//
//  Cell.hpp
//  Layer
//
//  Created by Ean Jee on 17/03/2017.
//
//

#ifndef Cell_hpp
#define Cell_hpp

#include "ofMain.h"

class Cell {
public:
	enum State {IDLE, IMAGE, COLOR, TEXT, BLACK};
	
	Cell();
	~Cell();
	
	void setup(int id, std::shared_ptr<class ofxCenteredTrueTypeFont> font,
			   glm::vec2 pos, glm::vec2 size,
			   std::shared_ptr<class BundleBuffer> bundleBuffer,
			   float timeImage, float timeColor, float timeText, float timeBlack);
	// start to cycle rendering images from ImageInfoBundle
	void cycle();
	void update();
	void draw();
	// cycle finished
	bool isFinished();
	
	// protected
	
	const float ORIGINAL_WIDTH = 1280.0f;
	const float ORIGINAL_HEIGHT = 720.0f;
	
	void wrapText();
	
	int mId;
	
	// double buffered
	std::shared_ptr<class BundleBuffer> mBundleBuffer;
	std::shared_ptr<class ImageInfoBundle> mImageInfoBundle;
	bool mGoFlip;
	
	// for cycling
	int mIndexStart, mIndexCurrent;
	// duration of each state
	float mTimeImage, mTimeColor, mTimeText, mTimeBlack;
	float mTimeStart;
	State mState;
	bool mOneCycleFinished;
	
	// how to render image and averaged color
	glm::vec2 mPos, mSize;
	// how to render text
	std::string mWrappedText;
	std::shared_ptr<class ofxCenteredTrueTypeFont> mFont;
	float mScaleText;
};

#endif /* Cell_hpp */
