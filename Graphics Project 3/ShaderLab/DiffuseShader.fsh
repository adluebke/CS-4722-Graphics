#version 150

in vec3 fN;		// NEW! From the vertex shader (but interpolated)
in vec3 fL;

out vec4 fColor;

uniform vec4 diffuse;	// NEW!!

void main () {
	vec3 N = normalize (fN);	// the Normal
	vec3 L = normalize (fL);	// the Light direction
	
	// Calculate the diffuse part - but don't go negative!
	float Kd = max (dot(N, L), 0.0);	// Intensity
	vec4 dif = Kd*diffuse;				// ... and then color

	// The final color is a combination of all of these
	fColor = vec4(N, 1.0);
	fColor.a = 1.0;
}