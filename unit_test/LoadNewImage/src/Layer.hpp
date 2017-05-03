//
//  Layer.hpp
//  Layer
//
//  Created by Ean Jee on 17/03/2017.
//
//

#ifndef Layer_hpp
#define Layer_hpp

#include "ImageInfoBundle.hpp"

class Layer {
public:
	Layer();
	~Layer();
	
	void setup(int cellNum, glm::vec2 size);

	void update();
	void draw();
	void draw(int cellIndex);
	
	void loadRandom(const ImageMetaBundle& metaBundle);
	void loadNew(const ImageMetaBundle& metaBundle);
	void onRandomLoaded();
	void onNewLoaded();
	
	void boom();

	bool isCellFinished();
	
	//protected:
	
	void sendReady();
	void sendStandby();

	std::vector<std::shared_ptr<class Cell>> mCells;
	// for random image
	std::shared_ptr<class BundleBuffer> mRandomBundleBuffer;
	// for new image
	std::shared_ptr<class BundleBuffer> mNewBundleBuffer;

	std::shared_ptr<class ofxCenteredTrueTypeFont> mFont;
	
	bool mReadySent;
	ofEvent<void> mReadyEvent;
	
	bool mStandbySent;
	ofEvent<void> mStandbyEvent;
	
	// deserialize
	ofParameterGroup pParameters;
	ofParameter<float> pTimeImage;
	ofParameter<float> pTimeColor;
	ofParameter<float> pTimeText;
	ofParameter<float> pTimeBlack;
	
	void timeImageChanged(float & time);
	void timeColorChanged(float & time);
	void timeTextChanged(float & time);
	void timeBlackChanged(float & time);
};

#endif /* Layer_hpp */
