#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main()
{
	ofGLFWWindowSettings settings;
	settings.setSize( 1280, 800 );
	settings.setGLVersion( 3, 2 );
	settings.setPosition( { 100, 100 } );
	settings.windowMode = OF_WINDOW;
	ofCreateWindow( settings );
	ofRunApp( new ofApp() );
}
