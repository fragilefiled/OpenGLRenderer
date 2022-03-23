#version 330 core
layout(triangles) in;
layout(line_strip,max_vertices = 6)out;
in VS_OUT{
 vec3 worldPos;
  vec3 normal;
 vec2 texCoord;
}gs_in[];
in vec4 lastnormal[];

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = (lastnormal[index]-gl_in[index].gl_Position)*0.3+gl_in[index].gl_Position;
    EmitVertex();
    EndPrimitive();
}
void main()
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}