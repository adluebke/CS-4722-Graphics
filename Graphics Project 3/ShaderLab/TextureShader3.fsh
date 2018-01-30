#version 150

in vec3 fN;		
in vec3 fE;
in vec3 fL;

out vec4 fColor;

in vec2 texCoord;				// NEW!! This is the UV
uniform sampler2D texture;		// NEW!! This is the texture

uniform vec4 ambient, diffuse, specular;	
float shininess;
uniform float bumpValue;
vec4 color;

// Stolen from http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main () {

	vec3 N = normalize (fN);	// the Normal
	vec3 L = normalize (fL);	// the Light direction
	vec3 E = normalize (-fE);	// the vertex position
	vec3 H = normalize (L + E); // the Half vector

	// Take cross product of actual normal with forward vector to determine axis of rotation
	//vec3 crosser = cross (N, vec3(0, 0, 1));
	// Figure out how far the normal has been rotated
	//float rotter = acos(dot(vec3(0, 0, 1), N));
	
	//rotter*=bumpValue*100;
	
	// Now, rotate the texture normal by the amount between the forward vector and the actual normal
	//mat4 rotMat = rotationMatrix(crosser, rotter);
	//vec3 offset = (rotMat*texture2D(texture, texCoord)).xyz;

	//N = normalize (N+offset);
	

	shininess = 1;
	// Calculate the diffuse part - but don't go negative!
	float Kd = max (dot(N, L), 0.2);	// Intensity
	vec4 dif = Kd*diffuse;				// ... and then color

	float Ks = pow (max(dot(N, H), 0.0), shininess);	// Intensity
	vec4 spec = Ks*specular;							// ... and then color
	if (dot(N, L) < 0.0) {								// But only if the light is in front!
		spec = vec4(0.0, 0.0, 0.0, 1.0);
	}

		fColor = ambient + dif + spec;//+0.5*texture2D(texture, newTexCoord);
		fColor.a = 1.0;

}