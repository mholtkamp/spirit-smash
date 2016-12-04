#ifndef CONSTANTS_H
#define CONSTANTS_H

#define LIGHTING_INTERVALS 3
#define LIGHTING_AMBIENT_INTENSITY 0.2f

#define SPIRIT_DEFAULT_SPEED 6.0f
#define SPIRIT_DEFAULT_ACCELERATION 24.0f
#define SPIRIT_DRAG 20.0f
#define SPIRIT_JUMP_VELOCITY 12.0f
#define SPIRIT_JUMP_FRAME_WINDOW 3
#define SPIRIT_GROUNDING_SWEEP_DISTANCE 0.01f
#define SPIRIT_SCALE 0.2f
#define SPIRIT_HALF_EXTENT_X 2.0f
#define SPIRIT_HALF_EXTENT_Y 5.0f
#define SPIRIT_HALF_EXTENT_Z 2.0f
#define SPIRIT_ATTACK_TIME 0.4f
#define SPIRIT_DEFAULT_DAMAGE 7
#define SPIRIT_DEFAULT_CHARGE_RATE 0.3f
#define SPIRIT_BASE_KNOCKBACK_SPEED 9.0f
#define SPIRIT_PERCENT_KNOCKBACK_MULTIPLIER 0.35f
#define SPIRIT_STARTING_LIVES 3
#define SPIRIT_MAX_ORBS 3
#define SPIRIT_CHARGING_MOVE_SCALE 0.3f

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1
#define DIRECTION_DOWN 2
#define DIRECTION_UP 3

#define GRAVITY 12.0f
#define FAST_FALL_GRAVITY (GRAVITY * 3)
#define FIELD_TYPE_FOREST 0
#define FIELD_FOREST_PLATFORM_COUNT 12

#define PLATFORM_SECTION_WIDTH 2.0f
#define PLATFORM_HALF_EXTENT 1.0f
#define PLATFORM_COLLISION_OFFSET 1.0f

#define KILL_LEFT 0 
#define KILL_RIGHT 1
#define KILL_TOP 2 
#define KILL_BOTTOM 3

#define MAX_PLAYERS 4

#define MIDDLEGROUND_Z  0.0f
#define BACKGROUND_Z   -8.0f
#define FOREGROUND_Z    4.0f

#define DEBUG_CAMERA_SPEED 5.0f

#define CAMERA_Z_OFFSET 15.0f
#define CAMERA_Z_SCALE_FACTOR 0.8f
#define DEAD_ZONE 0.2f

#define ATTACK_VOLUME_HALF_EXTENT_X 0.5f
#define ATTACK_VOLUME_HALF_EXTENT_Y 0.5f
#define ATTACK_VOLUME_HALF_EXTENT_Z 1.0f
#define ATTACK_VOLUME_X_OFFSET 0.7f

#define ORB_MIN_SIZE 0.3f
#define ORB_MAX_SIZE 1.0f
#define ORB_HALF_EXTENTS 0.5f
#define ORB_LIGHT_RADIUS_SCALE 12.0f
#define ORB_LIGHT_INTENSITY 1.0f
#define ORB_RIM_SIZE 0.5f
#define ORB_BASE_SPEED 8.0f
#define ORB_SPEED_SIZE_SCALE 8.0f
#define ORB_SPIRIT_OFFSET_X 0.7f
#define ORB_SPIRIT_OFFSET_Y -0.3f
#define ORB_LIGHT_BOX_EXTENTS 15.0f
#define ORB_INSTIGATOR_OFFSET 0.5f
#define ORB_DAMAGE_SCALE 15.0f
#define ORB_MIN_DAMAGE 1
#define ORB_MAX_DAMAGE 15

#define HUD_X -0.7f
#define HUD_Y -0.8f
#define HUD_SPACING 0.4f

#define HUD_PERCENT_OFF_X -0.07f
#define HUD_PERCENT_OFF_Y -0.03f
#define HUD_PERCENT_SCALE_X 1.0f
#define HUD_PERCENT_SCALE_Y 1.0f
#define HUD_PERCENT_OFF_X_1 (HUD_PERCENT_OFF_X + 0.022f)
#define HUD_PERCENT_OFF_X_2 (HUD_PERCENT_OFF_X + 0.002f)
#define HUD_PERCENT_OFF_X_3 (HUD_PERCENT_OFF_X - 0.018f)

#define HUD_BUBBLE_OFF_X -0.07f
#define HUD_BUBBLE_OFF_Y -0.17f
#define HUD_BUBBLE_SPACING 0.04f
#define HUD_BUBBLE_WIDTH 0.02f
#define HUD_BUBBLE_HEIGHT 0.035f

#define HUD_BACKGROUND_OFF_X -0.1f
#define HUD_BACKGROUND_OFF_Y -0.1f
#define HUD_BACKGROUND_WIDTH 0.15f
#define HUD_BACKGROUND_HEIGHT 0.2f
#define HUD_BACKGROUND_ALPHA 0.5f

#endif