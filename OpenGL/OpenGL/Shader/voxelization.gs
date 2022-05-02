#version 430 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 3)out;
in VS_OUT{
 vec3 worldPos;
  vec3 normal;
 vec2 texCoord;
}gs_in[];
in vec4 lastnormal[];
uniform mat4 voxelViewProjection[3];
uniform mat4 voxelViewProjectionInverse[3];
uniform float voxelResolution;

uniform vec3 worldMinPoint;
uniform float voxelWidthWorld;
uniform float RasterScale;
out GS_OUT{
 vec4 AABB;
 vec2 limitZ;
 vec4 projectPos;
 vec2 texCoord;
 vec4 worldTexCoord;
 vec3 normal;
 vec4 lightProjPos;
}gs_out;
uniform mat4 lightMat;
int CalculateAxis(vec3 normal)
{
    
    float xdot=abs(dot(normal,vec3(1.0,0,0)));
    float ydot=abs(dot(normal,vec3(0,1.0,0)));
    float zdot=abs(dot(normal,vec3(0,0,1.0)));
    if(xdot>ydot&&xdot>zdot)
    return 0;
   else if(ydot>xdot&&ydot>zdot)
    return 1;
   else
    return 2;
}
void projectToPlane(int index)
{
 
    
    gl_Position = voxelViewProjection[index]*vec4(gs_in[0].worldPos,1.0);
    gs_out.texCoord=gs_in[0].texCoord;
    EmitVertex();
     gl_Position = voxelViewProjection[index]*vec4(gs_in[1].worldPos,1.0);
     gs_out.texCoord=gs_in[1].texCoord;
    EmitVertex();
     gl_Position = voxelViewProjection[index]*vec4(gs_in[2].worldPos,1.0);
     gs_out.texCoord=gs_in[2].texCoord;
    EmitVertex();
    EndPrimitive();
}
void main()
{

    vec2 texCoord[3];
    for (int i = 0; i < 3; i++)
    {
        texCoord[i] = gs_in[i].texCoord; 
    }
    vec2 halfPixelSize=vec2(1.0/voxelResolution*RasterScale);
    vec3 dxy=gs_in[2].worldPos-gs_in[0].worldPos;
    vec3 dxz=gs_in[1].worldPos-gs_in[0].worldPos;    
    vec3 facenormal=normalize(cross(dxy,dxz));
    
    
    int projectIndex=CalculateAxis(facenormal);
    
    vec4 pos_project[3];
    pos_project[0] = voxelViewProjection[projectIndex]*vec4(gs_in[0].worldPos,1.0);
    pos_project[1] = voxelViewProjection[projectIndex]*vec4(gs_in[1].worldPos,1.0);
    pos_project[2] = voxelViewProjection[projectIndex]*vec4(gs_in[2].worldPos,1.0);

    // //if enable conservative rasterization 
    // vec4 trianglePlane=vec4(facenormal,-dot(facenormal,pos_project[0].xyz));
    // if(trianglePlane.z == 0.0f) return;
    //  if (dot(trianglePlane.xyz, vec3(0.0, 0.0, 1.0)) < 0.0)
    // {
    //     vec4 vertexTemp = pos_project[2];
    //     vec2 texCoordTemp = texCoord[2];
        
    //     pos_project[2] = pos_project[1];
    //     texCoord[2] = texCoord[1];
    
    //     pos_project[1] = vertexTemp;
    //     texCoord[1] = texCoordTemp;
    // }



    // gs_out.AABB.xy=min(min(pos_project[0].xy,pos_project[1].xy),pos_project[2].xy);
    // gs_out.AABB.zw=max(max(pos_project[0].xy,pos_project[1].xy),pos_project[2].xy);
    // gs_out.AABB.xy-=halfPixelSize;
    // gs_out.AABB.zw+=halfPixelSize;
    // vec3 plane[3];
    // plane[0]=cross(pos_project[0].xyw-pos_project[2].xyw,pos_project[2].xyw);//ca oc
    // plane[1]=cross(pos_project[1].xyw-pos_project[0].xyw,pos_project[0].xyw);//ab oa
    // plane[2]=cross(pos_project[2].xyw-pos_project[1].xyw,pos_project[1].xyw);//bc ob

    // plane[0].z-=dot(halfPixelSize,abs(plane[0].xy));
    // plane[1].z-=dot(halfPixelSize,abs(plane[1].xy));
    // plane[2].z-=dot(halfPixelSize,abs(plane[2].xy));

    // vec3 pos_project_expand[3];//intersection 与欧式平面的交点
    // pos_project_expand[0]=cross(plane[0],plane[1]);
    // pos_project_expand[1]=cross(plane[1],plane[2]);
    // pos_project_expand[2]=cross(plane[2],plane[0]);

    // pos_project_expand[0]/=pos_project_expand[0].z;
    // pos_project_expand[1]/=pos_project_expand[1].z;
    // pos_project_expand[2]/=pos_project_expand[2].z;

    
    // vec4 pos_project_over[3];
    // pos_project_over[0].z=-(trianglePlane.x*pos_project_expand[0].x+trianglePlane.y*pos_project_expand[0].y+trianglePlane.w)/trianglePlane.z;
    // pos_project_over[1].z=-(trianglePlane.x*pos_project_expand[1].x+trianglePlane.y*pos_project_expand[1].y+trianglePlane.w)/trianglePlane.z;
    // pos_project_over[2].z=-(trianglePlane.x*pos_project_expand[2].x+trianglePlane.y*pos_project_expand[2].y+trianglePlane.w)/trianglePlane.z;
    
    // pos_project_over[0].z=pos_project[0].z;
    // pos_project_over[1].z=pos_project[1].z;
    // pos_project_over[2].z=pos_project[2].z;
    // pos_project_over[0].xy=pos_project_expand[0].xy;
    // pos_project_over[1].xy=pos_project_expand[1].xy;
    // pos_project_over[2].xy=pos_project_expand[2].xy;
    // pos_project_over[0].w=pos_project[0].w;
    // pos_project_over[1].w=pos_project[1].w;
    // pos_project_over[2].w=pos_project[2].w;
    // // float z[3];
    // // z[0]=0.9*pos_project[0].z+0.1*pos_project_over[0].z;
    // // z[1]=0.9*pos_project[1].z+0.1*pos_project_over[1].z;
    // // z[2]=0.9*pos_project[2].z+0.1*pos_project_over[2].z;
    
    // // limitZ=vec2(min(z[0],min(z[1],z[2])),max(z[0],max(z[1],z[2])));
    // vec2 limitZ=vec2(min(pos_project[0].z,min(pos_project[1].z,pos_project[2].z)),max(pos_project[0].z,max(pos_project[1].z,pos_project[2].z)));
    // limitZ+=vec2(-3.0/voxelResolution,3.0/voxelResolution);
    // //if enable conservative rasterization 
    // for(int i=0;i<3;i++)
    // {
    //     vec4 voxelPos=voxelViewProjectionInverse[projectIndex]*vec4(pos_project_over[i]);
    //     voxelPos.xyz/=voxelPos.w;
        
    //     vec4 voxelwsPos=voxelPos-vec4(worldMinPoint,0.0); 

    //     voxelwsPos.xyz=voxelwsPos.xyz/voxelWidthWorld*(voxelResolution);  
    //     gl_Position=vec4(pos_project_over[i]);
    //     gs_out.projectPos=pos_project_over[i];
    //     gs_out.texCoord=texCoord[i];
    //     gs_out.worldTexCoord=voxelwsPos;
    //     gs_out.limitZ=limitZ;
    //     gs_out.normal=gs_in[i].normal;
    //     EmitVertex();
    // }
    
  
    //if enable msaa
    vec2 limitZ=vec2(min(pos_project[0].z,min(pos_project[1].z,pos_project[2].z)),max(pos_project[0].z,max(pos_project[1].z,pos_project[2].z)));
    for(int i=0;i<3;i++)
    {
        vec4 voxelPos=voxelViewProjectionInverse[projectIndex]*vec4(pos_project[i]);
        voxelPos.xyz/=voxelPos.w;
        
        vec4 voxelwsPos=voxelPos-vec4(worldMinPoint,0.0); 

        voxelwsPos.xyz=voxelwsPos.xyz/voxelWidthWorld*(voxelResolution);  
        gl_Position=vec4(pos_project[i]);
        gs_out.projectPos=pos_project[i];
        gs_out.texCoord=texCoord[i];
        gs_out.worldTexCoord=voxelwsPos;
        gs_out.limitZ=limitZ;
        gs_out.normal=gs_in[i].normal;
        EmitVertex();
    }
    EndPrimitive();
}