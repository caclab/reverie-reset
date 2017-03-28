//
//  Cell.cpp
//  Layer
//
//  Created by Ean Jee on 17/03/2017.
//
//

#include "Cell.hpp"
#include "ImageInfoBundle.hpp"

Cell::Cell() {
	mState = IDLE;
	
	mFont.load("fonts/verdana.ttf", 64, true, true, true);
	mOneCycleFinished = true;
}

Cell::~Cell() {
}

void Cell::setup(glm::vec2 pos, glm::vec2 size,
				 std::shared_ptr<ImageInfoBundle> imageInfoBundle, int indexStart,
				 float timeImage, float timeColor, float timeText, float timeBlack) {
	mPos = pos;
	mSize = size;
	mImageInfoBundle = imageInfoBundle;
	mIndexStart = (int)ofClamp(indexStart, 0, imageInfoBundle->mImageInfos.size());
	mTimeImage = timeImage;
	mTimeColor = timeColor;
	mTimeText = timeText;
	mTimeBlack = timeBlack;
	
	mIndexCurrent = mIndexStart;
	mScaleText = size.y / 720;
	
	cycle();
}

void Cell::cycle() {
	mTimeStart = ofGetElapsedTimef();
	mState = IMAGE;
	mOneCycleFinished = false;
}

void Cell::update() {
	float timeCurrent = ofGetElapsedTimef();
	switch (mState) {
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
				
				std::string text = mImageInfoBundle->mImageInfos[mIndexCurrent]->mText;
				float textWidth = mFont.stringWidth(text);
				mPosText.x = textWidth * -0.5f;
				mPosText.y = 0;
			}
			break;
		case TEXT:
			if (timeCurrent - mTimeStart >= mTimeText) {
				mState = BLACK;
				mTimeStart = timeCurrent;
			}
			break;
		case BLACK:
			if (timeCurrent - mTimeStart >= mTimeBlack) {
				int indexNext = (mIndexCurrent + 1) % mImageInfoBundle->mImageInfos.size();
				if (indexNext == mIndexStart) {
					// one cycle finished
					mOneCycleFinished = true;
				}
				
				// cycle not finished, switch to next image
				mIndexCurrent = indexNext;
				mState = IMAGE;
				mTimeStart = timeCurrent;
			}
			break;
		default:
			break;
	}
}

void Cell::draw() {
	switch (mState) {
		case IMAGE:
			ofSetColor(255, 255);
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
			ofTranslate(mPos + mSize * 0.5f);
			ofScale(mScaleText);
			mFont.drawStringAsShapes(mImageInfoBundle->mImageInfos[mIndexCurrent]->mText,
									 mPosText.x, mPosText.y);
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
