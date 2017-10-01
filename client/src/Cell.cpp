//
//  Cell.cpp
//  Layer
//
//  Created by Ean Jee on 17/03/2017.
//
//

#include "Cell.hpp"
#include "ofxCenteredTrueTypeFont.h"
#include "ImageInfoBundle.hpp"
#include "BundleBuffer.hpp"

Cell::Cell() {
	mState = IDLE;
	
	mOneCycleFinished = true;
}

Cell::~Cell() {
}

void Cell::setup(int id, std::shared_ptr<ofxCenteredTrueTypeFont> font,
				 glm::vec2 pos, glm::vec2 size,
				 std::shared_ptr<BundleBuffer> randomBuffer,
				 std::shared_ptr<BundleBuffer> newBuffer,
				 glm::vec2 randomTimeImage, glm::vec2 randomTimeColor,
				 glm::vec2 randomTimeText, glm::vec2 randomTimeBlack,
				 float newTimeImage, float newTimeColor,
				 float newTimeText, float newTimeBlack,
				 bool useCycle, int cycleNum) {
	mId = id;
	mFont = font;
	mPos = pos;
	mSize = size;
	
	mCanvas.allocate(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	mRandomBundleBuffer = randomBuffer;
	mNewBundleBuffer = newBuffer;
	
	mRandomTimeImageRange = randomTimeImage;
	mRandomTimeColorRange = randomTimeColor;
	mRandomTimeTextRange = randomTimeText;
	mRandomTimeBlackRange = randomTimeBlack;
	
	mNewTimeImage = newTimeImage;
	mNewTimeColor = newTimeColor;
	mNewTimeText = newTimeText;
	mNewTimeBlack = newTimeBlack;
	
	mBackToRandom = false;
	
	mUseCycle = useCycle;
	setCycleNum(cycleNum);
}

void Cell::random() {
	mBackToRandom = true;
}

void Cell::boom() {
	mIsNew = true;
	mBackToRandom = false;
	
	if (auto bundleBuffer = mNewBundleBuffer.lock()) {
		mImageInfoBundle = bundleBuffer->getNext();
	}
		
	// ImageInfoBundle in mNewBundleBuffer has only 1 image
	mIndexStart = 0;
	mIndexCurrent = 0;
	
	mTimeImage = mNewTimeImage;
	mTimeColor = mNewTimeColor;
	mTimeText = mNewTimeText;
	mTimeBlack = mNewTimeBlack;
	
	mTimeStart = ofGetElapsedTimef();
	mState = IMAGE;
	mOneCycleFinished = false;
	mGoFlip = false;
}

void Cell::update() {
	float timeCurrent = ofGetElapsedTimef();
	switch (mState) {
		case IDLE:
			if (mBackToRandom) {
				backToRandom();
			}
			break;
		case IMAGE:
			if (timeCurrent - mTimeStart >= mTimeImage) {
				mState = COLOR;
				mTimeStart = timeCurrent;
			}
			break;
		case COLOR:
			if (timeCurrent - mTimeStart >= mTimeColor) {
				mState = TEXT;
				mTimeStart = timeCurrent;
				
				wrapText();
			}
			break;
		case TEXT:
			if (timeCurrent - mTimeStart >= mTimeText) {
				mState = BLACK;
				mTimeStart = timeCurrent;
			}
			break;
		case BLACK:
			if (isNew()) {
				newStateInBlack(timeCurrent);
			} else {
				randomStateInBlack(timeCurrent);
			}
			break;
		default:
			break;
	}
	
	mCanvas.begin();
	ofClear(0);
	
	switch (mState) {
		case IMAGE:
		{
			ofSetColor(255);
			if (auto infoBundle = mImageInfoBundle.lock()) {
				glm::vec2 pos = infoBundle->mImageInfos[mIndexCurrent]->mRenderPos;
				glm::vec2 size = infoBundle->mImageInfos[mIndexCurrent]->mRenderSize;
				
				int currentIndex;
				
				if (mUseCycle) {
					currentIndex = mIndexCurrent;
				} else {
					currentIndex = mIndexStart;
				}
				
				infoBundle->mImageInfos[currentIndex]->mImage.draw(pos, size.x, size.y);
			}
		}
			break;
		case COLOR:
		{
			if (auto infoBundle = mImageInfoBundle.lock()) {
				ofSetColor(infoBundle->mImageInfos[mIndexCurrent]->mAvgColor);
				ofDrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			}
		}
			break;
		case TEXT:
		{
			if (auto infoBundle = mImageInfoBundle.lock()) {
				ofSetColor(infoBundle->mImageInfos[mIndexCurrent]->mAvgColor);
				ofDrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
				ofSetColor(255);
				mFont->drawStringCentered(mWrappedText, SCREEN_WIDTH * 0.5f,
										  SCREEN_HEIGHT * 0.5f);
			}
		}
			break;
		case BLACK:
		{
			ofSetColor(0);
			ofDrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
			break;
		default:
			break;
	}
	mCanvas.end();
}

void Cell::draw() {
	ofSetColor(255);
	mCanvas.draw(mPos, mSize.x, mSize.y);
}

bool Cell::isFinished() {
	return mOneCycleFinished;
}

void Cell::setCycleNum(int num) {
	mCycleNum = std::max(0, num - 1);
}

void Cell::backToRandom() {
	mBackToRandom = false;
	mIsNew = false;
	
	if (auto bundleBuffer = mRandomBundleBuffer.lock()) {
		mImageInfoBundle = bundleBuffer->getNext();
	}
	if (auto infoBundle = mImageInfoBundle.lock()) {
		int totalSize = infoBundle->mImageInfos.size();
		if (totalSize > 0) {
			mIndexStart = mId % infoBundle->mImageInfos.size();
			mIndexCurrent = mIndexStart;
			
			mTimeImage = ofRandom(mRandomTimeImageRange.x, mRandomTimeImageRange.y);
			mTimeColor = ofRandom(mRandomTimeColorRange.x, mRandomTimeColorRange.y);
			mTimeText = ofRandom(mRandomTimeTextRange.x, mRandomTimeTextRange.y);
			mTimeBlack = ofRandom(mRandomTimeBlackRange.x, mRandomTimeBlackRange.y);
			
			mTimeStart = ofGetElapsedTimef();
			mState = IMAGE;
			mOneCycleFinished = false;
			mGoFlip = false;
		}
	}
}

void Cell::wrapText() {
	// wrap text based on cell width
	mWrappedText = "";
	
	if (auto infoBundle = mImageInfoBundle.lock()) {
		std::vector<std::string> words = ofSplitString(infoBundle->mImageInfos[mIndexCurrent]->mText, " ", true, true);
		
		float maxWidth = SCREEN_WIDTH * 0.6f;
		float width = 0;
		
		for (auto& word : words) {
			std::string newWord;
			if (mWrappedText.size() > 0) {
				newWord = " " + word;
			} else {
				newWord = word;
			}
			
			width += mFont->stringWidth(newWord);
			
			if (width > maxWidth) {
				mWrappedText += "\n";
				width = mFont->stringWidth(word);
				mWrappedText += word;
			} else {
				mWrappedText += newWord;
			}
		}
		
		mWrappedText += ".";
	}
	
	try {
		mWrappedText = ofToUpper(mWrappedText.substr(0, 1)) + mWrappedText.substr(1);
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

bool Cell::isNew() {
	return mIsNew;
}

void Cell::cycle(float timeCurrent) {
	if (auto infoBundle = mImageInfoBundle.lock()) {
		int totalSize = infoBundle->mImageInfos.size();
		if (totalSize > 0) {
			int currentIndex;
			if (mIndexCurrent < mIndexStart) {
				currentIndex = mIndexCurrent + totalSize;
			} else {
				currentIndex = mIndexCurrent;
			}
			if (!mOneCycleFinished && currentIndex - mIndexStart >= mCycleNum) {
				// one cycle finished
				mOneCycleFinished = true;
			}
			
			int indexNext = (mIndexCurrent + 1) % totalSize;
			// cycle not finished, switch to next image
			mIndexCurrent = indexNext;
			mState = IMAGE;
			mTimeStart = timeCurrent;
		}
	}
}

void Cell::randomStateInBlack(float timeCurrent) {
	if (timeCurrent - mTimeStart >= mTimeBlack) {
		if (mGoFlip) {
			if (auto bundleBuffer = mRandomBundleBuffer.lock()) {
				if (bundleBuffer->isFlippable()) {
					// flip to next buffer
					mGoFlip = false;
					mImageInfoBundle = bundleBuffer->getNext();
					
					if (auto infoBundle = mImageInfoBundle.lock()) {
						int totalSize = infoBundle->mImageInfos.size();
						if (totalSize > 0) {
							mIndexStart = mId % infoBundle->mImageInfos.size();
							mIndexCurrent = mIndexStart;
							
							mState = IMAGE;
							mTimeStart = timeCurrent;
							
							mOneCycleFinished = false;
						}
					}
				}
			}
		} else {
			// cycle
			cycle(timeCurrent);
		}
	}
}

void Cell::newStateInBlack(float timeCurrent) {
	// use this flag here to prevent send READY message
	// at the beginning of switch to RANDOM mode
	if (!mBackToRandom) {
		// tell Layer this one ready when entering BLACK state
		// instead of exiting BLACK state in RANDOM mode,
		// in order to switch naturally if next message from server
		// is also a NEW image
		mOneCycleFinished = true;
	}
	
	if (timeCurrent - mTimeStart >= mTimeBlack) {
		if (mBackToRandom) {
			backToRandom();
		} else {
			// cycle
			cycle(timeCurrent);
		}
	}
}
