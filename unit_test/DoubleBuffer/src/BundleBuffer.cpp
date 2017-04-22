//
//  BundleBuffer.cpp
//  DoubleBuffer
//
//  Created by Ean Jee on 20/04/2017.
//
//

#include "BundleBuffer.hpp"
#include "Cell.hpp"

BundleBuffer::BundleBuffer() {
	mPing = std::make_shared<ImageInfoBundle>();
	mPong = std::make_shared<ImageInfoBundle>();
	
	mCurrent = mPing;
	mFlippable = false;
	
	ofAddListener(mPing->mLoadedEvent, this, &BundleBuffer::onLoaded);
	ofAddListener(mPong->mLoadedEvent, this, &BundleBuffer::onLoaded);
}

BundleBuffer::~BundleBuffer() {
	ofRemoveListener(mPing->mLoadedEvent, this, &BundleBuffer::onLoaded);
	ofRemoveListener(mPong->mLoadedEvent, this, &BundleBuffer::onLoaded);
}

void BundleBuffer::addUser(std::shared_ptr<Cell> user) {
	mUsers.push_back(user);
}

void BundleBuffer::update() {
	loadMetaBundle();
}

void BundleBuffer::load(const ImageMetaBundle& metaBundle) {
	if (mLoading) {
		ofLogNotice("BundleBuffer") << "already loading, ignored";
	} else {
		if (metaBundle.mImageMetas.size() > 0) {
			mImageMetaBundle = metaBundle;
			if (isAllUserFlipped()) {
				ofLogNotice("BundleBuffer") << "all user flipped, load image meta bundle";
			}
			else {
				ofLogNotice("BundleBuffer") << "user flipping... save image meta bundle, load later";
			}
		}
	}
}

void BundleBuffer::onLoaded() {
	ofLogNotice("BundleBuffer") << "all meta loaded";
	
	// clear image meta bundle
	mImageMetaBundle.mImageMetas.clear();
	mLoading = false;
	
	// flip ping and pong
	flip();
	
	// notify user to flip
	mFlippable = true;
	for (auto& user : mUsers) {
		user->mGoFlip = true;
	}
	
	ofNotifyEvent(mLoadedEvent, this);
}

std::shared_ptr<ImageInfoBundle> BundleBuffer::getCurrent() {
	return mCurrent;
}

std::shared_ptr<ImageInfoBundle> BundleBuffer::getNext() {
	return getFlipped(mCurrent);
}

bool BundleBuffer::isFlippable() {
	return mFlippable;
}

bool BundleBuffer::isAllUserFlipped() {
	bool ret = true;
	for (auto& user : mUsers) {
		if (user->mImageInfoBundle != getNext()) {
			ret = false;
			break;
		}
	}
	
	return ret;
}

void BundleBuffer::loadMetaBundle() {
	// all user flipped and image meta bundle is new
	if (!mLoading && isAllUserFlipped() &&
		mImageMetaBundle.mImageMetas.size() > 0) {
		getCurrent()->load(mImageMetaBundle);
		mLoading = true;
	}
}

void BundleBuffer::flip() {
	mCurrent = getNext();
}

std::shared_ptr<ImageInfoBundle> BundleBuffer::getFlipped(const std::shared_ptr<ImageInfoBundle> current) {
	if (current == mPing) {
		return mPong;
	} else if (current == mPong) {
		return mPing;
	} else {
		ofLogError("BundleBuffer") << "argument should be equal to mPing or mPong";
		return mPing;
	}
}
