#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 icolor;
out vec3 color;
void main()
{
   gl_Position = vec4(aPos, 1.0);
   color = icolor;
}

#shader fragment
#version 330 core
out vec4 FragColor;
in vec3 color;
void main()
{
   FragColor = vec4(color, 1.0f);
}