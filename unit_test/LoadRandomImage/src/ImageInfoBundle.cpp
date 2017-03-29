//
//  ImageInfoBundle.cpp
//  Layer
//
//  Created by Ean Jee on 16/03/2017.
//
//

#include "ImageInfoBundle.hpp"
#include "ImageProcesser.hpp"

ImageMeta::ImageMeta(const std::string& path, const std::string text, bool isUrl) :
mPath(path), mText(text), mIsUrl(isUrl) {
}

ImageMetaBundle::ImageMetaBundle() {
}

ImageMetaBundle::~ImageMetaBundle() {
}

void ImageMetaBundle::add(const std::string& path, const std::string text, bool isUrl) {
	mImageMetas.push_back(ImageMeta(path, text, isUrl));
}

ImageInfo::ImageInfo() {
	mText = "";
	mIsLoaded = false;
	mImageProcesser = std::make_shared<ImageProcesser>();
	mAvgColor = ofColor(0, 0, 255);
}

ImageInfo::~ImageInfo() {
}

ImageInfoBundle::ImageInfoBundle() {
	ofAddListener(ofEvents().update, this, &ImageInfoBundle::update);
	mEventSent = false;
	mLoading = false;
}

ImageInfoBundle::~ImageInfoBundle() {
	ofRemoveListener(ofEvents().update, this, &ImageInfoBundle::update);
}

void ImageInfoBundle::load(const ImageMetaBundle& metaBundle) {
	load(metaBundle.mImageMetas);
}

void ImageInfoBundle::load(const std::vector<ImageMeta>& metas) {
	std::lock_guard<std::mutex> guard(mMutex);
	if (!isLoading()) {
		mLoading = true;
		mImageInfos.clear();
		for (auto& meta : metas) {
			
			std::shared_ptr<ImageInfo> imgInfo = std::make_shared<ImageInfo>();
			imgInfo->mText = meta.mText;
			mImageInfos.push_back(imgInfo);
			
			if (meta.mIsUrl) {
				mLoader.loadFromURL(imgInfo->mImage, meta.mPath);
			}
			else {
				mLoader.loadFromDisk(imgInfo->mImage, meta.mPath);
			}
		}
		
		mEventSent = false;
	}
	else {
		ofLogNotice("ImageInfoBundle") << "already loading";
	}
}

bool ImageInfoBundle::isLoaded() {
	bool ret = false;
	if (mImageInfos.size() > 0) {
		ret = true;
		for (auto& imgInfo : mImageInfos) {
			if (!imgInfo->mIsLoaded) {
				ret = false;
				break;
			}
		}
	}
	
	return ret;
}

bool ImageInfoBundle::isLoading() {
	return mLoading;
}

void ImageInfoBundle::update(ofEventArgs & arg) {
	std::lock_guard<std::mutex> guard(mMutex);
	
	for (auto& imgInfo : mImageInfos) {
		if (!imgInfo->mImageProcesser->isThreadRunning() && !imgInfo->mIsLoaded &&
			imgInfo->mImage.isUsingTexture()) {
			ofLogNotice("ImageInfoBundle") << "image downloaded";
			imgInfo->mImageProcesser->process(imgInfo);
		}
	}
	
	if (!mEventSent && isLoaded()) {
		ofNotifyEvent(mLoadedEvent, this);
		mEventSent = true;
		mLoading = false;
	}
}
