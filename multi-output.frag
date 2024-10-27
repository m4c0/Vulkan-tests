#version 450

layout(location = 0) in vec2 frag_pos;

layout(location = 0) out vec4 frag_colour;
layout(location = 1) out vec4 frag_id;

const float pi = 3.1415926535;

float sd_circle(vec2 p, float r) {
  return length(p) - r;
}

void main() {
  float d = sd_circle(frag_pos, 0.2);
  d = 100.0 * abs(d);

  float x = floor(d / (pi * 2.0)) / 256.0;

  d = cos(d) * 0.5 + 0.5;
  frag_colour = vec4(d, x, 0, 1);

  d = step(0.5, d);
  frag_id = vec4(d, 0.5, 0.5, 0.5);
}
