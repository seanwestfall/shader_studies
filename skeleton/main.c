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

int main(int argc, char **argv)
{
	Bone *root,
	     *tmp,
	     *tmp2;

	int i;
	
	/* Create a root bone
	 * this is a "null" bone which represent a single point, which is the center of the structure.
	 * Do you remember the sea star example above?
	 */
	if (!(root = boneAddChild(NULL, 100, 100, 0, 0, 0, "NullBone")))
	{
		fprintf(stderr, "Error! Can't create a root!\n");
		exit(EXIT_FAILURE);
	}

	/* Creating a bone which has (x,y) == (0,0) and BONE_ABSOLUTE_POSITION NOT set
	 *  causes this bone to start where its parent ends.
	 * If ABSOLUTE_POSITION is off, x and y work as offsets with respect to the parents end position.
	 * If it's on, then (x,y) will be placed at an absolute position on the screen.
	 */
	boneAddChild(root, 100, 100, M_PI_2, 10, BONE_ABSOLUTE, "Head");
	tmp = boneAddChild(root, 0, 0, -M_PI_2, 30, 0, "Back");
 	tmp2 = boneAddChild(tmp, 0, 0, -M_PI_4, 30, 0, "LLeg");
	boneAddChild(tmp2, 0, 0, 0, 30, 0, "LLeg2");
	tmp2 = 	boneAddChild(tmp, 0, 0, -2 * M_PI_4, 30, 0, "RLeg");
 	boneAddChild(tmp2, 0, 0, 0, 30, 0, "RLeg2");
	tmp = boneAddChild(root, 0, 0, 0, 20, 0, "LArm");
	boneAddChild(tmp, 0, 0, 0, 20, 0, "LArm2");
	tmp = boneAddChild(root, 0, 0, M_PI, 20, 0, "RArm");
	boneAddChild(tmp, 0, 0, M_PI, 20, 0, "RArm2");

	boneDumpTree(root, 0);
	
	root = boneFreeTree(root);

	return EXIT_SUCCESS;
}
