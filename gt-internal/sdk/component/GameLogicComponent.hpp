#pragma once
#include "EntityComponent.hpp"
#include "../world/World.hpp"
#include "../world/WorldRenderer.hpp"
#include "../player/NetAvatar.hpp"
#include "../player/PlayerItems.hpp"
#include "../manager/NetObjectManager.hpp"
#include "../manager/TextManager.hpp"
#include "../manager/HudIndicatorManager.hpp"
#include "../manager/EffectManager.hpp"

#pragma pack(push, 1)
class GameLogicComponent : public EntityComponent
{
public:
	pad(88);
	World* m_world;
	WorldRenderer* m_world_renderer;
	pad(88);
	NetObjectManager m_net_object_manager;
	NetAvatar* m_local_player;
	TextManager m_text_manager;
	HudIndicatorManager* m_hud_indicator_manager;
	PlayerItems m_player_items;
	EffectManager m_effect_manager;
};
#pragma pack(pop)