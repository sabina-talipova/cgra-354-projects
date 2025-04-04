#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;

uniform vec3 uSpecularColor;
uniform float uSpecularStrength;
uniform vec3 uAmbiantColor;
uniform float uShininess;

uniform vec3 lightPos;
uniform vec3 viewPos;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} f_in;

// framebuffer output
out vec4 fb_color;


void main() {
	// Normalize normal
	vec3 norm = normalize(f_in.normal);
	
	// Calculate light direction
	vec3 lightDir = normalize(lightPos - f_in.position);

	// Ambient component
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * uAmbiantColor;

	// Diffuse component
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * uColor;

	// Specular component
	vec3 viewDir = normalize(viewPos - f_in.position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
	vec3 specular = uSpecularStrength * spec * uSpecularColor;

	// output to the frambuffer
	fb_color = vec4(specular + ambient + diffuse, 1.0);
}
