#pragma once
#include "BaseApp.hpp"
#include "../component/GameLogicComponent.hpp"
#include "../component/TrackHandlerComponent.hpp"
#include "../manager/ItemInfoManager.hpp"
#include "../manager/TileSheetManager.hpp"
#include "../manager/PlayerTributeManager.hpp"
#include "../variant/VariantDB.hpp"

#pragma pack(push, 1)
class App : public BaseApp
{
public:
	VariantDB m_variant_db;
	pad(1640);
	TileSheetManager m_tile_sheet_manager;
	ItemInfoManager* m_item_info_manager;
	pad(48);
	TrackHandlerComponent* m_track_handler_component; //4576
	GameLogicComponent* m_game_logic_component;
	pad(24);


	Variant* GetVariant(const std::string& name)
	{
		return m_variant_db.GetVariant(name);
	}
			
	FunctionObject* GetFunction(const std::string& name)
	{
		return m_variant_db.GetFunction(name);
	}
};
#pragma pack(pop)