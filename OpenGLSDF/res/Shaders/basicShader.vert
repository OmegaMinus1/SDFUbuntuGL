#version 450

//FS Tri Input
attribute vec3 position;
attribute vec2 texCoord;

//BB did this
varying vec2 texCoord0;

//Don't really need to be here
uniform float iTime;
uniform vec2 iResolution;

void main()
{
	//Posthrough
	gl_Position = vec4(position, 1.0);
	
	//Texthrough
	//BB why not pass as non varying
	texCoord0 = texCoord;
}
