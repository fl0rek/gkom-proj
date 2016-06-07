#version 330 core

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 color;

uniform sampler2D texture0;

uniform vec3 LightPosition_worldspace;
uniform float lightIntensity;

void main(){
	vec3 LightColor = vec3(1,1,1);
	
	vec3 MaterialDiffuseColor = texture(texture0, UV).rgb;
	vec3 MaterialAmbientColor = vec3(0.2,0.2,0.2) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	float distance = length( LightPosition_worldspace - Position_worldspace );

	vec3 n = normalize( Normal_cameraspace );
	vec3 l = normalize( LightDirection_cameraspace );

	// perpendicular -> 1
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	vec3 E = normalize(EyeDirection_cameraspace);
	vec3 R = reflect(-l,n);

	// loking into relection?
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	color = 
		// ambient
		MaterialAmbientColor +
		// diffuse
		MaterialDiffuseColor * LightColor * lightIntensity * cosTheta / (distance*distance) +
		// reflection
		MaterialSpecularColor * LightColor * lightIntensity * pow(cosAlpha,5) / (distance*distance);

}
