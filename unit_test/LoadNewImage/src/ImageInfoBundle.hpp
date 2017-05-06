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
	ImageMeta(const std::string& path, const std::string text, bool isUrl);
	std::string mPath;
	std::string mText;
	bool mIsUrl;
};

class ImageMetaBundle {
public:
	ImageMetaBundle();
	~ImageMetaBundle();
	
	void add(const std::string& path, const std::string text, bool isUrl);
	
	std::vector<ImageMeta> mImageMetas;
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
	// for image scale
	glm::vec2 mRenderPos;
	glm::vec2 mRenderSize;
};

class ImageInfoBundle {
public:
	ImageInfoBundle();
	~ImageInfoBundle();

	void load(const ImageMetaBundle& metaBundle);
	void load(const std::vector<ImageMeta>& metas);
	bool isLoaded();
	bool isLoading();

	void update(ofEventArgs & arg);
	
	const float SCREEN_WIDTH = 1280.0f;
	const float SCREEN_HEIGHT = 720.0f;
	
	std::vector<std::shared_ptr<ImageInfo>> mImageInfos;
	ofxThreadedImageLoader mLoader;

	std::mutex mMutex;

	ofEvent<void> mLoadedEvent;
	bool mEventSent;
	bool mLoading;
};

#endif /* ImageInfoBundle_hpp */
