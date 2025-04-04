/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/



#ifndef __SHOCKWAVE_H__
#define __SHOCKWAVE_H__

#include "globalincs/globals.h"
#include "globalincs/pstypes.h"

#include "gamesnd/gamesnd.h"

class object;
class model_draw_list;

#define	SW_USED				(1<<0)
#define	SW_WEAPON			(1<<1)
#define	SW_SHIP_DEATH		(1<<2)
#define	SW_WEAPON_KILL		(1<<3)	// Shockwave created when weapon destroyed by another

#define	MAX_SHOCKWAVES					16

// -----------------------------------------------------------
// Data structures
// -----------------------------------------------------------

typedef struct shockwave_info
{
	char filename[MAX_FILENAME_LEN];
	int	bitmap_id;
	int model_id;
	int	num_frames;
	int	fps;
	
	shockwave_info()
	: num_frames( 0 ), fps( 0 )
	{ 
		filename[ 0 ] = '\0';
		bitmap_id = -1; 
		model_id = -1; 
	}
} shockwave_info;

typedef struct shockwave {
	shockwave	*next, *prev;
	int			flags;
	int			objnum;					// index into Objects[] for shockwave
	SCP_vector<std::pair<int, int>>			obj_sig_hitlist;
	float		speed, radius;
	float		inner_radius, outer_radius, damage;
	int			weapon_info_index;	// -1 if shockwave not caused by weapon	
	int			damage_type_idx;			//What type of damage this shockwave does to armor
	vec3d		pos;
	float		blast;					// amount of blast to apply
	int			next_blast;				// timestamp for when to apply next blast damage
	int			shockwave_info_index;
	int			current_bitmap;
	float		time_elapsed;			// in seconds
	float		total_time;				// total lifetime of animation in seconds
	int			delay_stamp;			// for delayed shockwaves
	angles		rot_angles;
	int			model_id;
	gamesnd_id  blast_sound_id;
} shockwave;

typedef struct shockwave_create_info {

	char name[MAX_FILENAME_LEN];
	char pof_name[MAX_FILENAME_LEN];

	float inner_rad;		// max damage out to this distance
	float outer_rad;		// 0 damage at this distance or more, outer_rad / speed is total time
	float damage;
	float blast;
	float speed;
	int radius_curve_idx;   // curve for shockwave radius over time
	angles rot_angles;
	bool rot_defined;		// if the modder specified rot_angles
	bool damage_overridden;  // did this have shockwave damage specifically set or not

	int damage_type_idx;
	int damage_type_idx_sav;	// stored value from table used to reset damage_type_idx

	gamesnd_id blast_sound_id;	// allow setting unique sounds for a ship or weapon shockwave --wookieejedi

} shockwave_create_info;

extern bool Use_3D_shockwaves;

extern void shockwave_create_info_init(shockwave_create_info *sci);
extern void shockwave_create_info_load(const shockwave_create_info *sci);

void shockwave_level_init();
void shockwave_level_close();
void shockwave_delete(const object *objp);
void shockwave_move_all(float frametime);
int  shockwave_create(int parent_objnum, const vec3d *pos, const shockwave_create_info *sci, int flag, int delay = -1);
void shockwave_render(const object *objp, model_draw_list *scene);
int shockwave_load(const char *s_name, bool shock_3D = false);

int   shockwave_get_weapon_index(int index);
float shockwave_get_min_radius(int index);
float shockwave_get_max_radius(int index);
float shockwave_get_damage(int index);
int   shockwave_get_damage_type_idx(int index);
int   shockwave_get_framenum(const int index, const int ani_id);
int   shockwave_get_flags(int index);

#endif /* __SHOCKWAVE_H__ */
