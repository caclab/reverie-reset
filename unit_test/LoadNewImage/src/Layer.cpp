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
	mRandomBundleBuffer = std::make_shared<BundleBuffer>();
	mNewBundleBuffer = std::make_shared<BundleBuffer>();

	ofAddListener(mRandomBundleBuffer->mLoadedEvent, this, &Layer::onRandomLoaded);
	ofAddListener(mNewBundleBuffer->mLoadedEvent, this, &Layer::onNewLoaded);
	
	mFont = std::make_shared<ofxCenteredTrueTypeFont>();
	mFont->load("fonts/verdana.ttf", 64, true, true, true);
	
	mReadySent = true;
	mStandbySent = true;
}

Layer::~Layer() {
	ofRemoveListener(mRandomBundleBuffer->mLoadedEvent, this, &Layer::onRandomLoaded);
	ofRemoveListener(mNewBundleBuffer->mLoadedEvent, this, &Layer::onNewLoaded);
}

void Layer::setup(int cellNum, glm::vec2 size) {
	mCells.clear();
	for (int i = 0; i < cellNum; i++) {
		std::shared_ptr<Cell> cell = std::make_shared<Cell>();
		cell->setup(i, mFont, glm::vec2(0), size, mRandomBundleBuffer, mNewBundleBuffer,
					glm::vec2(1, 1), glm::vec2(1, 1), glm::vec2(1, 1), glm::vec2(1, 1),
					1, 1, 1, 1);
//		cell->setup(i, mFont, glm::vec2(0), size, mRandomBundleBuffer, mNewBundleBuffer,
//					glm::vec2(5, 10), glm::vec2(1, 3), glm::vec2(5, 10), glm::vec2(1, 1),
//					1, 1, 1, 1);
		
		mRandomBundleBuffer->addUser(cell);
		mNewBundleBuffer->addUser(cell);

		mCells.push_back(cell);
	}
}

void Layer::onRandomLoaded() {
	ofLogNotice("Layer") << "all random images loaded";
	
	mReadySent = false;
	for (auto& cell : mCells) {
		cell->mOneCycleFinished = false;
		cell->random();
	}
}

void Layer::onNewLoaded() {
	ofLogNotice("Layer") << "a new image loaded";
	
	mStandbySent = false;
	for (auto& cell : mCells) {
		cell->mOneCycleFinished = false;
	}
}

void Layer::boom() {
	if (mStandbySent) {
		// ready message also need to be sent after BOOM finished
		mReadySent = false;
		
		for (auto& cell : mCells) {
			cell->boom();
		}
	}
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
	mRandomBundleBuffer->update();
	mNewBundleBuffer->update();
	
	for (auto& cell : mCells) {
		cell->update();
	}
	
	sendReady();
	sendStandby();
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

void Layer::loadRandom(const ImageMetaBundle& metaBundle) {
	mRandomBundleBuffer->load(metaBundle);
}

void Layer::loadNew(const ImageMetaBundle& metaBundle) {
	mNewBundleBuffer->load(metaBundle);
}

void Layer::sendReady() {
	if (!mReadySent && isCellFinished()) {
		ofNotifyEvent(mReadyEvent, this);
		mReadySent = true;
	}
}

void Layer::sendStandby() {
	if (!mStandbySent) {
		ofNotifyEvent(mStandbyEvent, this);
		mStandbySent = true;
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
