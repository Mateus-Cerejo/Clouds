#version 330 core

in vec3 fragPos;

out vec4 FragColor;

uniform vec3 cameraPos;

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

    float finalDist = length(camToFrag * multiplier);

    return finalDist;
}

void main()
{
    float finalDist = getFragDensity();
    FragColor = vec4(vec3(0.9, 0.9, 0.9), 1-exp(-finalDist * 3));
} 