#ifdef VERTEX_SHADER

layout (location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
uniform int isTan;
out vec4 color;
void main(){
	if (isTan == 1){
		color = vec4(0.0f,0.0f,1.0f,1.0f);
	} else {
		color = vec4(1.0f,1.0f,1.0f,1.0f);
	}	
}

#endif
