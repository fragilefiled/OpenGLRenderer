#version 450 core
in vec2 uv0[2];
in vec2 uv1[2];
in vec2 uv2[2];
in vec2 uv3;
uniform sampler2D screen_RT;
out vec4 FragColor;
in float depth;
float rgbTolumin(vec3 col){

  
 return col.x*0.299 + col.y*0.587 + col.z*0.114;
} 

float CalcLerpVal(vec4 col0,vec4 col1){
  float val0=rgbTolumin(col0.xyz);
  float val1=rgbTolumin(col1.xyz);
  return max(1.0-abs(val1-val0),0.0)*3.0;
}

void main(){
//FragColor=vec4(ourColor,1.0f);
   
float a[4]={0.37004405286,0.31718061674,0.19823788546,0.11453744493};
vec4 col=texture(screen_RT,uv3);
vec4 col0_1=texture(screen_RT,uv0[0]);  
vec4 col1_1=texture(screen_RT,uv0[1]);  
vec4 col0_2=texture(screen_RT,uv1[0]);  
vec4 col1_2=texture(screen_RT,uv1[1]); 
vec4 col0_3=texture(screen_RT,uv2[0]);  
vec4 col1_3=texture(screen_RT,uv2[1]); 
float x0_1=CalcLerpVal(col,col0_1)*a[1];
float x1_1=CalcLerpVal(col,col1_1)*a[1];
float x0_2=CalcLerpVal(col,col0_2)*a[2];
float x1_2=CalcLerpVal(col,col1_2)*a[2];
float x0_3=CalcLerpVal(col,col0_3)*a[3];
float x1_3=CalcLerpVal(col,col1_3)*a[3];
vec4 result=a[0]*col;
result+=x0_1*col0_1;
result+=x1_1*col1_1;
result+=x0_2*col0_2;
result+=x1_2*col1_2;
result+=x0_3*col0_3;
result+=x1_3*col1_3;

result/=(a[0]+x0_1+x1_1+x0_2+x1_2+x0_3+x1_3);
 
FragColor=max(result,0.00001);

//FragColor=vec4(1.0);
//FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
//  float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//     FragColor = vec4(average, average, average, 1.0);
//FragColor=vec4(1.0,0.0,1.0,1.0);
}