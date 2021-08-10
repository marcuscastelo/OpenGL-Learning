#version 330 core

layout(location=0)out vec4 color;

in vec4 v_Pos;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(){
    color = texture(u_Texture, v_TexCoord);
    color = mix(color, u_Color, pow(v_Pos.x*v_Pos.x + v_Pos.y*v_Pos.y, 0.9));
}