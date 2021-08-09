#version 330 core

layout(location=0)out vec4 color;

in vec4 pos;

uniform vec4 u_Color;

void main(){
    color = u_Color * (1 - pow( (pos.x * pos.x + pos.y * pos.y) / 3, 0.2));
}