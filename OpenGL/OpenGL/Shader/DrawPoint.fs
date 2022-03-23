#version 450 core
in vec2 uv;
out vec4 FragColor;
in vec4 attri_use;
void main(){
//FragColor=vec4(ourColor,1.0f);

FragColor=vec4(uv,0,0);
FragColor=vec4(attri_use);
//FragColor=vec4(1.0,0,0,0);
//FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
//  float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//     FragColor = vec4(average, average, average, 1.0);
//FragColor=vec4(1.0,0.0,1.0,1.0);
}