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
#include "Cell.hpp"
#include <vector>

class Layer {
public:
	Layer();
	~Layer();
	
	void setup(int cellNum, glm::vec2 size);

	void update();
	void draw();
	void draw(int cellIndex);

	void load(const std::vector<ImageMeta> metas);
	void onLoaded();

	bool isCellFinished();

	std::vector<Cell> mCells;
	glm::vec2 mCellSize;
	std::shared_ptr<ImageInfoBundle> mImageInfoBundle;
	
	ofImage img;
};

#endif /* Layer_hpp */
