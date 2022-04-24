#version 450 core
in vec2 uv;
uniform sampler2D screen_RT;
out vec4 FragColor;
in float depth;
uniform float Time;
uniform sampler2D waveMap;
uniform vec4 Scale_control;
#define PI 3.1415926
uniform int powPower;
float powuse(float t,int x){
    int temp=x;
    float final=t;
    while(temp>0){
        final*=t;
        temp--;
    }
    return final;

}

void main(){
//FragColor=vec4(ourColor,1.0f);
vec2 uv_now=vec2(uv.x*3.0-Time*0.5,uv.y*3.0-Time*0.5);

// FragColor=pow(texture(waveMap, vec2(uv.x*1.9,0.5)),vec4(1.0/2.2));
// if(mod(floor(uv_now.x),2.0)==0)
// FragColor=texture(waveMap, fract(uv_now))*0.0+texture(screen_RT,uv)*0.7;
// else
// FragColor=texture(screen_RT,uv)*0.7;

// FragColor=texture(waveMap, -fract(uv_now))*cos((uv_now.y-0.5)*PI)*0.2+texture(screen_RT,uv)*1.0;;
// float k=2*PI/0.1f;
// float k_control=9.0;
// float A=0.1;
// float omega=2*PI/3.0;
// float x_gerstner=uv_now.x-k/0.5*A*sin(k*uv_now.x-omega*Time);
// float y_gerstner=A*cos(k_control*uv_now.x-omega*Time);
vec2 dir=normalize(vec2(1.0,0.0));
float wave_length=1.0;
float k=2*PI/wave_length;
float f=k*(dot(uv_now,dir)+0.04*Time);
float steepness=0.1;
float A=steepness/k;

float x_gerstner=dir.x*0.1*cos(f+0.5*PI);
float y_gerstner=0.1*sin(f+0.5*PI);
float z_gerstner=dir.y*0.1*cos(f+0.5*PI);
float t=fract( uv.x*2.0-0.4*Time)/wave_length;
int pownum=int(1.0*fract(uv.x));

x_gerstner=0.5*cos(2*PI*pow(t,1.0+1.0*fract(uv.x)));//powuse(t,pownum)
 //z_gerstner=0.5*cos(2*PI*powuse(t,powPower));
 y_gerstner=0.5*sin(2*PI*pow(t,1.0+1.0*fract(uv.x)));//pow 可以预计算的

// else{
//  x_gerstner=0.4*sin(2*PI*powuse(t,powPower+1));
//  y_gerstner=0.4*cos(2*PI*powuse(t,powPower+1));
// }


vec2 uv_use=fract(-uv_now);



vec4 B_spine=((texture(waveMap, -vec2(uv_use.x,uv.x*0.5+0.25))))*20*Scale_control.x*(1.0-uv.x)*cos((uv.y-0.5)*PI);//uv.x*0.5+0.25 uv出了点问题
FragColor=B_spine+(texture(screen_RT,uv))*Scale_control.y+vec4(x_gerstner,y_gerstner,z_gerstner,0.0)*Scale_control.z;
//+texture(screen_RT,0.5uv)挺有意思的想法


//FragColor=(texture(waveMap, -uv_use))*Scale_control.x*20;
//  dir=normalize(vec2(0.5,0.5));
//  wave_length=1.0;
//  k=2*PI/wave_length;
//  f=k*(dot(uv_now,dir)+0.5*Time);
//  steepness=0.1;
//  A=steepness/k;

//  x_gerstner=dir.x*0.1*cos(f+2.0);
//  y_gerstner=0.1*sin(f+2.0);
//  z_gerstner=dir.y*0.1*cos(f+2.0);
// FragColor+=vec4(x_gerstner,y_gerstner,z_gerstner,0.0)*Scale_control.z;

FragColor*=Scale_control.w;
//FragColor=(texture(waveMap, uv_use))*Scale_control.x*10;
//FragColor=(texture(waveMap, -fract(uv_now)))*cos((uv.y-0.5)*PI)*Scale_control.x;
//FragColor=vec4(x_gerstner,y_gerstner,0,0.0);;
//FragColor=texture(waveMap, -fract(uv_now))*cos((uv_now.y-0.5)*3.1415926)*0.2;;
//FragColor=texture(screen_RT,uv)*0.7;
//FragColor=max(FragColor,vec4(0.1,0.1,0.1,0.0));
//FragColor=texture(waveMap, fract(uv_now))*1;
//FragColor=vec4(uv*1.7-floor(uv*1.7),0.0,0);
//FragColor=vec4(1.0,1.0,0.0,0);
//FragColor=vec4(uv_now-floor(uv_now),0,0);
//FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
//  float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//     FragColor = vec4(average, average, average, 1.0);
//FragColor=vec4(1.0,0.0,1.0,1.0);
}