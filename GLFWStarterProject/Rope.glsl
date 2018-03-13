////////////////////////////////////////
// Rope.glsl
////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

#ifdef VERTEX_SHADER

layout(location=0) in vec3 Position;

out vec3 fragPosition;

uniform mat4 ModelMtx=mat4(1);
uniform mat4 ModelViewProjMtx=mat4(1);

////////////////////////////////////////
// Vertex shader
////////////////////////////////////////

void main() {
	gl_Position=ModelViewProjMtx * vec4(Position,1);
	fragPosition=vec3(ModelMtx * vec4(Position,1));
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FRAGMENT_SHADER

in vec3 fragPosition;

uniform vec4 AmbientColor=vec4(0.1);
uniform vec3 LightDirection1=normalize(vec3(-1,-1,-1));
uniform vec3 LightColor1=vec3(1,1,1);
uniform vec4 DiffuseColor=vec4(1,1,1,1);
uniform sampler2D tex;
uniform int hasTex;

out vec4 finalColor;

////////////////////////////////////////
// Fragment shader
////////////////////////////////////////

void main() {
	

	// Diffuse reflectance
	//vec4 reflectance=irradiance * DiffuseColor;

	// Gamma correction
	//finalColor=sqrt(reflectance);
	finalColor = vec4(1.0f,1.0f,1.0f, 0.0f);
}

#endif

////////////////////////////////////////////////////////////////////////////////
