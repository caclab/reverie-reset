//
//  ImageInfoBundle.hpp
//  Layer
//
//  Created by Ean Jee on 16/03/2017.
//
//

#ifndef ImageInfoBundle_hpp
#define ImageInfoBundle_hpp

#include "ofMain.h"
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include "ofxThreadedImageLoader.h"

class ImageMeta {
public:
	std::string mPath;
	std::string mText;
	bool mIsUrl;
};

class ImageInfo {
public:
	ImageInfo();
	~ImageInfo();

	ofImage mImage;
	std::string mText;
	ofColor mAvgColor;
	std::shared_ptr<class ImageProcesser> mImageProcesser;
	bool mIsLoaded;
};

class ImageInfoBundle {
public:
	ImageInfoBundle();
	~ImageInfoBundle();

	void load(const std::vector<ImageMeta> metas);
	bool isLoaded();
	bool isLoading();

	void update(ofEventArgs & arg);
	
	std::vector<std::shared_ptr<ImageInfo>> mImageInfos;
	ofxThreadedImageLoader mLoader;

	std::mutex mMutex;

	ofEvent<void> mLoadedEvent;
	bool mEventSent;
	bool mLoading;
};

#endif /* ImageInfoBundle_hpp */
