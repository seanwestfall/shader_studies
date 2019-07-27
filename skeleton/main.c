static Mesh halo("halo.material",//     required material file)
         "halo.mesh.xml",        //     required mesh file
         "halo.skeleton.xml");   // optional skeleton file
 
int idle = halo.animation.GetAnimationIndexOf("idle");
 
halo.animation.SetPose(idle, // animation id (2 animations are available)
              time_elapsed); // time in seconds
 
halo.Draw( vec3f(0,0,0),     // position
           vec3f(0,0,0),     // rotation
        0);                  // LOD level

int index  = halo.animation.GetBoneIndexOf("joint1"); 
matrix44 m = halo.animation.bones[ index ].matrix;

// get the index
int index  = halo.animation.GetBoneIndexOf("joint2"); 
 
// get / modify / set the matrix
matrix44 m = halo.animation.bones[ index ].matrix;
m.x_component()=vec3f(1,0,0);
m.y_component()=vec3f(0,1,0); // set the rotation to identity
m.z_component()=vec3f(0,0,1);
halo.animation.bones[ index ].matrix=m;
 
// re-evaluate the child bones
loopi(0,halo.animation.bones[ index ].childs.size())
{
    halo.animation.EvalSubtree(
        halo.animation.bones[ index ].childs[i], // bone id
        halo.animation.animations[0],            // animation
        -1);                                     // key frame -1 means not use the animation
}

uniform mat4 bones[100];
uniform int  use_skinning;
  
void main(void)
{
  mat4 mfinal = gl_ModelViewMatrix ;
 
  // skinning
  if(use_skinning==1)
  {
    vec3 weights= gl_Color.xyz;
    vec3 boneid = gl_Vertex.w * vec3( 1.0/128.0 , 1.0 , 128.0 );
    boneid = (boneid - floor(boneid))*128.0;
 
    mat4 mskin  = bones[int(boneid.x)]*weights.x+
                  bones[int(boneid.y)]*weights.y+
                  bones[int(boneid.z)]*weights.z;
    mfinal = mfinal * mskin;
  } 
  gl_Position = gl_ProjectionMatrix * mfinal * vec4(gl_Vertex.xyz,1.0);
}
