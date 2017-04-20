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
	mScaleText = size.y / ORIGINAL_HEIGHT;
	
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
					
					width += mFont.stringWidth(newWord);
					
					if (width > maxWidth) {
						mWrappedText += "\n";
						width = mFont.stringWidth(word);
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
			break;
		case TEXT:
			if (timeCurrent - mTimeStart >= mTimeText) {
				mState = BLACK;
				mTimeStart = timeCurrent;
			}
			break;
		case BLACK:
			if (timeCurrent - mTimeStart >= mTimeBlack) {
				int totalSize = mImageInfoBundle->mImageInfos.size();
				if (totalSize > 0) {
					int currentIndex;
					if (mIndexCurrent < mIndexStart) {
						currentIndex = mIndexCurrent + totalSize;
					} else {
						currentIndex = mIndexCurrent;
					}
					if (currentIndex - mIndexStart > 0) {
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
			ofTranslate(mPos);
			ofScale(mScaleText);
			mFont.drawStringCentered(mWrappedText, ORIGINAL_WIDTH * 0.5f,
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
