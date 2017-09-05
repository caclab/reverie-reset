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
	
	initConfig();
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
					glm::vec2(5, 10), glm::vec2(1, 3), glm::vec2(5, 10), glm::vec2(1, 1),
					1, 1, 1, 1, true, 1);
		
		mRandomBundleBuffer->addUser(cell);
		mNewBundleBuffer->addUser(cell);

		mCells.push_back(cell);
	}
	
	ofXml xml;
	if (xml.load("settings/layer.xml")) {
		ofDeserialize(xml, pParameters);
	}
}

void Layer::reset() {
	mRandomBundleBuffer->reset();
	mNewBundleBuffer->reset();
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

void Layer::initConfig() {
	pParameters.setName("Layer");
	pRandomStateGroup.setName("RANDOM State");
	pNewStateGroup.setName("NEW State");
	pCycleGroup.setName("Cycle");
	
	pRandomTimeImage.addListener(this, &Layer::randomTimeImageChanged);
	pRandomStateGroup.add(pRandomTimeImage.set("Image Time", glm::vec2(5, 10)));
	
	pRandomTimeColor.addListener(this, &Layer::randomTimeColorChanged);
	pRandomStateGroup.add(pRandomTimeColor.set("Color Time", glm::vec2(1, 3)));
	
	pRandomTimeText.addListener(this, &Layer::randomTimeTextChanged);
	pRandomStateGroup.add(pRandomTimeText.set("Text Time", glm::vec2(5, 10)));
	
	pRandomTimeBlack.addListener(this, &Layer::randomTimeBlackChanged);
	pRandomStateGroup.add(pRandomTimeBlack.set("Black Time", glm::vec2(1, 1)));
	
	pParameters.add(pRandomStateGroup);
	
	pNewTimeImage.addListener(this, &Layer::newTimeImageChanged);
	pNewStateGroup.add(pNewTimeImage.set("Image Time", 5));
	
	pNewTimeColor.addListener(this, &Layer::newTimeColorChanged);
	pNewStateGroup.add(pNewTimeColor.set("Color Time", 3));
	
	pNewTimeText.addListener(this, &Layer::newTimeTextChanged);
	pNewStateGroup.add(pNewTimeText.set("Text Time", 5));
	
	pNewTimeBlack.addListener(this, &Layer::newTimeBlackChanged);
	pNewStateGroup.add(pNewTimeBlack.set("Black Time", 3));
	
	pParameters.add(pNewStateGroup);
	
	pUseCycle.addListener(this, &Layer::useCycleChanged);
	pCycleGroup.add(pUseCycle.set("Use Cycle", true));
	
	pCycleNum.addListener(this, &Layer::cycleNumChanged);
	pCycleGroup.add(pCycleNum.set("Cycle Num", 1));
	
	pParameters.add(pCycleGroup);
}

void Layer::randomTimeImageChanged(glm::vec2 & time) {
	for (auto& cell : mCells) {
		cell->mRandomTimeImageRange = time;
	}
}

void Layer::randomTimeColorChanged(glm::vec2 & time) {
	for (auto& cell : mCells) {
		cell->mRandomTimeColorRange = time;
	}
}

void Layer::randomTimeTextChanged(glm::vec2 & time) {
	for (auto& cell : mCells) {
		cell->mRandomTimeTextRange = time;
	}
}

void Layer::randomTimeBlackChanged(glm::vec2 & time) {
	for (auto& cell : mCells) {
		cell->mRandomTimeBlackRange = time;
	}
}

void Layer::newTimeImageChanged(float & time) {
	for (auto& cell : mCells) {
		cell->mNewTimeImage = time;
	}
}

void Layer::newTimeColorChanged(float & time) {
	for (auto& cell : mCells) {
		cell->mNewTimeColor = time;
	}
}

void Layer::newTimeTextChanged(float & time) {
	for (auto& cell : mCells) {
		cell->mNewTimeText = time;
	}
}

void Layer::newTimeBlackChanged(float & time) {
	for (auto& cell : mCells) {
		cell->mNewTimeBlack = time;
	}
}

void Layer::useCycleChanged(bool & cycle) {
	for (auto& cell : mCells) {
		cell->mUseCycle = cycle;
	}
}

void Layer::cycleNumChanged(int & num) {
	for (auto& cell : mCells) {
		cell->setCycleNum(num);
	}
}
