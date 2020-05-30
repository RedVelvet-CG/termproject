#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

// input from vertex shader
in vec4 epos;
in vec2 tc;
in vec3 norm;

// the only output variable
out vec4 fragColor;

// shader's global variables, called the uniform variables
uniform sampler2D	TEX0;
uniform sampler2D	TEX1;
uniform sampler2D   TEX2;
uniform sampler2D	TEX3;
uniform sampler2D	TEX4;
uniform sampler2D	TEX5;
uniform int			game_mode;
uniform int			mode;
uniform vec4		color;

void main()
{
	if (game_mode == 1)
	{
		if (mode == 0)		fragColor = texture(TEX0, tc);
		else if (mode == 1)	fragColor = texture(TEX1, tc);
		else if (mode == 2) fragColor = texture(TEX2, tc);
		else if (mode == 3) fragColor = color;
		else				fragColor = vec4(tc.xy, 0, 1);
	}
	else if (game_mode == 0) {
		fragColor = texture(TEX3, tc);
	}
	else if (game_mode == 2) {
		fragColor = texture(TEX4, tc);
	}
	else if (game_mode == 3) {
		fragColor = texture(TEX5, tc);
	}

	//fragColor = b_solid_color ? solid_color : vec4(tc.xy,0,1);
}