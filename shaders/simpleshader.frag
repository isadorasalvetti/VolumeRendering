#version 330 core

uniform sampler3D VOXELS;
uniform vec2 SIZE;

out vec4 color;

void main(){
    vec2 coords = (gl_FragCoord.xy - 0.5) / SIZE;
    vec3 texcoords = vec3(coords, 0.5);
    color = vec4(vec3(1)-texture(VOXELS, texcoords).rrr, 1.0);
}
