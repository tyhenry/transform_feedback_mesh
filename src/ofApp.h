#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();

	void keyPressed( int key );
	void keyReleased( int key );
	void mouseMoved( int x, int y );
	void mouseDragged( int x, int y, int button );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseEntered( int x, int y );
	void mouseExited( int x, int y );
	void windowResized( int w, int h );
	void dragEvent( ofDragInfo dragInfo );
	void gotMessage( ofMessage msg );

	ofShader transformShader;
	ofVbo vbo, transformInputVbo;
	ofBufferObject transformOutputBuffer;  // output capture buffer
	glm::ivec2 vboDims = { 1024, 1024 };
	ofEasyCam cam;

	glm::vec3 noiseFrequency = glm::vec3(.01,.01, .1);
	float noiseAmplitude = 100.;

	ofMesh mesh;
};
