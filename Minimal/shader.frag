#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 TexCoord;

uniform sampler2D tex;
uniform vec3 lightPos = vec3(0,20,0); 
uniform vec3 lightColor = vec3(1);
uniform vec3 objectColor = vec3(1);
uniform float hasTexture = 0;

out vec4 FragColor;

void main()
{		
	if(hasTexture == 0){
		float ambientStrength = 0.4;
		vec3 ambient = ambientStrength * lightColor;
  	
		// diffuse 
		vec3 norm = normalize(fragNormal);
		vec3 lightDir = normalize(lightPos - fragPosition);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;
            
		vec3 result = (ambient + diffuse) * objectColor;
		FragColor = vec4(result, 1.0);
	}
	else{
		FragColor = texture(tex, TexCoord); 
	}

	

}
