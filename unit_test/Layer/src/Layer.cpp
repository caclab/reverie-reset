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
		mCells[i].setup(glm::vec2(0), mCellSize, mImageInfoBundle, startIndex, 2, 2, 3);
		mCells[i].cycle();
	}
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

void Layer::load(const std::vector<ImageMeta> metas) {
	if (isCellFinished()) {
		mImageInfoBundle->load(metas);
	}
	else {
		ofLogNotice("Layer") << "cycling... loading unavailable";
	}
}
