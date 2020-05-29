#shader vertex
layout(Location = 0) in vec3 aPos;
void main() 
{
	gl_Position = vec4(aPos, 1.0f);
}


#shader fragment
out vec4 FragColor;
void main() 
{
	FragColor = vec4(0.8f, 0.5f, 0.7f, 1.0f);
}