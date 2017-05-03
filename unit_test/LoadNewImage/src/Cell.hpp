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
			   std::shared_ptr<class BundleBuffer> randomBuffer,
			   std::shared_ptr<class BundleBuffer> newBuffer,
			   glm::vec2 randomTimeImage, glm::vec2 randomTimeColor,
			   glm::vec2 randomTimeText, glm::vec2 randomTimeBlack,
			   float newTimeImage, float newTimeColor,
			   float newTimeText, float newTimeBlack);
	
	
	void random();
	
	// cut to images from mNewBundleBuffer
	void boom();
	
	void update();
	void draw();
	// cycle finished
	bool isFinished();
	
	// protected
	
	const float ORIGINAL_WIDTH = 1280.0f;
	const float ORIGINAL_HEIGHT = 720.0f;
	
	// start to cycle rendering images from mRandomBundleBuffer
	void backToRandom();
	// wrap text to fit screen
	void wrapText();
	// random image or new image
	bool isNew();
	// cycling images from current mImageInfoBundle
	void cycle(float timeCurrent);
	
	void randomStateInBlack(float timeCurrent);
	void newStateInBlack(float timeCurrent);
	
	int mId;
	
	// for current use
	std::shared_ptr<class ImageInfoBundle> mImageInfoBundle;
	
	// double buffering for random image
	std::shared_ptr<class BundleBuffer> mRandomBundleBuffer;
	bool mGoFlip;
	
	// double buffering for new image
	std::shared_ptr<class BundleBuffer> mNewBundleBuffer;
	bool mBackToRandom;

	// false: random image, true: new image
	bool mIsNew;	

	// for cycling
	int mIndexStart, mIndexCurrent;
	// duration of each state
	glm::vec2 mRandomTimeImageRange, mRandomTimeColorRange;
	glm::vec2 mRandomTimeTextRange, mRandomTimeBlackRange;
	float mNewTimeImage, mNewTimeColor;
	float mNewTimeText, mNewTimeBlack;
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
