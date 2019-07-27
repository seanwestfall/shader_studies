/* C code, made for tabs of 8 spaces
 * uint8_t is defined in the standard C header stdint.h
 */

/* Define numbers and flags */
#define MAX_CHCOUNT			8	/* Max children count */
#define BONE_ABSOLUTE_ANGLE		0x01	/* Bone angle is absolute or relative to parent */
#define BONE_ABSOLUTE_POSITION		0x02	/* Bone position is absolute in the world or relative to the parent */
#define BONE_ABSOLUTE			(BONE_ABSOLUTE_ANGLE | BONE_ABSOLUTE_POSITION)

typedef struct _Bone
{
	char name[20];				/* Just for the sake of the example */
	float x,				/* Starting point x */
	      y,				/* Starting point y */
	      a,				/* Angle, in radians */
	      l;				/* Length of the bone */

	uint8_t flags;				/* Bone flags, 8 bits should be sufficient for now */
	uint8_t childCount;			/* Number of children */

	struct _Bone *child[MAX_CHCOUNT],	/* Pointers to children */
		     *parent;			/* Parent bone */
} Bone;

/* Create a bone and return it's address */
Bone *boneAddChild(Bone *root, float x, float y, float a, float l, Uint8 flags, char *name)
{
	Bone *t;
	int i;

	if (!root) /* If there is no root, create one */
	{
		if (!(root = (Bone *)malloc(sizeof(Bone))))
			return NULL;
		root->parent = NULL;
	}
	else if (root->childCount < MAX_CHCOUNT) /* If there is space for another child */
	{
		/* Allocate the child */
		if (!(t = (Bone *)malloc(sizeof(Bone))))
			return NULL; /* Error! */

		t->parent = root; /* Set it's parent */
		root->child[root->childCount++] = t; /* Increment the childCounter and set the pointer */
		root = t; /* Change the root */
	}
	else /* Can't add a child */
		return NULL;

	/* Set data */
	root->x = x;
	root->y = y;
	root->a = a;
	root->l = l;
	root->flags = flags;
	root->childCount = 0;

	if (name)
		strcpy(root->name, name);
	else
		strcpy(root->name, "Bone");

	for (i = 0; i < MAX_CHCOUNT; i++)
		root->child[i] = NULL;

	return root;
}

/* Free the bones */
Bone *boneFreeTree(Bone *root)
{
	int i;

	if (!root)
		return;

	/* Recursively call this function to free subtrees */
	for (i = 0; i < root->childCount; i++)
		boneFreeTree(root->child[i]);

	free(root);

	return NULL;
}

/* Dump on stdout the bone structure. Root of the tree should have level 1 */
void boneDumpTree(Bone *root, Uint8 level)
{
	int i;

	if (!root)
		return;

	for (i = 0; i < level; i++)
		printf("#"); /* We print # to signal the level of this bone. */

	printf(" %4.4f %4.4f %4.4f %4.4f %d %d %s\n", root->x, root->y,
              root->a, root->l, root->childCount, root->flags, root->name);

	/* Recursively call this on my children */
	for (i = 0; i < root->childCount; i++)
		boneDumpTree(root->child[i], level + 1);
}
