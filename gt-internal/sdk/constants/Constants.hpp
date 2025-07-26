#pragma once
#include "../proton/EncryptedFloat.hpp"
#include "../proton/Vector.hpp"

/*
* todo - @trailyy:
* - find constants, initialize them
*/

enum eConstants
{
    C_INVALID = -1,
    C_MAX_FALLING_SPEED,                                  // 400 
    C_PLAYER_FLOAT_POWER,                                 // 5000
    C_WIND_SPEED,                                         // 1000
    C_WIND_SPEED_AGAINST,                                 // 100
    C_LOCAL_COLLISION_FOR_NET_PLAYER_MAX_DIVERGENCE,      // 48
    C_PLAYER_DRAG,                                        // 1200
    C_INTIAL_JUMP_BOOST,                                  // -450
    C_CONTROL_MOD_WHEN_HURT,                              // 0.15
    C_LAVA_BOUNCE_POWER_X,                                // 300
    C_LAVA_BOUNCE_POWER_Y,                                // 800
    C_TRAMPOLINE_BOUNCE_POWER_Y,                          // 800
    C_BOUNCY_BOUNCE_POWER,                                // 500
    C_PUNCHED_KNOCKBACK_DEFAULT,                          // 400
    C_PARASOL_GRAV_REDUCE_MOD,                            // 0.3
    C_UNDER_WATER_GRAVITY_MOD,                            // 0.3
    C_MARS_GRAVITY_MOD,                                   // 0.75
    C_ONE_THOUSAND,                                       // 1000
    C_GHOST_SPEED,                                        // 330
    C_WATER_LEVITATION_TIME,                              // 3000
    C_PUNCH_RELOAD_TIME,                                  // 0.2
    C_DELAY_BETWEEN_HACK_REPORTS_MS,                      // 15000
    C_MAX_VALUE
};

#pragma pack(push, 1)
class Constants
{
public:
    EncryptedFloat m_constants[eConstants::C_MAX_VALUE];

    float Get(int constant) const { return m_constants[constant].Get(); }
    void Set(int constant, float val) { m_constants[constant].Set(val); }
};
#pragma pack(pop)