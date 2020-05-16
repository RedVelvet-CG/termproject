#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

// input from vertex shader

in vec2 tc;
in vec3 norm;

// the only output variable
out vec4 fragColor;

// shader's global variables, called the uniform variables
uniform sampler2D	TEX0;
uniform sampler2D	TEX1;
uniform sampler2D   TEX2;
uniform int			mode;
uniform vec4		tank_color;

void main()
{
	if (mode == 0)		fragColor = texture(TEX0, tc);
	else if (mode == 1)	fragColor = texture(TEX1, tc);
	else if (mode == 2) fragColor = texture(TEX2, tc);
	else if (mode == 3) fragColor = tank_color;
	else				fragColor = vec4(tc.xy,0,1);

	//fragColor = b_solid_color ? solid_color : vec4(tc.xy,0,1);
}