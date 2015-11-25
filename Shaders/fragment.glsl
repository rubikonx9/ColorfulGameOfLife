#version 330

uniform samplerBuffer sampler;
uniform int screenSizeX;

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
	fragmentColor = getBlurred(2);
}