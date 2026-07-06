#version 330 core

in vec3 fragPos;

out vec4 FragColor;

uniform vec4 baseColor;
uniform vec3 cameraPos;
uniform vec3 recSize;
uniform vec3 recPos;

void main()
{
    vec3 recPos = vec3(0.0, 0.0, -2.0);
    vec3 camPos = cameraPos;
    vec3 recSize = vec3(1.0, 1.0, 1.0);

    vec3 camToFrag = normalize(fragPos - camPos);

    float xDist = 0;
    float yDist = 0;
    float zDist = 0;

    if (camToFrag.x > 0){
        float possibleX = recPos.x + (recSize.x / 2);
        xDist = abs(possibleX - fragPos.x);
    } else {
        float possibleX = recPos.x - (recSize.x / 2);
        xDist = abs(possibleX - fragPos.x);
    }

    if (camToFrag.y > 0){
        float possibleY = recPos.y + (recSize.y / 2);
        yDist = abs(possibleY - fragPos.y);
    } else {
        float possibleY = recPos.y - (recSize.y / 2);
        yDist = abs(possibleY - fragPos.y);
    }

    if (camToFrag.z > 0){
        float possibleZ = recPos.z + (recSize.z / 2);
        zDist = abs(possibleZ - fragPos.z);
    } else{
        float possibleZ = recPos.z - (recSize.z / 2);
        zDist = abs(possibleZ - fragPos.z);
    }

    float xSteps = abs(xDist / camToFrag.x);
    float ySteps = abs(yDist / camToFrag.y);
    float zSteps = abs(zDist / camToFrag.z);

    float multiplier = min(min(xSteps, ySteps), zSteps);

    float finalDist = length(camToFrag * multiplier);
    
    FragColor = vec4(vec3(0.9, 0.9, 0.9), 1-exp(-finalDist * 3));
} 