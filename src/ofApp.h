#pragma once
#include "ofMain.h"

const std::string uvVisVertShader = R"glsl(
#version 150 

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4  position;
in vec2  texcoord;
in vec4  color;
in vec3  normal;

out vec4 colorVarying;
out vec2 texCoordVarying;
out vec4 normalVarying;

void main()
{
	colorVarying = color;
	texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
	gl_Position = modelViewProjectionMatrix * position;
}

)glsl";

const std::string uvVisFragShader = R"glsl(

#version 150

in vec4 colorVarying;
in vec2 texCoordVarying;
in vec4 normalVarying;

out vec4 fragColor;

void main(){
	fragColor = vec4( texCoordVarying, 0., 1. );
}

)glsl";

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
	glm::ivec2 vboDims = { 2048, 2048 };
	ofEasyCam cam;

	glm::vec3 noiseFrequency = glm::vec3(.01,.01, .1);
	float noiseAmplitude = 100.;

	ofShader uvVisShader;
};
