#version 150

// Combined our old stuff with the shader from Angel book

in vec4 vPosition;
in vec4 vNormal;
uniform mat4 mM;
uniform mat4 mV;
uniform mat4 mP;
uniform mat4 mR;

uniform vec4 lightPosition;	// NEW!

out vec3 fN;				// NEW!  These go to the fragment shader for interpolation
out vec3 fL;

void main () {

	fN = (mR*vNormal).xyz;
	fL = (lightPosition).xyz;		// In world space

	gl_Position = mP*mV*mM*vPosition;
}