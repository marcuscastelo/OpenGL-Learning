#version 330 core

layout(location=0)out vec4 color;

in vec4 pos;

void main(){
    color=vec4(pos.x + 0.5,pos.y + 0.5,pos.z + 0.5,1.0);
}