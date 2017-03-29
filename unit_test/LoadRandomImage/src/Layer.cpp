//
//  Layer.hpp
//  Layer
//
//  Created by Ean Jee on 17/03/2017.
//
//

#include "Layer.hpp"

Layer::Layer() {
	mImageInfoBundle = std::make_shared<ImageInfoBundle>();

	ofAddListener(mImageInfoBundle->mLoadedEvent, this, &Layer::onLoaded);
}

Layer::~Layer() {
	ofRemoveListener(mImageInfoBundle->mLoadedEvent, this, &Layer::onLoaded);
}

void Layer::setup(int cellNum, glm::vec2 size) {
	mCells.clear();
	mCells.resize(cellNum);
	mCellSize = size;
}

void Layer::onLoaded() {
	ofLogNotice("Layer") << "all cells loaded";

	for (int i = 0; i < mCells.size(); i++) {
		int startIndex = i % mImageInfoBundle->mImageInfos.size();
		mCells[i].setup(glm::vec2(0), mCellSize, mImageInfoBundle, startIndex, 1, 1, 1, 1);
	}
	
	// clear image meta bundle
	mImageMetaBundle.mImageMetas.clear();
}

bool Layer::isCellFinished() {
	bool ret = true;
	for (auto& cell : mCells) {
		if (!cell.isFinished()) {
			ret = false;
			break;
		}
	}

	return ret;
}

void Layer::update() {
	for (auto& cell : mCells) {
		cell.update();
	}
	
	loadMetaBundle();
}

void Layer::draw() {
	for (auto& cell : mCells) {
		cell.draw();
	}
}

void Layer::draw(int cellIndex) {
	if (cellIndex >= 0 && cellIndex < mCells.size()) {
		mCells[cellIndex].draw();
	} else {
		ofLogError("Layer") << "draw with invalid cell index";
	}
}

void Layer::load(const ImageMetaBundle& metaBundle) {
	if (metaBundle.mImageMetas.size() > 0) {
		mImageMetaBundle = metaBundle;
		if (isCellFinished()) {
			ofLogNotice("Layer") << "cycle finished, load image meta bundle";
		}
		else {
			ofLogNotice("Layer") << "cycling... save image meta bundle, load later";
		}
	}
}

void Layer::loadMetaBundle() {
	// all cell finished and image meta bundle is new
	if (isCellFinished() && mImageMetaBundle.mImageMetas.size() > 0) {
		mImageInfoBundle->load(mImageMetaBundle);
	}
}
