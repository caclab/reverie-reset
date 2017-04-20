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
	
	const float ORIGINAL_WIDTH = 1280.0f;
	const float ORIGINAL_HEIGHT = 720.0f;
	
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
	// how to render text
	std::string mWrappedText;
	ofxCenteredTrueTypeFont mFont;
	float mScaleText;
};

#endif /* Cell_hpp */
