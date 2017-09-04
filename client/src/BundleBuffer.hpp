//
//  BundleBuffer.hpp
//  DoubleBuffer
//
//  Created by Ean Jee on 20/04/2017.
//
//

#ifndef BundleBuffer_hpp
#define BundleBuffer_hpp

#include "ImageInfoBundle.hpp"

class BundleBuffer {
public:
	BundleBuffer();
	~BundleBuffer();
	
	void reset();
	
	void addUser(std::shared_ptr<class Cell> user);
	void update();
	void load(const ImageMetaBundle& metaBundle);
	void onLoaded();
	
	std::shared_ptr<ImageInfoBundle> getCurrent();
	std::shared_ptr<ImageInfoBundle> getNext();
	bool isFlippable();
	
	//protected:
	
	bool isAllUserFlipped();
	void loadMetaBundle();
	void flip();
	std::shared_ptr<ImageInfoBundle> getFlipped(const std::shared_ptr<ImageInfoBundle> current);
	
	std::vector<std::shared_ptr<class Cell>> mUsers;
	std::shared_ptr<ImageInfoBundle> mPing, mPong, mCurrent;
	
	ImageMetaBundle mImageMetaBundle;
	
	bool mLoading;
	bool mFlippable;
	
	ofEvent<void> mLoadedEvent;
};

#endif /* BundleBuffer_hpp */
