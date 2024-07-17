#pragma once

#include "ofMain.h"
#include "ofxGui.h" // Include ofxGui addon

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void loadPoints(const std::string& fileName);
    void loadVectors(const std::string& fileName);
    void calculateScaling();

    std::vector<glm::vec2> points; // 2Dポイント
    std::vector<glm::vec3> vectors3D; // 3Dベクトルデータ
    std::vector<float> vectorLengths; // ベクトルの長さ
    float pointScaling; // 座標のスケーリング値
    float vectorScaling; // ベクトルのスケーリング値

    float maxLength; // ベクトルの最大長さ
    ofEasyCam cam; // ofEasyCamのインスタンス

    ofxPanel gui; // GUIパネル
    ofxFloatSlider vectorScalingSlider; // ベクトルスケーリングのスライダー
};

