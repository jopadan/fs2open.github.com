/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/



#ifndef _FS2OPEN_RADARSETUP_H
#define _FS2OPEN_RADARSETUP_H

#include "hud/hud.h"
#include "hud/hudconfig.h"
#include "sound/sound.h"


//structures
#define NUM_FLICKER_TIMERS	2

class object;

typedef struct rcol {
	ubyte	r, g, b;
} rcol;

typedef struct blip	{
	blip	*prev, *next;
	int	rad;
	int	flags;
	color *blip_color;
	vec3d position;
	int radar_image_2d;
	int radar_color_image_2d;
	int radar_image_size;
	float radar_projection_size;

	float   dist;
	int		objnum;
} blip;


#define MAX_BLIPS 150

#define	MAX_RADAR_COLORS		5
#define MAX_RADAR_LEVELS		2		// bright and dim radar dots are allowed

#define RCOL_BOMB				0
#define RCOL_NAVBUOY_CARGO		1
#define RCOL_WARPING_SHIP		2
#define RCOL_JUMP_NODE			3
#define RCOL_TAGGED				4

extern rcol Radar_color_rgb[MAX_RADAR_COLORS][MAX_RADAR_LEVELS];

#define BLIP_TYPE_JUMP_NODE			0
#define BLIP_TYPE_NAVBUOY_CARGO		1
#define BLIP_TYPE_BOMB				2
#define BLIP_TYPE_WARPING_SHIP		3
#define BLIP_TYPE_TAGGED_SHIP		4
#define BLIP_TYPE_NORMAL_SHIP		5

#define MAX_BLIP_TYPES	6

extern blip	Blip_bright_list[MAX_BLIP_TYPES];		// linked list of bright blips
extern blip	Blip_dim_list[MAX_BLIP_TYPES];			// linked list of dim blips

extern blip	Blips[MAX_BLIPS];								// blips pool
extern int	N_blips;										// next blip index to take from pool

extern SCP_map<int, TIMESTAMP> Blip_last_update;	// map of objnums to timestamps

// blip flags
#define BLIP_CURRENT_TARGET	(1<<0)
#define BLIP_DRAW_DIM		(1<<1)	// object is farther than Radar_bright_range units away
#define BLIP_DRAW_DISTORTED	(1<<2)	// object is resistant to sensors, so draw distorted

extern float	Radar_bright_range;				// range within which the radar blips are bright
extern TIMESTAMP	Radar_calc_bright_dist_timer;	// timestamp at which we recalc Radar_bright_range

extern int See_all;

enum RadarVisibility
{
	NOT_VISIBLE, //!< Not visible on the radar
	VISIBLE, //!< Fully visible on the radar
	DISTORTED //!< Visible but not fully
};

void radar_frame_init();
void radar_mission_init();
void radar_plot_object( object *objp );
RadarVisibility radar_is_visible( object *objp );

extern sound_handle Radar_static_looping;

class HudGaugeRadar: public HudGauge
{
protected:
	// user defined members
	int Radar_radius[2];
	int Radar_dist_offsets[RR_MAX_RANGES][2];

	int Radar_blip_radius_normal;
	int Radar_blip_radius_target;

	bool Radar_static_playing;			// is static currently playing on the radar?
	TIMESTAMP Radar_static_next;		// next time to toggle static on radar
	bool Radar_avail_prev_frame;		// was radar active last frame?
	TIMESTAMP Radar_death_timer;		// timestamp used to play static on radar

	TIMESTAMP	Radar_flicker_timer[NUM_FLICKER_TIMERS];					// timestamp used to flicker blips on and off
	bool		Radar_flicker_on[NUM_FLICKER_TIMERS];

	ubyte Radar_infinity_icon;
public:
	HudGaugeRadar();
	HudGaugeRadar(int _gauge_object, int r, int g, int b);
	void initRadius(int w, int h);
	void initBlipRadius(int normal, int target);
	void initDistanceShortOffsets(int x, int y);
	void initDistanceLongOffsets(int x, int y);
	void initDistanceInfinityOffsets(int x, int y);
	void initInfinityIcon();

	void drawRange(bool config);
	void render(float frametime, bool config = false) override;
	void initialize() override;
	void pageIn() override;
};

#endif //_FS2OPEN_RADARSETUP_H
