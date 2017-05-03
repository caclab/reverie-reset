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
				 float newTimeText, float newTimeBlack) {
	mId = id;
	mPos = pos;
	mSize = size;
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
	
	mTimeImage = ofRandom(mRandomTimeImageRange.x, mRandomTimeImageRange.y);
	mTimeColor = ofRandom(mRandomTimeColorRange.x, mRandomTimeColorRange.y);
	mTimeText = ofRandom(mRandomTimeTextRange.x, mRandomTimeTextRange.y);
	mTimeBlack = ofRandom(mRandomTimeBlackRange.x, mRandomTimeBlackRange.y);
	
	mFont = font;
	mScaleText = size.y / ORIGINAL_HEIGHT;
	
	mBackToRandom = false;
	
//	// start with random state, init property
//	mImageInfoBundle = mRandomBundleBuffer->getNext();
//	
//	mIsNew = false;
//	
//	mTimeStart = ofGetElapsedTimef();
//	mState = BLACK;
//	mOneCycleFinished = false;
//	mGoFlip = false;
}

void Cell::random() {
	mBackToRandom = true;
}

void Cell::boom() {
	mIsNew = true;
	
	mImageInfoBundle = mNewBundleBuffer->getNext();
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
}

void Cell::draw() {
	switch (mState) {
		case IMAGE:
			ofSetColor(255);
			mImageInfoBundle->mImageInfos[mIndexCurrent]->mImage.draw(mPos, mSize.x, mSize.y);
			break;
		case COLOR:
			ofSetColor(mImageInfoBundle->mImageInfos[mIndexCurrent]->mAvgColor);
			ofDrawRectangle(mPos, mSize.x, mSize.y);
			break;
		case TEXT:
			ofSetColor(mImageInfoBundle->mImageInfos[mIndexCurrent]->mAvgColor);
			ofDrawRectangle(mPos, mSize.x, mSize.y);
			ofSetColor(255);
			ofPushMatrix();
			ofTranslate(mPos);
			ofScale(mScaleText);
			mFont->drawStringCentered(mWrappedText, ORIGINAL_WIDTH * 0.5f,
									  ORIGINAL_HEIGHT * 0.5f);
			ofPopMatrix();
			break;
		case BLACK:
			ofSetColor(0);
			ofDrawRectangle(mPos, mSize.x, mSize.y);
			break;
		default:
			break;
	}
}

bool Cell::isFinished() {
	return mOneCycleFinished;
}

void Cell::backToRandom() {
	mBackToRandom = false;
	mIsNew = false;
	
	mImageInfoBundle = mRandomBundleBuffer->getNext();
	
	int totalSize = mImageInfoBundle->mImageInfos.size();
	if (totalSize > 0) {
		mIndexStart = mId % mImageInfoBundle->mImageInfos.size();
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

void Cell::wrapText() {
	// wrap text based on cell width
	mWrappedText = "";
	
	std::vector<std::string> words = ofSplitString(mImageInfoBundle->mImageInfos[mIndexCurrent]->mText, " ", true, true);
	
	float maxWidth = ORIGINAL_WIDTH * 0.6f;
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
	int totalSize = mImageInfoBundle->mImageInfos.size();
	if (totalSize > 0) {
		int currentIndex;
		if (mIndexCurrent < mIndexStart) {
			currentIndex = mIndexCurrent + totalSize;
		} else {
			currentIndex = mIndexCurrent;
		}
		if (!mOneCycleFinished && currentIndex - mIndexStart > 0) {
			// one cycle finished
			mOneCycleFinished = true;
			std::cout << mId << " finished" << std::endl;
		}
		
		int indexNext = (mIndexCurrent + 1) % totalSize;
		// cycle not finished, switch to next image
		mIndexCurrent = indexNext;
		mState = IMAGE;
		mTimeStart = timeCurrent;
	}
}

void Cell::randomStateInBlack(float timeCurrent) {
	if (timeCurrent - mTimeStart >= mTimeBlack) {
		if (mGoFlip && mRandomBundleBuffer->isFlippable()) {
			// flip to next buffer
			mGoFlip = false;
			mImageInfoBundle = mRandomBundleBuffer->getNext();
			
			int totalSize = mImageInfoBundle->mImageInfos.size();
			if (totalSize > 0) {
				mIndexStart = mId % mImageInfoBundle->mImageInfos.size();
				mIndexCurrent = mIndexStart;
				
				mState = IMAGE;
				mTimeStart = timeCurrent;
				
				mOneCycleFinished = false;
			}
		} else {
			// cycle
			cycle(timeCurrent);
		}
	}
}

void Cell::newStateInBlack(float timeCurrent) {
	mOneCycleFinished = true;
	
	if (timeCurrent - mTimeStart >= mTimeBlack) {
		if (mBackToRandom) {
			backToRandom();
		} else {
			// cycle
			cycle(timeCurrent);
		}
	}
}
