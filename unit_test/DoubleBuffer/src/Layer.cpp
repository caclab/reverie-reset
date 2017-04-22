//
//  Layer.hpp
//  Layer
//
//  Created by Ean Jee on 17/03/2017.
//
//

#include "Layer.hpp"
#include "Cell.hpp"
#include "BundleBuffer.hpp"
#include "ofxCenteredTrueTypeFont.h"

Layer::Layer() {
	mBundleBuffer = std::make_shared<BundleBuffer>();

	ofAddListener(mBundleBuffer->mLoadedEvent, this, &Layer::onLoaded);
	
	mFont = std::make_shared<ofxCenteredTrueTypeFont>();
	mFont->load("fonts/verdana.ttf", 64, true, true, true);
	
	mReadySent = true;
}

Layer::~Layer() {
	ofRemoveListener(mBundleBuffer->mLoadedEvent, this, &Layer::onLoaded);
}

void Layer::setup(int cellNum, glm::vec2 size) {
	mCells.clear();
	for (int i = 0; i < cellNum; i++) {
		std::shared_ptr<Cell> cell = std::make_shared<Cell>();
//		cell->setup(i, mFont, glm::vec2(0), size, mBundleBuffer, 1, 1, 1, 1);
		cell->setup(i, mFont, glm::vec2(0), size, mBundleBuffer,
					ofRandom(5, 10), ofRandom(1, 3), ofRandom(5, 10), ofRandom(1));
		
		mBundleBuffer->addUser(cell);

		mCells.push_back(cell);
	}
}

void Layer::onLoaded() {
	ofLogNotice("Layer") << "all cells loaded";
	
	mReadySent = false;
}

bool Layer::isCellFinished() {
	bool ret = true;
	for (auto& cell : mCells) {
		if (!cell->isFinished()) {
			ret = false;
			break;
		}
	}

	return ret;
}

void Layer::update() {
	mBundleBuffer->update();
	
	for (auto& cell : mCells) {
		cell->update();
	}
	
	sendReady();
}

void Layer::draw() {
	for (auto& cell : mCells) {
		cell->draw();
	}
}

void Layer::draw(int cellIndex) {
	if (cellIndex >= 0 && cellIndex < mCells.size()) {
		mCells[cellIndex]->draw();
	} else {
		ofLogError("Layer") << "draw with invalid cell index";
	}
}

void Layer::load(const ImageMetaBundle& metaBundle) {
	mBundleBuffer->load(metaBundle);
}

void Layer::sendReady() {
	if (!mReadySent && isCellFinished()) {
		ofNotifyEvent(mReadyEvent, this);
		mReadySent = true;
	}
}

void Layer::timeImageChanged(float & time) {
	for (auto& cell : mCells) {
		cell->mTimeImage = time;
	}
}

void Layer::timeColorChanged(float & time) {
	for (auto& cell : mCells) {
		cell->mTimeColor = time;
	}
}

void Layer::timeTextChanged(float & time) {
	for (auto& cell : mCells) {
		cell->mTimeText = time;
	}
}

void Layer::timeBlackChanged(float & time) {
	for (auto& cell : mCells) {
		cell->mTimeBlack = time;
	}
}
