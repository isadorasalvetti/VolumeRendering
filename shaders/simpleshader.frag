#version 330 core

uniform sampler3D VOXELS;
uniform vec2 SIZE;
uniform vec3 worldSpaceRayDirection;
uniform vec3 candidateFaces;
uniform mat4x4 invViewMatrix;
uniform mat4x4 viewMatrix;
//uniform float brigtness;

out vec4 color;

vec3 cubeNormals[6] = vec3[6](
    vec3(0, 0, -1), //1. front
    vec3(-1, 0, 0), //2. right
    vec3(0, -1, 0), //3. top
    vec3(0, 0, 1), //4. back
    vec3(1, 0, 0), //5. left
    vec3(0, 1, 0) //6. bottom
);

vec3 cubePoints[6] = vec3[6](
    vec3(1, 1, 1), //1. front
    vec3(1, 1, 1), //2. right
    vec3(1, 1, 1), //3. top
    vec3(-1, -1, -1), //4. back
    vec3(-1, -1, -1), //5. left
    vec3(-1, -1, -1) //6. bottom
);

vec3 texSize = textureSize(VOXELS, 0);

bool isValidText(const vec3 pos){
    return all(lessThan(pos, vec3(1+1e-6))) && all(greaterThan(pos, vec3(-1e-6)));
}

bool isValidWorld(const vec3 pos){
    return all(lessThan(pos, vec3(1+1e-6))) && all(greaterThan(pos, vec3(-1-1e-6)));
}

vec3 intersectPlane(const vec3 n, const vec3 p0, const vec3 l0, const vec3 l){
    // assuming vectors are all normalized
    float denom = dot(l, n);
    if (denom >= 1e-10) {
        vec3 p0l0 = p0 - l0;
        float t = dot(p0l0, n) / denom;
        return l0+t*l;
    }
    else return vec3(-100000);
}

vec3 viewToTextSpace(vec3 point){
    return (point+1)/2;
}

vec3 textSpaceToView(vec2 texCoord, float a){
    return (vec3(texCoord, a)*2)-1; //??
}

vec3 worldToView(vec3 point){
    return (viewMatrix*vec4(point, 1)).xyz;
}

void main(){
    vec3 testColor = vec3(0);

    //Get texture size and pixel coords
    float maxRays = 1000;

    //Brigtness
    vec3 brigtness = vec3(0.5f);

    //Find intersections
    vec3 intersec[2] = vec3[2](vec3(0), vec3(0)); //Intersections ordered first[0], last[1].
    vec3 worldSpaceRayOrigin = (invViewMatrix*vec4(textSpaceToView(gl_FragCoord.xy/SIZE, 0),0)).xyz;

    for (int i = 0; i < 6; i ++){
        for (int j = 0; j < 2; j++){
            vec3 cn;

            if (j == 0) cn = cubeNormals[i];
            else cn = -cubeNormals[i];

            vec3 intersec_i = intersectPlane(cn, cubePoints[i], worldSpaceRayOrigin, worldSpaceRayDirection);

            if (isValidWorld(intersec_i)){
                if (intersec[0] == vec3(0)) intersec[0] = intersec_i;
                else if(length(worldSpaceRayOrigin - intersec[0]) < length(worldSpaceRayOrigin - intersec_i)) intersec[1] = intersec_i;
                else {
                    intersec[1] = intersec[0];
                    intersec[0] = intersec_i;
                }
                break;
            }
        }
    }

    //Choose first intersection
    vec3 samplecoords;
    vec3 fragRayStep = normalize(intersec[1] - intersec[0])/texSize;
    if (!(intersec[0] == vec3(0))){
        intersec[0] = viewToTextSpace(worldToView(intersec[0]));
        intersec[1] = viewToTextSpace(worldToView(intersec[1]));
        samplecoords = intersec[0];
        testColor = samplecoords;
    }

    //Initial color and ray direction
    vec3 colorS = vec3(0);

    //Cast ray
    for (int i=0; i < maxRays; i++){
        // Break when ray is out of volume.
        if (!isValidText(samplecoords)) break;
        //Add color
        colorS += texture(VOXELS, samplecoords).rrr/(brigtness*texSize);
        //Add next voxel
        samplecoords += fragRayStep;
        colorS = vec3(i/100.0f);
    }
    color = vec4(colorS, 1);
}
