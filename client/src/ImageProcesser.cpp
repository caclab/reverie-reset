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
	if (auto imageInfo = mImageInfo.lock()) {
		averageColor(imageInfo);
		ofLogNotice("ImageProcesser") << "done: " << imageInfo->mText;
	} else {
		ofLogNotice("ImageProcesser") << "ImageInfo not available";
	}
}

void ImageProcesser::averageColor(std::shared_ptr<class ImageInfo> imageInfo) {
	if (imageInfo) {
		int red = 0;
		int green = 0;
		int blue = 0;
		
		int size = imageInfo->mImage.getWidth() * imageInfo->mImage.getHeight();
		if (size > 0) {
			for (int col = 0; col < imageInfo->mImage.getWidth(); col++) {
				for (int row = 0; row < imageInfo->mImage.getHeight(); row++) {
					red += (int)imageInfo->mImage.getColor(col, row).r;
					green += (int)imageInfo->mImage.getColor(col, row).g;
					blue += (int)imageInfo->mImage.getColor(col, row).b;
				}
			}
			
			red /= size;
			green /= size;
			blue /= size;
		}
		
		imageInfo->mAvgColor = ofColor(red, green, blue);
		imageInfo->mIsLoaded = true;
	}
}
