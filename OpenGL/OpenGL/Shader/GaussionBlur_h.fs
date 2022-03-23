#version 450 core
in vec2 uv0;
in  vec2 uv1;
in  vec2 uv2;
in  vec2 uv3;
in  vec2 uv4;
uniform sampler2D screen_RT;
out vec4 FragColor;
in float depth;

void main(){
//FragColor=vec4(ourColor,1.0f);
   
 float a[5]={0.0545,0.2442,0.4026,0.2442,0.0545};
  //float a[5]={0.00,0.0,0.0,0.0,0.01};
    vec4 sum=a[2]*texture(screen_RT, uv2)+a[0]*texture(screen_RT, uv0).xyzw+a[1]*texture(screen_RT, uv1).xyzw+a[3]*texture(screen_RT, uv3).xyzw+a[4]*texture(screen_RT, uv4);

FragColor=sum;
//FragColor=vec4(1.0);
//FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
//  float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//     FragColor = vec4(average, average, average, 1.0);
//FragColor=vec4(1.0,0.0,1.0,1.0);
}