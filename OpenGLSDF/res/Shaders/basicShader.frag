#version 450

//BB is this not sloooow?
varying vec2 texCoord0;

//SDF Varibles (Shadertoy)
uniform sampler2D iChannel0;
uniform float iTime;
uniform vec2 iResolution;

vec2 rotate(vec2 pos, float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	
	return mat2(c, s, -s, c) * pos;
}

float plane(vec3 pos)
{
	return pos.y;
}

float sphere(vec3 pos, float radius)
{
	return length(pos) - radius;
}

float box(vec3 pos, vec3 size)
{
	return length(max(abs(pos) - size, 0.0));
}

float roundedBox(vec3 pos, vec3 size, float radius)
{
	return length(max(abs(pos) - size, 0.0)) - radius;
}

float map(vec3 pos)
{
	float planeDistance = plane(pos);
	
	pos.xy = rotate(pos.xy, pos.z * sin(iTime) * 0.01);
	
	pos.x = abs(pos.x);
	
	pos = mod(pos + 10.0, 20.0) - 10.0;
	
	pos.xy = rotate(pos.xy, iTime);
	pos.xz = rotate(pos.xz, iTime * 0.7);
	
	//if(iMouse.z > 0.0)
     	//	return min(planeDistance, roundedBox(pos, vec3(2.0), 1.0));
	//else
		return min(planeDistance, sphere(pos, 3.0));
}

vec3 albedo(vec3 pos)
{
	return vec3(smoothstep(0.4, 0.41, fract(pos.x + sin(pos.z * 0.4 + iTime))));
}
// noun / æl'bi?dou / 
// (physics) the proportion of the total light striking the surface of an object, 
// such as a planet, which is reflected from that surface
vec3 material(vec3 pos)
{
	return vec3(smoothstep(0.4, 0.41, fract(pos.x + sin(pos.z * 0.4 + iTime))));
	
}

vec3 computeNormal(vec3 pos)
{
	vec2 eps = vec2(0.01, 0.0);
	return normalize(vec3(
	map(pos + eps.xyy) - map(pos - eps.xyy), 
	map(pos + eps.yxy) - map(pos - eps.yxy), 
	map(pos + eps.yyx) - map(pos - eps.yyx)));
	
}

float diffuse(vec3 normal, vec3 lightDirection)
{
	// return max(dot(normal, lightDirection), 0.0);
	// wrap lighting
	return dot(normal, lightDirection) * 0.5 + 0.5;
}

float specular(vec3 normal, vec3 dir)
{
	// IBL
	vec3 h = normalize(normal - dir);
	return pow(max(dot(h, normal), 0.0), 100.0);
}


void main()
{
	vec2 fragCoord = gl_FragCoord.xy;
	vec4 fragColor = texture2D(iChannel0, texCoord0); 

	//Insert Shader Here

vec2 vUV = (fragCoord.xy / iResolution.xy);
	vec2 vViewCoord = vUV * 2.0 - 1.0;
	
	float fRatio = iResolution.x / iResolution.y;
	vViewCoord.y /= fRatio;
	
	vec2 uv = vViewCoord;
	
	vec3 pos = vec3(sin(iTime * 0.2) * 4.0, 5.0 + sin(iTime * 0.4) * 3.0, -13.0);
	vec3 dir = normalize(vec3(uv, 1.0));
	
	// Ray March 
	for(int i = 0;i < 64;i++)
	{
		float d = map(pos);
		pos += d * dir;
	}
	
	vec3 normal = computeNormal(pos);
	
	
	vec3 lightPos = vec3(0.0, 100.0, -100.0);
	vec3 dirToLight = normalize(lightPos - pos);
	vec3 posToLight = pos + (0.001 * dirToLight);
	
	float fShadowBias = 0.05;
	float fStartDistance = fShadowBias / abs(dot(dirToLight, normal));
	float fLightDistance = 100.0;
	float fLength = fLightDistance - fStartDistance;
	
	float posToLightDistance = 0.0;
	for(int i = 0;i < 64;i++)
	{
		float d = map(posToLight);
		posToLightDistance += d;
		posToLight += d * dirToLight;
	}
	
	float fShadow = step(0.0, posToLightDistance) * step(fLightDistance, posToLightDistance);		
	
	float fAmbientOcclusion = 1.0;
	
	float fDist = 0.0;
	for(int i = 0;i <= 5;i++)
	{
		fDist += 0.1;
		
		float d = map(pos + normal * fDist);
		
		fAmbientOcclusion *= 1.0 - max(0.0,(fDist - d) * 0.2 / fDist);                                 
	}
	
	// get colour from reflected ray
	float fSeparation = 0.1;
	fLength = 160.0;
	
	vec3 dirReflected = reflect(dir, normal);
	fStartDistance = fSeparation / abs(dot(dirReflected, normal));
	
	vec3 posReflected = pos + (0.001 * dirReflected);
	
	float posReflectedDistance = 0.0;
	for(int i = 0;i < 64;i++)
	{
		float d = map(posReflected);
		posReflectedDistance += d;
		posReflected += d * dirReflected;
	}
	
	float fReflected = step(0.0, posReflectedDistance) * step(fLength, posReflectedDistance);		
	
	float diffReflected = diffuse(normal, dirReflected);
	float specReflected = specular(normal, dir);
	
	vec3 colorReflected = (diffReflected + specReflected) * vec3(0.0, 0.2, 0.81) * (1.0 - fReflected) * material(posReflected);
	
	float diff = diffuse(normal, dirToLight);
	float spec = specular(normal, dir);
	vec3 color = (diff + spec) * vec3(0.0, 0.2, 0.81) *  material(pos);
	
	float fogFactor = exp(-pos.z * 0.01);
	vec3 fogColor = vec3(0.0, 0.2, 0.81);
	
	color = mix(clamp(color + colorReflected, 0.0,1.0), clamp(color + colorReflected, 0.0,1.0) * 0.25, 1.0 - fShadow);
	color = mix(fogColor, fAmbientOcclusion * color, fogFactor);
	
	//color = abs(dirReflected);
     	//color = abs(dirToLight);
	//color = abs(normal);
	//color = vec3(fAmbientOcclusion);
	//color = colorReflected;
	fragColor = vec4(color, 1.0);

	gl_FragColor = fragColor;
	//gl_FragColor = vec4(iTime, 0.0, 0.0, 0.0);
}






