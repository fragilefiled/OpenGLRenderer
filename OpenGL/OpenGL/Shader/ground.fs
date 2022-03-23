
# version 450 core
out vec4 FragColor;
uniform sampler2D texture_Diffuse_0;
uniform sampler2D texture_Specular_0;
uniform sampler2D texture_Normal_0;
in vec2 uv;
in vec3 t1;
in vec3 t2;
in vec3 t3;
struct DirLight
{
    vec3 lightDir;
    vec3 lightPos;
    vec4 lightColor;
    
};
uniform DirLight dirLight;

void main(){

vec3 bump=texture(texture_Normal_0,uv).xyz;
bump=normalize(vec3(dot(t1, bump), dot(t2, bump), dot(t3, bump)));

FragColor=texture(texture_Diffuse_0,uv);
FragColor=pow(vec4(dot(-normalize(dirLight.lightDir),bump))*texture(texture_Diffuse_0,uv),vec4(1.0/2.2));
//FragColor=texture(texture_Normal_0,uv);
}