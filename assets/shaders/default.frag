#version 330 core

in vec3 fragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform sampler2D noiseTexture;

// Measures the distance that the vector from the camera to the fragPos would travel inside the mesh if extended indefinitely
float getFragDensity()
{
    vec3 camToFrag = normalize(fragPos - cameraPos);

    float xDist = 0;
    float yDist = 0;
    float zDist = 0;

    if (camToFrag.x > 0){
        float possibleX = 0.5; // For now this are only going to be cubes so hardcode 0.5 on all possibles
        xDist = abs(possibleX - fragPos.x);
    } else {
        float possibleX = -0.5;
        xDist = abs(possibleX - fragPos.x);
    }

    if (camToFrag.y > 0){
        float possibleY = 0.5;
        yDist = abs(possibleY - fragPos.y);
    } else {
        float possibleY = -0.5;
        yDist = abs(possibleY - fragPos.y);
    }

    if (camToFrag.z > 0){
        float possibleZ = 0.5;  
        zDist = abs(possibleZ - fragPos.z);
    } else{
        float possibleZ = -0.5;
        zDist = abs(possibleZ - fragPos.z);
    }

    float xSteps = abs(xDist / camToFrag.x);
    float ySteps = abs(yDist / camToFrag.y);
    float zSteps = abs(zDist / camToFrag.z);

    float multiplier = min(min(xSteps, ySteps), zSteps);

    float finalDist = length(camToFrag * multiplier) + ((1-texture(noiseTexture, TexCoord).x) * 0.3);
//
//    float temp = finalDist;
//    for(float i = 0; i < temp; i += 0.01){
//        finalDist += (1-texture(noiseTexture, TexCoord).x) * 0.01;
//    }
//

    if(finalDist < 0.4){
        finalDist = 1-exp(finalDist);
    } else if(finalDist > 1){
        finalDist = 1;
    }

    return finalDist;
}

void main()
{
    float density = getFragDensity();
    FragColor = vec4(vec3(0.9, 0.9, 0.9), 1-exp(-density * 1));
} 