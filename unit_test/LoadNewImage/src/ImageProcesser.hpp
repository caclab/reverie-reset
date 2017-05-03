//
//  ImageProcesser.hpp
//  Layer
//
//  Created by Ean Jee on 17/03/2017.
//
//

#ifndef ImageProcesser_hpp
#define ImageProcesser_hpp

#include "ofMain.h"
#include <memory>

class ImageProcesser : public ofThread {
public:
	ImageProcesser();
	~ImageProcesser();

	void process(std::shared_ptr<class ImageInfo> imageInfo);

	void threadedFunction();
	
	void averageColor();

	std::shared_ptr<class ImageInfo> mImageInfo;
};


#endif /* ImageProcesser_hpp */
