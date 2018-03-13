////////////////////////////////////////
// Model.glsl
////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

#ifdef VERTEX_SHADER

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoord;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 texCoord;

uniform mat4 ModelMtx=mat4(1);
uniform mat4 ModelViewProjMtx=mat4(1);

////////////////////////////////////////
// Vertex shader
////////////////////////////////////////

void main() {
	gl_Position=ModelViewProjMtx * vec4(Position,1);
	texCoord = TexCoord;
	fragPosition=vec3(ModelMtx * vec4(Position,1));
	fragNormal=vec3(ModelMtx * vec4(Normal,0));
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FRAGMENT_SHADER

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 texCoord;

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
	// Compute irradiance (sum of ambient & direct lighting)
	vec4 irradiance;
	if (hasTex == 1){
		irradiance= texture(tex, texCoord);
	} else {
		irradiance = AmbientColor + vec4(LightColor1 * max(0, dot(LightDirection1, fragNormal)), 1.0f);
	}
	

	// Diffuse reflectance
	vec4 reflectance=irradiance * DiffuseColor;

	// Gamma correction
	finalColor=sqrt(reflectance);
	//finalColor = vec4(1.0f,0.0f,0.0f, 0.0f);
}

#endif

////////////////////////////////////////////////////////////////////////////////
