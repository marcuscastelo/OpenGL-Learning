#version 330 core

layout(location=0)out vec4 color;

in vec4 pos;

void main(){
    color=vec4(
        pos[1]+.5,
        pos[0]+.5,
        -pos[0]+.5,
        1
    );
}