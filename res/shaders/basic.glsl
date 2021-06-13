#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coordinates;

out vec2 v_texture_coordinates;

uniform mat4 u_mvp_matrix;

void main() {
   gl_Position = u_mvp_matrix * position;

   v_texture_coordinates = texture_coordinates;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texture_coordinates;

uniform sampler2D u_texture;

void main() {
   color = texture(u_texture, v_texture_coordinates);
};