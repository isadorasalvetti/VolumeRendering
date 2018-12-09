#version 330 core

uniform sampler3D VOXELS;
uniform vec2 SIZE;
uniform vec3 rayDirection;
//uniform float brigtness;

out vec4 color;

void main(){
    //Get texture size and pixel coords
    float maxRays = 0.8;
    vec3 texSize = textureSize(VOXELS, 0);
    vec2 coords = (gl_FragCoord.xy - 0.5) / SIZE;
    vec3 samplecoords = vec3(coords, 0.5);

    //Brigtness
    vec3 brigtness = vec3(0.5f);
    brigtness = brigtness * texSize;

    //Initial color and ray direction
    vec3 colorS = texture(VOXELS, samplecoords).rrr;

    //Cast ray
    for (int i; i < maxRays*texSize[2]; i++){
        // Break when ray is out of volume.
        if (samplecoords.x >= texSize.x
         || samplecoords.y >= texSize.y
         || samplecoords.z >= texSize.z
         || samplecoords.x < 0
         || samplecoords.y < 0
         || samplecoords.z < 0) break;
        //Add color
        samplecoords -= rayDirection/texSize;
        colorS += texture(VOXELS, samplecoords).rrr/brigtness;
        //colorS += mix(colorS, texture(VOXELS, samplecoords).rrr, 0.01);
    }

    color = vec4(colorS, 1);

}
