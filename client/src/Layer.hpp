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
	void initConfig();
	ofParameterGroup pParameters;
	ofParameterGroup pRandomStateGroup;
	ofParameterGroup pNewStateGroup;
	ofParameterGroup pCycleGroup;
	
	ofParameter<glm::vec2> pRandomTimeImage;
	ofParameter<glm::vec2> pRandomTimeColor;
	ofParameter<glm::vec2> pRandomTimeText;
	ofParameter<glm::vec2> pRandomTimeBlack;
	
	ofParameter<float> pNewTimeImage;
	ofParameter<float> pNewTimeColor;
	ofParameter<float> pNewTimeText;
	ofParameter<float> pNewTimeBlack;
	
	ofParameter<bool> pUseCycle;
	ofParameter<int> pCycleNum;
	
	void randomTimeImageChanged(glm::vec2 & time);
	void randomTimeColorChanged(glm::vec2 & time);
	void randomTimeTextChanged(glm::vec2 & time);
	void randomTimeBlackChanged(glm::vec2 & time);

	void newTimeImageChanged(float & time);
	void newTimeColorChanged(float & time);
	void newTimeTextChanged(float & time);
	void newTimeBlackChanged(float & time);
	
	void useCycleChanged(bool & cycle);
	void cycleNumChanged(int & num);
};

#endif /* Layer_hpp */
