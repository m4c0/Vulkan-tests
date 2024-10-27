#version 450

layout(location = 0) out vec4 frag_colour;
layout(location = 1) out vec4 frag_id;

void main() {
  frag_colour = vec4(1, 1, 0, 1);  
  frag_id = vec4(0.5, 0.5, 0.5, 1.0);
}
