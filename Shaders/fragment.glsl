#version 330

uniform samplerBuffer sampler;
uniform int screenSizeX;
uniform int screenSizeY;

in vec3 vPosition;
in vec2 vTexCoord;
out vec4 fragmentColor;

int translateToLinearPosition(int x, int y) {
	return (int(y * (screenSizeX - 1) + x));
}

vec4 getBlurred(int n) {
	int x = int(gl_FragCoord.x);
	int y = int(gl_FragCoord.y);
	vec4 texel = vec4(0.0, 0.0, 0.0, 0.0);
	for(int i = -n; i <= n; ++i) {
		for(int j = -n; j <= n; ++j) {
			if(abs(i)!=abs(j)) texel = texel + texelFetch(sampler, translateToLinearPosition(x+i, y+j));
		}
	}
	texel = texel / ((n+1) * (n+1));
	return texel;
}

void main() {
	vec3 color = vec3(0.0, 0.0, 0.0);
	
	int position = translateToLinearPosition(int(gl_FragCoord.x), int(gl_FragCoord.y));
	vec4 texel = texelFetch(sampler, position);
//	if(texel.r != 1.0 && texel.g != 1.0 && texel.b != 1.0) {
		texel = getBlurred(2);
//	}

	fragmentColor = texel;
}