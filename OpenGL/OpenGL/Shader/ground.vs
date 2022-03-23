# version 450 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
layout (location = 3) in vec3 aTangent;

out vec2 uv;
uniform mat4 model;
out vec3 worldpos;
uniform mat4 model_inverse_t;
uniform sampler2D texture_Specular_0;
out vec3 t1;
out vec3 t2;
out vec3 t3;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    uv=aTexcoord;
    vec3 displacement=texture(texture_Specular_0,uv).xyz;
    vec3 worldtangent=(model*vec4(aTangent,0.0)).xyz;
    vec3 worldnormal=(model_inverse_t*vec4(aNormal,0.0)).xyz;
    vec3 worldBionormal=-cross(worldnormal,worldtangent);
    t1=vec3(worldtangent.x,worldBionormal.x,worldnormal.x);
    t2=vec3(worldtangent.y,worldBionormal.y,worldnormal.y);
    t3=vec3(worldtangent.z,worldBionormal.z,worldnormal.z);
    gl_Position=projection*view*model*vec4(aPos+vec3(0,displacement.y,0),1.0);


}