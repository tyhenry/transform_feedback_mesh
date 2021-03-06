#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground( 0 );
	ofDisableArbTex();
	ofSetVerticalSync( false );

	// Load transform feedback shader.
	ofShader::TransformFeedbackSettings settings;
	settings.shaderFiles[GL_VERTEX_SHADER] = ofToDataPath( "transformShader.vert", true );
	//settings.shaderFiles[GL_GEOMETRY_SHADER] = ofToDataPath( "transformShader.geom", true );
	settings.bindDefaults      = false;
	settings.varyingsToCapture = { "vPosition", "vNormal" };  // vPosition = vec3 (could also capture "vTexCoord" but tex coords should be static)
	//settings.bufferMode                    = GL_SEPARATE_ATTRIBS;		// either GL_INTERLEAVED_ATTRIBS or GL_SEPARATE_ATTRIBS: choose memory layout of captured attributes (ABABAB vs. AAABBB)
	if ( !transformShader.setup( settings ) ) {
		ofLogError( "ofApp" ) << "Error loading transformShader.vert!";
	}

	// setup vbos

	// input vbo
	size_t nVerts = vboDims.x * vboDims.y;
	std::vector<glm::vec3> verts( nVerts );                                   // just init a bunch of points (at 0,0,0 - they'll be positioned by vertex shader)
	transformInputVbo.setVertexData( verts.data(), nVerts, GL_STATIC_DRAW );  // GL_STATIC_DRAW is GPU hint, not precise, see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml

	// output capture buffer
	// allocate buffer to # bytes (n points x size)
	size_t bufSz = nVerts * sizeof( glm::vec3 ) * 2;          // "vPosition", "vNormal" = vec3 * 2
	transformOutputBuffer.allocate( bufSz, GL_STREAM_COPY );  // GL_STREAM_COPY hint: we'll copy attributes from GL and use for subsequent draw

	// init vbo tex coords
	std::vector<glm::vec2> texCoords( nVerts );
	for ( int i = 0; i < nVerts; ++i ) {
		texCoords[i] = { i % vboDims.x, i / vboDims.x };  // pixel/grid coords
		if ( !ofGetUsingArbTex() ) {
			texCoords[i] /= glm::vec2( vboDims );  // normalized coords
		}
	}
	vbo.setTexCoordData( texCoords.data(), nVerts, GL_STATIC_DRAW );  // set these directly on our vbo mesh, they won't change
	auto mesh = ofMesh::plane( 0, 0, vboDims.x, vboDims.y, OF_PRIMITIVE_TRIANGLES );
	vbo.setIndexData( mesh.getIndices().data(), mesh.getNumIndices(), GL_STREAM_DRAW );

	// setup tex coord visualization shader
	uvVisShader.setupShaderFromSource( GL_VERTEX_SHADER, uvVisVertShader );
	uvVisShader.setupShaderFromSource( GL_FRAGMENT_SHADER, uvVisFragShader );
	uvVisShader.bindDefaults();  // binds default oF programmable renderer attributes (position, color, normal, texcoord)
	uvVisShader.linkProgram();
}

//--------------------------------------------------------------
void ofApp::update()
{

	// Run transform feedback.
	transformShader.beginTransformFeedback( GL_POINTS, transformOutputBuffer );
	{
		transformShader.setUniform2i( "uFrameSize", vboDims.x, vboDims.y );
		transformShader.setUniform3f( "uNoiseFrequency", noiseFrequency );
		transformShader.setUniform1f( "uNoiseAmplitude", noiseAmplitude );
		transformShader.setUniform1f( "uTime", ofGetElapsedTimef() );
		transformInputVbo.draw( GL_POINTS, 0, transformInputVbo.getNumVertices() );
	}
	transformShader.endTransformFeedback( transformOutputBuffer );
	auto vec3Size = sizeof( glm::vec3 );
	vbo.setVertexBuffer( transformOutputBuffer, 3, vec3Size * 2, 0 );      // stride, offset
	vbo.setNormalBuffer( transformOutputBuffer, vec3Size * 2, vec3Size );  // stride, offset
}

//--------------------------------------------------------------
void ofApp::draw()
{

	cam.begin();
	ofEnableDepthTest();
	ofPushStyle();

	uvVisShader.begin();
	//vbo.draw( GL_POINTS, 0, vbo.getNumVertices() );
	vbo.drawElements( GL_POINTS, vbo.getNumIndices() );
	uvVisShader.end();

	ofPopStyle();
	ofDrawAxis( 100 );
	ofDisableDepthTest();
	cam.end();

	std::stringstream data;
	data << "noiseFrequency (adjust (-) 1,2,3 / (+) 4,5,6): " << noiseFrequency << std::endl
	     << "noiseAmplitude (adjust - / + ): " << noiseAmplitude << std::endl
	     << "framerate: " << ofToString( ofGetFrameRate(), 1. );
	ofDrawBitmapStringHighlight( data.str(), 10, 10 );
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{

	if ( key >= '1' && key <= '6' ) {
		int k = key - '1';
		if ( k < 3 ) {
			noiseFrequency[k] -= .001;
		} else {
			k -= 3;
			noiseFrequency[k] += .001;
		}
	} else if ( key == '-' || key == '_' ) {
		noiseAmplitude -= .5;
	} else if ( key == '+' || key == '=' ) {
		noiseAmplitude += .5;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{
	if ( key == 'r' ) {
		// hot reload shader
		ofShader::TransformFeedbackSettings settings;
		settings.shaderFiles[GL_VERTEX_SHADER] = ofToDataPath( "transformShader.vert", true );
		settings.bindDefaults                  = false;
		settings.varyingsToCapture             = { "vPosition" };  // vec3 (could also capture "vTexCoord" but should be static)
		if ( !transformShader.setup( settings ) ) {
			ofLogError( "ofApp" ) << "Error loading transformShader.vert!";
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseExited( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h )
{
}

//--------------------------------------------------------------
void ofApp::gotMessage( ofMessage msg )
{
}

//--------------------------------------------------------------
void ofApp::dragEvent( ofDragInfo dragInfo )
{
}
