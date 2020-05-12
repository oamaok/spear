@module ShadowGrid

@ctype vec3 sf::Vec3
@ctype vec4 sf::Vec4

@vs vs

@glsl_options flip_vert_y

in vec2 pos;

out vec2 v_uv;

void main()
{
	v_uv = pos;
    gl_Position = vec4(pos.x * 2.0 - 1.0, pos.y * 2.0 - 1.0, 0.5, 1.0);
}

@end

@fs fs

in vec2 v_uv;

out float o_color;

uniform samplerCube shadowDepth;

uniform Pixel
{
	float depthSlices;
	vec3 volumeOrigin;
	vec3 volumeUnits;
};

void main()
{
	float x = v_uv.x * depthSlices;
	float slice = floor(x);
	vec3 relPos = vec3(x - slice, slice, v_uv.y);
	vec3 pos = volumeOrigin + relPos * volumeUnits;

	float dist = length(pos) - 0.5;
	float sum = 0.0;

	float radius = 0.5;

	// Generated by misc/shadow_sample_halton.py
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.500000,-0.500000,-0.500000)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.300000,-0.357143,-0.409091)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.100000,-0.214286,-0.318182)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.100000,-0.071429,-0.227273)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.300000,0.071429,-0.136364)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.460000,0.214286,-0.045455)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.260000,0.357143,0.045455)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.060000,-0.479592,0.136364)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.140000,-0.336735,0.227273)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.340000,-0.193878,0.318182)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.420000,-0.051020,0.409091)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.220000,0.091837,-0.491736)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.020000,0.234694,-0.400826)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.180000,0.377551,-0.309917)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.380000,-0.459184,-0.219008)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.380000,-0.316327,-0.128099)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.180000,-0.173469,-0.037190)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.020000,-0.030612,0.053719)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.220000,0.112245,0.144628)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.420000,0.255102,0.235537)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.340000,0.397959,0.326446)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.140000,-0.438776,0.417355)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.060000,-0.295918,-0.483471)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.260000,-0.153061,-0.392562)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.460000,-0.010204,-0.301653)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.492000,0.132653,-0.210744)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.292000,0.275510,-0.119835)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.092000,0.418367,-0.028926)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.108000,-0.418367,0.061983)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(0.308000,-0.275510,0.152893)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.452000,-0.132653,0.243802)).x ? 1.0 : 0.0;
	sum += dist < texture(shadowDepth, pos + radius*vec3(-0.252000,0.010204,0.334711)).x ? 1.0 : 0.0;
	sum *= 1.0 / 32.0;

	o_color = sum;
}

@end

@program ShadowGrid vs fs
