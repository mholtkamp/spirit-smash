#ifndef CONSTANTS_H
#define CONSTANTS_H

#define LIGHTING_INTERVALS 3
#define LIGHTING_AMBIENT_INTENSITY 0.2f

#define SPIRIT_DEFAULT_SPEED 6.0f
#define SPIRIT_DEFAULT_ACCELERATION 24.0f
#define SPIRIT_DRAG 20.0f
#define SPIRIT_JUMP_VELOCITY 12.0f
#define SPIRIT_JUMP_FRAME_WINDOW 5
#define SPIRIT_GROUNDING_SWEEP_DISTANCE 0.01f
#define SPIRIT_SCALE 0.2f
#define SPIRIT_HALF_EXTENT_X 2.0f
#define SPIRIT_HALF_EXTENT_Y 5.0f
#define SPIRIT_HALF_EXTENT_Z 2.0f
#define SPIRIT_ATTACK_TIME 0.4f

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1
#define DIRECTION_DOWN 2
#define DIRECTION_UP 3

#define GRAVITY 12.0f
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

#endif