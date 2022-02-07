#version 330 core

uniform vec3 color;

out vec4 fragment_color;

void main()
{
  fragment_color = vec4(color, 0.9f);
}