#version 450 core
in vec2 uv;
uniform sampler2D screen_RT;
out vec4 FragColor;
in float depth;
uniform float threshold;
float GetLumin(vec4 color){

return  0.2125 * color.r + 0.7154 * color.g + 0.0721 * color.b;
}
void main(){
//FragColor=vec4(ourColor,1.0f);
vec4 col=texture(screen_RT, uv);
float lumin=GetLumin(col);
float coef=clamp(lumin-threshold,0.0,1.0);
FragColor=coef*col;

//FragColor=pow(texture(screen_RT, uv),vec4(1.0/2.2));
//FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
//  float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//     FragColor = vec4(average, average, average, 1.0);
//FragColor=vec4(1.0,0.0,1.0,1.0);
}