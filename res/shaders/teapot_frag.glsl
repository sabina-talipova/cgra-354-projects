#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;     // Base color of the object
uniform vec3 uLightPos;  // Light position in world space

in VertexData {
    vec3 position;       // Vertex position
    vec3 normal;         // Vertex normal
    vec2 textureCoord;   // Texture coordinates
} f_in;

// framebuffer output
out vec4 fb_color;

void main() {
    vec3 fragPos = vec3(uModelViewMatrix * vec4(f_in.position, 1.0));
    vec3 lightDir = normalize(uLightPos - fragPos);
    float diff = max(dot(f_in.normal, lightDir), 0.0);
    vec3 diffuse = diff * uColor;

    vec3 finalColor = diffuse;
    fb_color = vec4(finalColor, 1.0);
}
