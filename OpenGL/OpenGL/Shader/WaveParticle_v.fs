
#version 450 core
uniform sampler2D screen_RT;
uniform sampler2D particle_map;
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

vec4 particle=texture(particle_map,uv);
float lengtnum=radius;
float r=lengtnum/float(N);
float yoffset=-0.5*r;
float di=0.0;
vec2 real_uv=uv;
vec2 dxy=vec2(0);


dxy.x=particle.x;
di=particle.y;



for(float y=1.0/lengtnum;y<1;y+=1.0/lengtnum)
{

    // if(real_uv.y+yoffset>1.0||real_uv.y+yoffset<0.0)
    //  continue;
    yoffset=r*y;
    vec2 uv1=real_uv+vec2(0,yoffset);
    vec2 uv2=real_uv-vec2(0,yoffset);
    
    
    vec4 velAmpB=texture(particle_map,uv1);
    vec4 velAmpT=texture(particle_map,uv2);
    // if(uv1.y>1.0||uv1.y<0.0)
    // velAmpB=vec4(0.0);
    // if(uv2.y>1.0||uv2.y<0.0)
    // velAmpT=vec4(0.0);
    vec4 ampSum=velAmpB+velAmpT;
    vec4 ampDif=velAmpB-velAmpT;
    vec3 f = GetFilter(y);
    di+=ampSum.y*f.x;
    dxy.x+=ampSum.x*f.x*f.x;
    dxy.y+=ampDif.z*f.x*f.y*2.0;


}



vec2 uv1=uv+vec2(0,yoffset);
 vec4 particle2=texture(particle_map,uv);
//FragColor=vec4(cos(0.5*PI * abs(xoffset/r)+ 1.0),0,0,0);
FragColor=texture(screen_RT,uv);

FragColor=vec4(-Scale*dxy.x,di,-Scale*dxy.y,0);
//FragColor=texture(particle_map,uv);
//FragColor=texture(screen_RT,uv);  
//FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
//  float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//     FragColor = vec4(average, average, average, 1.0);
//FragColor=vec4(1.0,0.0,1.0,1.0);
}