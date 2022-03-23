
#version 450 core
uniform sampler2D screen_RT;
out vec4 FragColor;
uniform float N;
uniform float radius;
uniform float Scale;
in vec2 uv;
#define PI 3.14159274

vec3 GetFilter(float v)
{
    float s, c;
  
    s=sin(PI*v);
    c=cos(PI*v);
    return vec3(
        0.5f * (c + 1.0f), // 0.5 ( cos(v) + 1 )
        -0.5f * s, // -0.5 sin(v)
        -0.25f * (c * c - s * s + c) // cos(2v) + cos(v)
    );
}
void main(){
//FragColor=vec4(ourColor,1.0f);
vec4 particle=texture(screen_RT,uv);
float lengtnum=radius;
float r=lengtnum/float(N);
float xoffset=-0.5*r;
float di=0.0;
vec2 real_uv=(uv);
vec2 dxy=vec2(0);


dxy.y=particle.x;
di=particle.x;
for(float x=1.0/lengtnum;x<1;x+=1.0/lengtnum)
{
    xoffset=r*x;
    vec2 uv1=real_uv+vec2(xoffset,0);
    vec2 uv2=real_uv-vec2(xoffset,0);
   
    vec4 velAmpL=texture(screen_RT,uv1);
    vec4 velAmpR=texture(screen_RT,uv2);

    float ampSum=velAmpL.x+velAmpR.x;
    float ampDif=velAmpL.x-velAmpR.x;
    vec3 f = GetFilter(x);
    di+=ampSum*f.x;
    dxy.x+=ampDif*f.x*f.y*2;
    dxy.y+=ampSum*f.x*f.x;


}
  
//FragColor=vec4(cos(0.5*PI * abs(xoffset/r)+ 1.0),0,0,0);
FragColor=vec4(dxy.x,di,dxy.y,0);
//FragColor=particle;
//FragColor=texture(screen_RT,uv);
//FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
//  float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//     FragColor = vec4(average, average, average, 1.0);
//FragColor=vec4(1.0,0.0,1.0,1.0);
}