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
uniform int b_solid_color;
uniform vec4 solid_color;

void main()
{
	if(b_solid_color==0){
		fragColor = vec4(tc.xy,0,1);
	}
	else if (b_solid_color==1){
		fragColor = vec4(tc.xxx,1);
	}
	else{
		fragColor = vec4(tc.yyy,1);
	}
	//fragColor = b_solid_color ? solid_color : vec4(tc.xy,0,1);
}