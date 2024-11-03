#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofFill();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofColor color;
	for (int i = 0; i < 4; i++) {

		auto noise_location = glm::vec2(
			ofMap(ofNoise(39 + i * 5000, ofGetFrameNum() * 0.01), 0, 1, -360 + (int)(i / 2) * 360, 0 + (int)(i / 2) * 360),
			ofMap(ofNoise(239 + i * 5000, ofGetFrameNum() * 0.01), 0, 1, -360 + (int)(i % 2) * 360, 0 + (int)(i % 2) * 360));

		auto next_noise_location = glm::vec2(
			ofMap(ofNoise(39 + i * 5000, (ofGetFrameNum() + 1) * 0.01), 0, 1, -360 + (int)(i / 2) * 360, 0 + (int)(i / 2) * 360),
			ofMap(ofNoise(239 + i * 5000, (ofGetFrameNum() + 1) * 0.01), 0, 1, -360 + (int)(i % 2) * 360, 0 + (int)(i % 2) * 360));
		
		for (int k = 0; k < 2; k++) {

			vector<glm::vec2> log;
			log.push_back(noise_location);
			this->log_list.push_back(log);

			auto deg = glm::atan(next_noise_location.y - noise_location.y, next_noise_location.x - noise_location.x) * RAD_TO_DEG + 180 + ofRandom(-35, 35);
			glm::vec2 velocity = glm::vec2(cos(deg * DEG_TO_RAD), sin(deg * DEG_TO_RAD));
			this->velocity_list.push_back(velocity);

			this->noise_seed_list.push_back(ofRandom(1000));

			color.setHsb(ofRandom(i * 60, 60 + i * 60), 255, 255);
			this->color_list.push_back(color);
		}
	}

	for (int i = 0; i < this->log_list.size(); i++) {

		auto future = this->velocity_list[i] * 200;
		auto random_deg = ofMap(ofNoise(glm::vec2(this->noise_seed_list[i], ofGetFrameNum() * 0.01)), 0, 1, 0, 360);
		future += glm::vec2(50 * cos(random_deg * DEG_TO_RAD), 50 * sin(random_deg * DEG_TO_RAD));

		auto next = this->log_list[i].back() + glm::normalize(future) * 6;
		next = glm::length(next) < 1000 ? next : glm::normalize(next) * 1000;
		this->log_list[i].push_back(next);

		if (this->log_list[i].size() > 30) {

			this->log_list.erase(this->log_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->noise_seed_list.erase(this->noise_seed_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	ofGetFrameNum() % 100 < 50 ? ofFill() : ofNoFill();
	for (int i = 0; i < this->log_list.size(); i++) {

		ofPushMatrix();
		ofTranslate(this->log_list[i].back());

		ofSetColor(this->color_list[i]);
		auto size = this->log_list[i].size() < 3 ? 20 : ofMap(this->log_list[i].size(), 3, 30, 20, 1);
		ofDrawCircle(glm::vec3(), size);

		ofPopMatrix();
	}

	/*
	int start = 100;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}