#include "Render.h"

//--------------------------------------------------------------
Render::Render(){

}

//--------------------------------------------------------------
void Render::setup(int ts, int rx, int ry, int px, int py){
    totalScreens = ts;
    // imgPath = path;
    resX = rx;
    resY = ry;
    posX = px;
    posY = py;

    images.resize(totalScreens);

    for(int i = 0; i < images.size(); i++) {
        imgPath.push_back("images/" + ofToString(i) + ".png");
        loader.loadFromDisk(images[i], imgPath[i]);
        // loader.loadFromURL(images[i*2+1], "http://www.openframeworks.cc/of_inverted.png");
        bool rs = false;
        int val = ofRandom(0,100);
        if(val>50){
            rs = true;
        }
        int ti = ofRandom(5000, 15000); 
        imgSwitches.push_back(rs);
        startTime.push_back(ofGetElapsedTimeMillis());
        timeInterval.push_back((unsigned int)ti);
    }
    
}

//--------------------------------------------------------------
ofColor Render::averageColor(ofImage img){
    // avrColors.clear();
    int total = resX*resY;
    ofColor c;
    // for(int i=0; i<images.size(); i++) {
        int red = 0;
        int green = 0;
        int blue = 0;
        for(int y=0; y<resY; y++) {
            for(int x=0; x<resX; x++) {
                // ofColor colorAtXY = images[i].getColor(x, y);
                ofColor colorAtXY = img.getColor(x, y);
                red += int(colorAtXY.r);
                green += int(colorAtXY.g);
                blue += int(colorAtXY.b);
                //cout<< int(colorAtXY.r)<< endl;
            }
        }
        int avrRed = red/total;
        int avrGreen = green/total;
        int avrBlue = blue/total;
        c.set(avrRed,avrGreen,avrBlue);

    return c;
        // avrColors.push_back(c);
    // }
    //ofSleepMillis(1000);

    // avrColorsByLines.clear();
    // for(int i=0; i<images.size(); i++) {
    //     int red = 0;
    //     int green = 0;
    //     int blue = 0;
    //     for(int y=0; y<resY; y++) {
    //         for(int x=0; x<resX; x++) {
    //             ofColor colorAtXY = images[i].getColor(x, y);
    //             red += int(colorAtXY.r);
    //             green += int(colorAtXY.g);
    //             blue += int(colorAtXY.b);
    //             //cout<< int(colorAtXY.r)<< endl;
    //         }
    //         int avrRed = red/resX;
    //         int avrGreen = green/resX;
    //         int avrBlue = blue/resX;
    //         c.set(avrRed,avrGreen,avrBlue);
    //         avrColors.push_back(c);
    //     }  
    //     avrColorsByLines.push_back(avrColors); 
    // }
}

//--------------------------------------------------------------
void Render::update(){
    //Timer interval to load new images
    for(int i = 0; i < images.size(); i++) {
        uint64_t now = ofGetElapsedTimeMillis();
        if (now - startTime[i] > timeInterval[i]) {
            imgSwitches[i] = !imgSwitches[i];
            startTime[i] = ofGetElapsedTimeMillis(); //reset timer
        }
    }
}

//--------------------------------------------------------------
void Render::draw(){
    // draw the images OR the average color.
    for(int i = 0; i < images.size(); i++) {
        if(images[i].isAllocated()){
            if(!imgSwitches[i]){ //draw the image
                ofSetColor(255);
                images[i].draw(resX*i, posY, resX, resY);
            }else{
                ofFill();
                ofColor avrColor = averageColor(images[i]);
                ofSetColor(avrColor);
                ofRect(resX*i, posY, resX, resY);
            }   
        }
    }
}

//--------------------------------------------------------------
void Render::exit(){
    loader.stopThread();
}

//--------------------------------------------------------------

