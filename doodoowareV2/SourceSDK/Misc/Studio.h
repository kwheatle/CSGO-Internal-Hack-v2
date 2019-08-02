#pragma once

#include "..\Math\Math.h"
#include "..\sdk.h"

typedef float Quaternion[4];
typedef float RadianEuler[3];

#define MAX_QPATH  260

#define BONE_CALCULATE_MASK             0x1F
#define BONE_PHYSICALLY_SIMULATED       0x01    // bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL         0x02    // procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL          0x04    // bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE        0x08    // bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER      0x10    // bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK                  0x0007FF00
#define BONE_USED_BY_ANYTHING           0x0007FF00
#define BONE_USED_BY_HITBOX             0x00000100    // bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT         0x00000200    // bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK        0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0        0x00000400    // bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1        0x00000800    
#define BONE_USED_BY_VERTEX_LOD2        0x00001000  
#define BONE_USED_BY_VERTEX_LOD3        0x00002000
#define BONE_USED_BY_VERTEX_LOD4        0x00004000
#define BONE_USED_BY_VERTEX_LOD5        0x00008000
#define BONE_USED_BY_VERTEX_LOD6        0x00010000
#define BONE_USED_BY_VERTEX_LOD7        0x00020000
#define BONE_USED_BY_BONE_MERGE         0x00040000    // bone is available for bone merge to occur against it

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8
#define MAXSTUDIOBONES		128		// total bones actually used
#define MAXSTUDIOSKINS		32

#define BONE_TYPE_MASK                  0x00F00000
#define BONE_FIXED_ALIGNMENT            0x00100000    // bone can't spin 360 degrees, all interpolation is normalized around a fixed orientation

#define BONE_HAS_SAVEFRAME_POS          0x00200000    // Vector48
#define BONE_HAS_SAVEFRAME_ROT64        0x00400000    // Quaternion64
#define BONE_HAS_SAVEFRAME_ROT32        0x00800000    // Quaternion32


enum modtype_t
{
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

enum Hitboxes
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_STOMACH,
	HITBOX_UPPER_STOMACH,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_SHIN,
	HITBOX_RIGHT_SHIN,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_MAX,
};

/*

Head = 0,
Neck,
Pelvis,
Stomach,
LowerChest,
Chest,
UpperChest,
RightThigh,
LeftThigh,
RightShin,
LeftShin,
RightFoot,
LeftFoot,
RightHand,
LeftHand,
RightUpperArm,
RightLowerArm,
LeftUpperArm,
LeftLowerArm,
Max,

HITBOX_HEAD,
HITBOX_NECK,
HITBOX_LOWER_NECK,
HITBOX_PELVIS,
HITBOX_STOMACH,
HITBOX_UPPER_STOMACH,
HITBOX_CHEST,
HITBOX_UPPER_CHEST,
HITBOX_LEFT_THIGH,
HITBOX_RIGHT_THIGH,
HITBOX_LEFT_SHIN,
HITBOX_RIGHT_SHIN,
HITBOX_LEFT_FOOT,
HITBOX_RIGHT_FOOT,
HITBOX_LEFT_HAND,
HITBOX_RIGHT_HAND,
HITBOX_LEFT_UPPER_ARM,
HITBOX_LEFT_FOREARM,
HITBOX_RIGHT_UPPER_ARM,
HITBOX_RIGHT_FOREARM



*/

typedef unsigned short MDLHandle_t;

struct mstudiobbox_t
{
	int     bone;
	int     group;
	Vector  bbmin;
	Vector  bbmax;
	int     szhitboxnameindex;
	int     unused[8];

	char* GetHitboxName(void)
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((char*)this) + szhitboxnameindex;
	}
};
struct mstudiohitboxset_t {
	int sznameindex;

	inline char *const pszName() const
	{
		return ((char *) this) + sznameindex;
	}

	int numhitboxes;
	int hitboxindex;

	inline mstudiobbox_t *GetHitbox(int i) const
	{
		return (mstudiobbox_t *)(((unsigned char *) this) + hitboxindex) + i;
	};
};
struct mstudiobone_t
{
	int                    sznameindex;
	inline char * const    pszName(void) const { return ((char *) this) + sznameindex; }
	int                    parent;
	int                    bonecontroller[6];    // bone controller index, -1 == none
	Vector                 pos;
	Quaternion             quat;
	RadianEuler            rot;
	// compression scale
	Vector                 posscale;
	Vector                 rotscale;

	matrix3x4_t            poseToBone;
	Quaternion             qAlignment;
	int                    flags;
	int                    proctype;
	int                    procindex;
	mutable int            physicsbone;
	inline void *          pProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *) this) + procindex); };
	int                    surfacepropidx;
	inline char * const    pszSurfaceProp(void) const { return ((char *) this) + surfacepropidx; }
	inline int             GetSurfaceProp(void) const { return surfacepropLookup; }

	int                    contents;
	int                    surfacepropLookup;
	int                    m_iPad01[7];

	mstudiobone_t() {}
private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t& vOther);
};
class studiohdr_t
{
public:
	int id;                     //0x0000 
	int version;                //0x0004 
	long    checksum;               //0x0008 
	char    szName[64];             //0x000C 
	int length;                 //0x004C 
	Vector  vecEyePos;              //0x0050 
	Vector  vecIllumPos;            //0x005C 
	Vector  vecHullMin;             //0x0068 
	Vector  vecHullMax;             //0x0074 
	Vector  vecBBMin;               //0x0080 
	Vector  vecBBMax;               //0x008C 
	int flags;                  //0x0098 
	int numbones;               //0x009C 
	int boneindex;              //0x00A0 
	int numbonecontrollers;     //0x00A4 
	int bonecontrollerindex;    //0x00A8 
	int numhitboxsets;          //0x00AC 
	int hitboxsetindex;         //0x00B0 
	int numlocalanim;           //0x00B4 
	int localanimindex;         //0x00B8 
	int numlocalseq;            //0x00BC 
	int localseqindex;          //0x00C0 
	int activitylistversion;    //0x00C4 
	int eventsindexed;          //0x00C8 
	int numtextures;            //0x00CC 
	int textureindex;           //0x00D0
	int		skinreference_count;
	int		skinrfamily_count;
	int             skinreference_index;

	// mstudiobodyparts_t
	int		bodypart_count;
	int		bodypart_offset;

	// Local attachment points		
	// mstudioattachment_t
	int		attachment_count;
	int		attachment_offset;

	// Node values appear to be single bytes, while their names are null-terminated strings.
	int		localnode_count;
	int		localnode_index;
	int		localnode_name_index;

	// mstudioflexdesc_t
	int		flexdesc_count;
	int		flexdesc_index;

	// mstudioflexcontroller_t
	int		flexcontroller_count;
	int		flexcontroller_index;

	// mstudioflexrule_t
	int		flexrules_count;
	int		flexrules_index;

	// IK probably referse to inverse kinematics
	// mstudioikchain_t
	int		ikchain_count;
	int		ikchain_index;

	// Information about any "mouth" on the model for speech animation
	// More than one sounds pretty creepy.
	// mstudiomouth_t
	int		mouths_count;
	int		mouths_index;

	// mstudioposeparamdesc_t
	int		localposeparam_count;
	int		localposeparam_index;

	/*
	* For anyone trying to follow along, as of this writing,
	* the next "surfaceprop_index" value is at position 0x0134 (308)
	* from the start of the file.
	*/

	// Surface property value (single null-terminated string)
	int		surfaceprop_index;

	// Unusual: In this one index comes first, then count.
	// Key-value data is a series of strings. If you can't find
	// what you're interested in, check the associated PHY file as well.
	int		keyvalue_index;
	int		keyvalue_count;

	// More inverse-kinematics
	// mstudioiklock_t
	int		iklock_count;
	int		iklock_index;


	float		mass; 		// Mass of object (4-bytes)
	int		contents;	// ??

						// Other models can be referenced for re-used sequences and animations
						// (See also: The $includemodel QC option.)
						// mstudiomodelgroup_t
	int		includemodel_count;
	int		includemodel_index;

	int		virtualModel;	// Placeholder for mutable-void*

							// mstudioanimblock_t
	int		animblocks_name_index;
	int		animblocks_count;
	int		animblocks_index;

	int		animblockModel; // Placeholder for mutable-void*

							// Points to a series of bytes?
	int		bonetablename_index;

	int		vertex_base;	// Placeholder for void*
	int		offset_base;	// Placeholder for void*

							// Used with $constantdirectionallight from the QC 
							// Model should have flag #13 set if enabled
	byte		directionaldotproduct;

	byte		rootLod;	// Preferred rather than clamped

							// 0 means any allowed, N means Lod 0 -> (N-1)
	byte		numAllowedRootLods;

	byte		unused; // ??
	int		unused1; // ??

					// mstudioflexcontrollerui_t
	int		flexcontrollerui_count;
	int		flexcontrollerui_index;

	/**
	* Offset for additional header information.
	* May be zero if not present, or also 408 if it immediately
	* follows this studiohdr_t
	*/
	// studiohdr2_t
	int		studiohdr2index;

	int		unused2; // ??

	mstudiohitboxset_t* GetHitboxSet(int i)
	{
		if (i > numhitboxsets) return nullptr;
		return (mstudiohitboxset_t*)((byte*)this + hitboxsetindex) + i;
	}
	mstudiobone_t* GetBone(int i)
	{
		if (i > numbones) return nullptr;
		return (mstudiobone_t*)((uint8_t*)this + boneindex) + i;
	}

};//Size=0x00D4