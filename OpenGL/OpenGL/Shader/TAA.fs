#version 450 core
in vec2 uv;
uniform sampler2D screen_RT;
uniform sampler2D preScreen_RT;
uniform sampler2D gDepth;
uniform sampler2D gVelocity;
uniform vec2 resolution;
out vec4 FragColor;
uniform float TAAVarianceClipGamma;
float Luminance(vec3 color)
{
    return 0.25 * color.r + 0.5 * color.g + 0.25 * color.b;
}

vec3 ToneMap(vec3 color)
{
    return color / (1 + Luminance(color));
}

vec3 UnToneMap(vec3 color)
{
    return color / (1 - Luminance(color));
}

vec3 RGB2YCoCgR(vec3 rgbColor)
{
    vec3 YCoCgRColor;

    YCoCgRColor.y = rgbColor.r - rgbColor.b;
    float temp = rgbColor.b + YCoCgRColor.y / 2;
    YCoCgRColor.z = rgbColor.g - temp;
    YCoCgRColor.x = temp + YCoCgRColor.z / 2;

    return YCoCgRColor;
}

vec3 YCoCgR2RGB(vec3 YCoCgRColor)
{
    vec3 rgbColor;

    float temp = YCoCgRColor.x - YCoCgRColor.z / 2;
    rgbColor.g = YCoCgRColor.z + temp;
    rgbColor.b = temp - YCoCgRColor.y / 2;
    rgbColor.r = rgbColor.b + YCoCgRColor.y;

    return rgbColor;
}

vec3 clipAABB(vec3 nowColor, vec3 preColor)
{
    vec3 aabbMin = nowColor, aabbMax = nowColor;
    vec2 deltaRes = vec2(1.0f)/resolution;
    vec3 m1 = vec3(0), m2 = vec3(0);

    for(int i=-1;i<=1;++i)
    {
        for(int j=-1;j<=1;++j)
        {
            vec2 newUV = uv + deltaRes * vec2(i, j);
            vec3 C = RGB2YCoCgR(ToneMap(texture(screen_RT, newUV).rgb));
            // aabbMax = max(aabbMax, C+vec3(0.04));
            // aabbMin = min(aabbMin, C-vec3(0.04));
            m1 += C;
            m2 += C * C;
        }
    }

    // // Variance clip
    const int N = 9;
    const float VarianceClipGamma = TAAVarianceClipGamma;//1.0f
    vec3 mu = m1 / N;
    vec3 sigma = sqrt(abs(m2 / N - mu * mu));
    aabbMin = mu - VarianceClipGamma * sigma;
    aabbMax = mu + VarianceClipGamma * sigma;

    // clip to center
    vec3 p_clip = 0.5 * (aabbMax + aabbMin);
    vec3 e_clip = 0.5 * (aabbMax - aabbMin);

    vec3 v_clip = preColor - p_clip;
    vec3 v_unit = v_clip.xyz / e_clip;
    vec3 a_unit = abs(v_unit);
    float ma_unit = max(a_unit.x, max(a_unit.y, a_unit.z));

    if (ma_unit > 1.0)
        return p_clip + v_clip / ma_unit;
    else
        return preColor;

   // return clamp(preColor,aabbMin,aabbMin);
}

void main(){
//FragColor=vec4(ourColor,1.0f);
float closestdepth=1.0f;
vec2 closestuv=uv;
for(int i=-1;i<=1;i++)
for(int j=-1;j<=1;j++){
    vec2 uv_now=uv+vec2(i,j)/resolution;
    float depth_now=texture(gDepth,uv_now).x;
    if(depth_now<closestdepth)
    {
        closestdepth=depth_now;
        closestuv=uv_now;
    }
}
vec2 velocity=texture(gVelocity,closestuv).xy;
vec2 offsetUV = clamp(uv - velocity, 0, 1);
vec4 preColor=texture(preScreen_RT,offsetUV);
vec4 nowColor=texture(screen_RT,uv);

vec3 preColor_YCoCgR=RGB2YCoCgR(ToneMap(preColor.rgb));
vec3 nowColor_YCoCgR=RGB2YCoCgR(ToneMap(nowColor.rgb));
preColor_YCoCgR=clipAABB(nowColor_YCoCgR,preColor_YCoCgR);

preColor.rgb=UnToneMap(YCoCgR2RGB(preColor_YCoCgR));
float mixCoeff = 0.05;
FragColor = vec4(mixCoeff * nowColor + (1-mixCoeff) * preColor);

//FragColor=pow(texture(screen_RT, uv),vec4(1.0/2.2));
//FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
//  float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//     FragColor = vec4(average, average, average, 1.0);
//FragColor=vec4(1.0,0.0,1.0,1.0);
}