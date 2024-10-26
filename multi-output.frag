#version 450

layout(location = 0) out vec4 frag_colour;
layout(location = 1) out vec4 frag_test;

void main() {
  frag_colour = vec4(1, 1, 0, 1);  
  frag_test = vec4(1, 0, 1, 1);  
}
