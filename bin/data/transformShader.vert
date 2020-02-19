
#version 150
#pragma include "simplexNoise.glsl"
// -----------------------------------------------------------------------
// Azure Kinect point cloud - vertex shader
// * transform vertices from depth texture -> world map -> world space
// adapted by @tyhenry from https://github.com/prisonerjohn/ofxAzureKinect/tree/master/example-shader
// -----------------------------------------------------------------------

in vec4 position;// oF
in vec3 normal; // oF
uniform mat4 modelViewMatrix;// oF
uniform mat4 modelViewProjectionMatrix;// oF

uniform ivec2 uFrameSize;// vbo dims
uniform float uTime = 0.;
uniform vec3 uNoiseFrequency = vec3(1.);
uniform float uNoiseAmplitude = 1.;

out vec3 vPosition;// world space position
out vec3 vNormal;
//out float vZTopLeft;
//out float vZTopRight;
//out float vZBottomLeft;
out vec2 vTexCoord;// color UV map coords

float getZ( vec2 id ){
    return (snoise( vec3(id, uTime) * uNoiseFrequency ) * 2. - 1. ) * uNoiseAmplitude; // signed noise
}

void main()
{
    vec2 id = vec2(gl_VertexID % uFrameSize.x, gl_VertexID / uFrameSize.x);
    id -= vec2(uFrameSize) * .5;
    float z = getZ( id );
    // vzTopLeft = getZ( id - vec2(1,1) );
    // vzTopRight = getZ( id - vec2(0,1));
    // vzBottomLeft = getZ( id - vec2(1,0));
    vPosition = vec3( id, z);
    // calc normals
    // https://stackoverflow.com/questions/34644101/calculate-surface-normals-from-depth-image-using-neighboring-pixels-cross-produc
    float dx	= .5 * ( getZ( id + vec2(1,0) ) - getZ( id + vec2(-1,0)) );
    float dy	= .5 * ( getZ( id + vec2(0,1) ) - getZ( id + vec2(0,-1)) );
    vec3 d		= vec3(-dx, -dy, 1.);
    vNormal		= normalize(d);
    gl_Position=modelViewProjectionMatrix* vec4(vPosition,1.);
}