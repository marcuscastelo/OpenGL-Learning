#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoord;

out vec4 v_Pos;
out vec2 v_TexCoord;

void main() {
    gl_Position = position;
    v_Pos = position;
    v_TexCoord = texCoord;
}