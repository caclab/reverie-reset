//
//  Cell.hpp
//  Layer
//
//  Created by Ean Jee on 17/03/2017.
//
//

#ifndef Cell_hpp
#define Cell_hpp

#include <memory>
#include "ofMain.h"
#include "ofxCenteredTrueTypeFont.h"

class Cell {
public:
	enum State {IDLE, IMAGE, COLOR, TEXT, BLACK};
	
	Cell();
	~Cell();
	
	void setup(glm::vec2 pos, glm::vec2 size,
			   std::shared_ptr<class ImageInfoBundle> imageInfoBundle, int indexStart,
			   float timeImage, float timeColor, float timeText, float timeBlack);
	// start to cycle rendering images from ImageInfoBundle
	void cycle();
	void update();
	void draw();
	// cycle finished
	bool isFinished();
	
	// protected
	
	std::shared_ptr<class ImageInfoBundle> mImageInfoBundle;
	// for cycling
	int mIndexStart, mIndexCurrent;
	// duration of each state
	float mTimeImage, mTimeColor, mTimeText, mTimeBlack;
	float mTimeStart;
	State mState;
	bool mOneCycleFinished;
	
	// how to render image and averaged color
	glm::vec2 mPos, mSize;
	// how to render text, mPosText is relative position to image center
	ofxCenteredTrueTypeFont mFont;
	glm::vec2 mPosText;
	float mScaleText;
};

#endif /* Cell_hpp */
