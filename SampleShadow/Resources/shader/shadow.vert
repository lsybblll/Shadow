attribute vec3 a_position;

uniform vec3 u_LightLocation;

void main()
{
	vec3 A=vec3(0.0,0.1,0.0);
	vec3 n=vec3(0.0,1.0,0.0);
	vec3 S=u_LightLocation;
	vec3 V=(CC_MVMatrix*vec4(a_position,1)).xyz;
	vec3 VL=S+(V-S)*(dot(n,(A-S))/dot(n,(V-S)));
    gl_Position  = CC_PMatrix * vec4(VL,1);
}
