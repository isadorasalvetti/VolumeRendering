#version 330 core

uniform sampler3D VOXELS;
uniform vec2 SIZE;
uniform mat4x4 invViewMatrix;
uniform mat4x4 viewMatrix;
uniform float brightness;
uniform float min, max;

out vec4 color;

vec3 cubeNormals[3] = vec3[3](
    vec3(-1, 0, 0), //2. right
    vec3(0, -1, 0), //3. top
    vec3(0, 0, -1) //1. front
);

vec3 texSize = textureSize(VOXELS, 0);

const float eps = 1e-6;

bool isValidText(const vec3 pos){
    return all(lessThan(pos, vec3(1+eps))) && all(greaterThan(pos, vec3(-eps)));
}

bool isValidWorld(const vec3 pos){
    return all(lessThan(pos, vec3(1+eps))) && all(greaterThan(pos, vec3(-1-eps)));
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

vec3 transfer(float reading){
    if (reading < min || reading > max) return vec3(0.0f);
    else return vec3(reading);
}

void main(){
    //Find intersections
    vec3 intersec[2] = vec3[2](vec3(0), vec3(0)); //Intersections ordered first[0], last[1].

    vec2 tex_norm = gl_FragCoord.xy / SIZE; // [0,1]x[0,1]
    vec2 tex_clip = tex_norm*2 - vec2(1); // [-1,1]x[-1,1]
    vec3 rayDisplacement = (invViewMatrix * vec4(tex_clip, 0, 0)).xyz; // orthogonal plane of ray space (parallel to screen plane)
    vec3 rayDirection = (invViewMatrix * vec4(0,0,-1,0)).xyz; // ray space

    vec3 d = rayDirection;
    for (int i = 0; i < 3; i ++){ // each pair of parallel face planes
        vec3 p = rayDisplacement;
        for (int j = -1; j <= 1; j += 2) { // -1,+1, one of the planes
            float lambda = (j - p[i])/d[i];
            vec3 q = p + lambda * d; // point in plane
            // check in the face
            if (isValidWorld(q)) {
                float z = (viewMatrix * vec4(q, 1.0f)).z;
                intersec[int(z > 0)] = q;
            }
        }
    }

    if (intersec[1] == vec3(0) && intersec[0] == vec3(0)) discard; // no intersection. Discard fragment.

    //Initial color and ray direction
    vec3 colorS = vec3(0);
    vec3 samplecoords = intersec[0];
    float stepSize = 0.01;
    vec3 fragRayStep = normalize(intersec[1]-intersec[0]) * stepSize;

    //Cast ray
    float maxRays = 1000;
    for (int i=0; i < maxRays; i++){
        //Add color
        vec3 textCoord = viewToTextSpace(samplecoords);
        if (!isValidText(textCoord)) break;
        if (transfer(texture(VOXELS, textCoord).r) != vec3(0, 0, 0)){
                colorS = vec3(1, 1, 1);
                break;
        }
        //Go to next voxel
        samplecoords += fragRayStep;
    }
    samplecoords -= fragRayStep;

    //Shadows
    stepSize = 0.03;
    vec3 normal = vec3(0,0,0);
    int iter = 2;
    for (int i = -iter; i <= iter; i++){
        for (int j = -iter; j <= iter; j++){
            for (int k = -iter; k <= iter; k++){
                vec3 surroundingSample = viewToTextSpace(samplecoords + normalize(vec3(i, j, k))*stepSize);
                if(isValidText(surroundingSample)){
                    float valDif = texture(VOXELS, surroundingSample).r - texture(VOXELS, samplecoords).r;
                    if(valDif > 0) normal += vec3(i, j, k) * valDif;
                    else normal -= vec3(i, j, k) * valDif;
                    normal -= vec3(i, j, k);
                }
            }
        }
    }
    if (normal != vec3(0, 0, 0)) normalize(normal);
    vec3 lDir = normalize(vec3(viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2]));
    float ndotl = dot(lDir, normal);
    if (ndotl < 0) ndotl = 0;

    color = vec4(mix(colorS*ndotl, colorS, 0.3), 1);

}
