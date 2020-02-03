
#version 150
#pragma include "simplexNoise.glsl"
// -----------------------------------------------------------------------
// Azure Kinect point cloud - vertex shader
// * transform vertices from depth texture -> world map -> world space
// adapted by @tyhenry from https://github.com/prisonerjohn/ofxAzureKinect/tree/master/example-shader
// -----------------------------------------------------------------------

in vec4 position;// oF
uniform mat4 modelViewMatrix;// oF
uniform mat4 modelViewProjectionMatrix;// oF

uniform ivec2 uFrameSize;// vbo dims
uniform float uTime = 0.;
uniform vec3 uNoiseFrequency = vec3(1.);
uniform float uNoiseAmplitude = 1.;

out vec3 vPosition;// world space position
// out float vZTopLeft;
// out float vZTopRight;
// out float vZBottomLeft;
// out vec2 vTexCoord;// color UV map coords

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
    gl_Position=modelViewProjectionMatrix* vec4(vPosition,1.);
}