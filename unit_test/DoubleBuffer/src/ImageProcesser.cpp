//
//  ImageProcesser.cpp
//  Layer
//
//  Created by Ean Jee on 17/03/2017.
//
//

#include "ImageProcesser.hpp"
#include "ImageInfoBundle.hpp"

ImageProcesser::ImageProcesser() {

}

ImageProcesser::~ImageProcesser() {
	waitForThread(true);
}

void ImageProcesser::process(std::shared_ptr<ImageInfo> imageInfo) {
	mImageInfo = imageInfo;
	startThread();
}

void ImageProcesser::threadedFunction() {
	ofLogNotice("ImageProcesser") << "processing...";
	averageColor();
	ofLogNotice("ImageProcesser") << "done: " << mImageInfo->mText;
}

void ImageProcesser::averageColor() {
	if (mImageInfo) {
		int red = 0;
		int green = 0;
		int blue = 0;
		
		int size = mImageInfo->mImage.getWidth() * mImageInfo->mImage.getHeight();
		if (size > 0) {
			for (int col = 0; col < mImageInfo->mImage.getWidth(); col++) {
				for (int row = 0; row < mImageInfo->mImage.getHeight(); row++) {
					red += (int)mImageInfo->mImage.getColor(col, row).r;
					green += (int)mImageInfo->mImage.getColor(col, row).g;
					blue += (int)mImageInfo->mImage.getColor(col, row).b;
				}
			}
			
			red /= size;
			green /= size;
			blue /= size;
		}
		
		mImageInfo->mAvgColor = ofColor(red, green, blue);
		mImageInfo->mIsLoaded = true;
	}
}
