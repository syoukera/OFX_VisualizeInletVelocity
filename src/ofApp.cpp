#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // ファイルからデータを読み込む
    // loadPoints("points");
    // loadVectors("U");

    // loadPoints("\\\\CFDGroup18\\Public\\shioyoke\\NH3_HiTAC\\20240709_interpolate\\data\\points");
    // loadVectors("\\\\CFDGroup18\\Public\\shioyoke\\NH3_HiTAC\\20240709_interpolate\\data\\U");

    // loadPoints("\\\\CFDGroup18\\Public\\shioyoke\\NH3_HiTAC\\20240709_interpolate\\output\\X_interpolated");
    // loadVectors("\\\\CFDGroup18\\Public\\shioyoke\\NH3_HiTAC\\20240709_interpolate\\output\\U_interpolated");

    loadPoints("\\\\CFDGroup18\\Public\\shioyoke\\NH3_HiTAC\\20240709_interpolate\\output\\X_interpolated");
    
    // 時刻ステップのフォルダ名を読み込む
    std::vector<std::string> timeSteps = loadTimeSteps("\\\\CFDGroup18\\Public\\shioyoke\\NH3_HiTAC\\20240709_interpolate\\data\\list_foldernames");
 
    // for (size_t i = 0; i < timeSteps.size(); i++) {
    //     std::cout << timeSteps[i] << std::endl;
    // }

    // 最大時刻ステップを設定
    maxTimeStep = timeSteps.size() - 1;
    
    loadVectors("\\\\CFDGroup18\\Public\\shioyoke\\NH3_HiTAC\\fuel_Xinterp\\0.04404993\\U");

    // 座標のスケーリング値を計算
    calculateScaling();

    // GUIパネルを設定
    gui.setup(); // デフォルト位置でのGUIパネルの設定
    gui.add(vectorScalingSlider.setup("Vector Scaling", 1.0, 0.0, 100.0)); // スライダーの設定

    // 3Dビューのためにデプステストを有効化
    // ofEnableDepthTest();
    ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update(){
    // ベクトルのスケーリング値を更新
    // vectorScaling = ofGetWidth() / 100.0 * vectorScalingSlider / maxLength; // ベクトルの長さを適切にスケーリングする係数を調整
    vectorScaling = ofGetWidth() / 100.0 * vectorScalingSlider / 100.0; // ベクトルの長さを適切にスケーリングする係数を調整
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);

    // カメラの視点を開始
    cam.begin();

    for(size_t i = 0; i < points.size(); ++i) {
        // スケーリングを適用
        glm::vec2 adjustedPoint = points[i] * pointScaling;
        glm::vec3 adjustedVector = vectors3D[i] * vectorScaling;

        // ベクトルの長さを計算
        float vectorLength = glm::length(vectors3D[i]);
        // float hue = ofMap(vectorLength, 0, maxLength, 240, 0); // 長さに応じてHue値を計算（青から赤にグラデーション）
        float hue = ofMap(vectorLength, 0, 100, 240, 0); // 長さに応じてHue値を計算（青から赤にグラデーション）

        // 色を設定
        ofColor color;
        color.setHsb(hue, 255, 255);
        ofSetColor(color);

        // ベクトルを描画
        ofDrawLine(adjustedPoint.x, adjustedPoint.y, 0,
                   adjustedPoint.x + adjustedVector.x, adjustedPoint.y + adjustedVector.y, adjustedVector.z);
    }

    // カメラの視点を終了
    cam.end();

    // GUIパネルを描画
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::loadPoints(const std::string& fileName) {
    ofBuffer buffer = ofBufferFromFile(fileName);
    for(auto line : buffer.getLines()) {
        if(line[0] == '(') {
            glm::vec2 point;
            sscanf(line.c_str(), "(%f %f %*f)", &point.x, &point.y); // Z値を無視
            points.push_back(point);
        }
    }
}

//--------------------------------------------------------------
void ofApp::loadVectors(const std::string& fileName) {
    ofBuffer buffer = ofBufferFromFile(fileName);
    for(auto line : buffer.getLines()) {
        if(line[0] == '(') {
            glm::vec3 vector;
            sscanf(line.c_str(), "(%f %f %f)", &vector.x, &vector.y, &vector.z);
            vectors3D.push_back(vector);

            // ベクトルの長さを計算して保存
            float length = glm::length(vector);
            vectorLengths.push_back(length);
        }
    }
}

//--------------------------------------------------------------
std::vector<std::string> ofApp::loadTimeSteps(const std::string& fileName) {
    std::vector<std::string> timeSteps;
    ofBuffer buffer = ofBufferFromFile(fileName);
    for(auto line : buffer.getLines()) {
        timeSteps.push_back(line);
    }
    return timeSteps;
}

//--------------------------------------------------------------
void ofApp::calculateScaling() {
    // 座標の最大値と最小値を計算
    glm::vec2 minPoint = points[0];
    glm::vec2 maxPoint = points[0];
    for(const auto& point : points) {
        minPoint = glm::min(minPoint, point);
        maxPoint = glm::max(maxPoint, point);
    }

    // 座標のスケーリング値を計算
    glm::vec2 range = maxPoint - minPoint;
    float maxRange = glm::max(range.x, range.y);
    pointScaling = (ofGetWidth() - 40) / maxRange; // ウィンドウ幅に基づくスケーリングファクター（40はマージン）

    // // ベクトルの最大長さを計算
    // maxLength = 0;
    // for(const auto& length : vectorLengths) {
    //     maxLength = glm::max(maxLength, length);
    // }
}
