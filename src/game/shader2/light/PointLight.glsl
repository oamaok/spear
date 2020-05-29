#pragma once

#include "light/ShadowGrid.glsl"
#include "light/Brdf.glsl"

vec3 evaluatePointLight(vec3 P, vec3 N, vec3 V, vec3 cdiff, vec3 f0, float alpha2, int base)
{
	vec4 data0 = pointLightData[base + 0];
	vec4 data1 = pointLightData[base + 1];
	vec4 data2 = pointLightData[base + 2];
	vec4 data3 = pointLightData[base + 3];

	vec3 lightOrigin = data0.xyz;
	float lightRadius = data0.w;

	vec3 delta = lightOrigin - P;
	float distSq = dot(delta, delta);
	vec3 L = delta / sqrt(distSq);

	float radiusSq = lightRadius*lightRadius;
	if (distSq < radiusSq) {

		vec3 lightColor = data1.xyz;
		vec3 shadowMul = data2.xyz;
		vec3 shadowBias = data3.xyz;
		vec3 shadowTexCoord = delta * shadowMul + shadowBias;
		float shadow = evaluateShadowGrid(shadowTexCoord);
		float attenuation = 1.0 / (0.1 + distSq) - 1.0 / (0.1 + radiusSq);

		vec3 H = normalize(L + V);
		float VdotH = saturate(dot(V, H));
		float NdotL = saturate(dot(N, L));
		float NdotV = saturate(dot(N, V));
		float NdotH = saturate(dot(N, H));

		return asVec3(shadow * attenuation * NdotL) * lightColor * BRDF_specularGGX(cdiff, f0, alpha2, VdotH, NdotL, NdotV, NdotH);
	} else {
		return asVec3(0.0);
	}
}