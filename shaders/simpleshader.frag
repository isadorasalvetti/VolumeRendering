#version 330 core

uniform sampler3D VOXELS;
uniform vec2 SIZE;
uniform vec3 rayDirection;
uniform vec3 candidateFaces;
uniform mat4x4 invViewMatrix;
//uniform float brigtness;

out vec4 color;

vec3 cubeNormals[6] = {
    vec3(0, 0, -1), //1. front
    vec3(-1, 0, 0), //2. right
    vec3(0, -1, 0), //3. top
    vec3(0, 0, 1), //4. back
    vec3(1, 0, 0), //5. left
    vec3(0, 1, 0) //6. bottom
};

vec3 cubePoints[6] = {
    vec3(-1, -1, -1), //1. front
    vec3(1, 1, -1), //2. right
    vec3(1, 1, -1), //3. top
    vec3(-1, -1, 1), //4. back
    vec3(-1, 1, -1), //5. left
    vec3(1, -1, -1) //6. bottom
};

vec3 texSize = textureSize(VOXELS, 0);

bool isValid (const vec3 pos){
    return all(greaterThanEqual(pos,texSize)) && all(lessThan(pos, vec3(0)));
}

vec3 intersectPlane(const vec3 n, const vec3 p0, const vec3 l0, const vec3 l){
    // assuming vectors are all normalized
    float denom = dotProduct(n, l);
    if (denom > 1e-6) {
        Vec3f p0l0 = p0 - l0;
        vec3 t = dotProduct(p0l0, n) / denom;
        return t
    }
}

void main(){
    //Get texture size and pixel coords
    float maxRays = 1000;
    vec2 coords = (gl_FragCoord.xy - 0.5) / SIZE;
    vec3 samplecoords = vec3(coords, 0);

    //Brigtness
    vec3 brigtness = vec3(0.5f);
    brigtness = brigtness * texSize;

    //Initial color and ray direction
    vec3 colorS = texture(VOXELS, samplecoords).rrr;

    vec3 intersec[2] = {vec3(0), vec3(0)};
    //Find initial position
    for (int i = 0; i < 6; i ++){
        vec3 intersec_i = intersectPlane(cubeNormals[i], cubePoints[i], invViewMatrix*vec3(gl_FragCoord.xy, 0, 1), rayDirection);
        if (isValid(intersec))
            if (intersec[0] == vec3(0)) intersec[0] == intersec_i
            else if (intersec[1] == vec3(0)) intersec[1] == intersec_i
    }

    samplecoords = intersec;

    //Cast ray
    for (int i; i < maxRays; i++){
        // Break when ray is out of volume.
        if (!isValid(samplecoords)) break;
        //Add color
        samplecoords += rayDirection/texSize;
        colorS += texture(VOXELS, samplecoords).rrr/brigtness;
        //colorS += mix(colorS, texture(VOXELS, samplecoords).rrr, 0.01);
    }

    color = vec4(colorS, 1);

}
