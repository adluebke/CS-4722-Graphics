#version 150

in vec3 fN;		// NEW! From the vertex shader (but interpolated)
in vec3 fE;
in vec3 fL;

out vec4 fColor;
uniform vec4 ambient, diffuse, specular;	// NEW!!
float shininess;
float Kd;

void main () {
	vec3 N = normalize (fN);	// the Normal
	vec3 L = normalize (fL);	// the Light direction
	vec3 E = normalize (-fE);	// the vertex position
	vec3 H = normalize (L + E); // the Half vector

	shininess = 100;
	// Calculate the diffuse part - but don't go negative!
	if(dot(N, L) > 1.0)
	{
		Kd = 1.0f;
	}
	else if (dot(N, L) > 0.9)
	{
		Kd = 0.9f;
	}
	else if (dot(N, L) > 0.8)
	{
		Kd = 0.8f;
	}
	else if (dot(N, L) > 0.7)
	{
		Kd = 0.7f;
	}
	else if (dot(N, L) > 0.6)
	{
		Kd = 0.6f;
	}
	else if (dot(N, L) > 0.5)
	{
		Kd = 0.5f;
	}
	else if (dot(N, L) > 0.4)
	{
		Kd = 0.4f;
	}
	else if (dot(N, L) > 0.3)
	{
		Kd = 0.3f;
	}
	else if (dot(N, L) > 0.2)
	{
		Kd = 0.2f;
	}
	else if (dot(N, L) > 0.1)
	{
		Kd = 0.1f;
	}
	else
	{	
		Kd = 0.0f;
	}
	vec4 dif = Kd*diffuse;				// ... and then color

	// Calculate the specular part. 
	float Ks = pow (max(dot(N, H), 0.0), shininess);	// Intensity
	vec4 spec = Ks*specular;							// ... and then color
	
	/*if (dot(N, L) < 0.0) {								// For testing
		spec = vec4(0.0, 1.0, 0.0, 1.0);				
	}*/
	
	// The final color is a combination of all of these
	fColor = ambient + dif + spec;
	fColor.a = 1.0;
}