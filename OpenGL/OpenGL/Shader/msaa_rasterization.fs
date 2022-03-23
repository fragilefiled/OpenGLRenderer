#version 450 core
in vec2 uv;
uniform sampler2DMS screen_RT;
//uniform sampler2D screen_RT;
out vec4 FragColor;
in float depth;
void main(){
//FragColor=vec4(ourColor,1.0f);
vec4 colorSample1 = texelFetch(screen_RT, ivec2(uv*128.0), 0);
vec4 colorSample2 = texelFetch(screen_RT, ivec2(uv*128.0), 1);
vec4 colorSample3 = texelFetch(screen_RT, ivec2(uv*128.0), 2);
vec4 colorSample4 = texelFetch(screen_RT, ivec2(uv*128.0), 3);
FragColor=(colorSample1+colorSample2+colorSample3+colorSample4)/4.0;
// if(colorSample1.xyzw==0&&colorSample2.xyzw==0&&colorSample3.xyzw==0&&colorSample4.xyzw==0)
// FragColor=vec4(0);
//FragColor=colorSample1;
//FragColor=texture(screen_RT,uv);
//FragColor=vec4(1);
//FragColor=pow(texture(screen_RT, uv),vec4(1.0/2.2));
//FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
//  float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//     FragColor = vec4(average, average, average, 1.0);
//FragColor=vec4(1.0,0.0,1.0,1.0);
}