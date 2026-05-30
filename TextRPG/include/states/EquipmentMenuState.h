#pragma once
#include "IState.h"

#include "ui/Selector.h"

#include "stats/Stats.h"

class Player;
class Equipment;

class EquipmentMenuState : public IState
{
  public:
	EquipmentMenuState(Player& player, StateContext& context);
	~EquipmentMenuState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void ProcessInputs() override;

	virtual bool Exit() override;

private:
	void DrawEquipment();
	void DrawPlayerInfo();
	void DrawStatPrediction();
	void DrawStatModifier(int x, int y, const std::wstring& stat, int value);

	void OnMenuSelect(int index, std::vector<std::wstring> data);
	void OnEquipSelect(int index, std::vector<std::shared_ptr<Equipment>> data);
	void OnSlotSelect(int index, std::vector<std::wstring> data);
	void RenderEquip(int x, int y, std::shared_ptr<Equipment> item);
	void RenderEquipSlots(int x, int y, const std::wstring& item);

	void RemoveEquipment(int index, std::vector<std::wstring>& data);

	void UpdateIndex();


private:
	StateContext& m_Context;
	Player& m_Player;

	Selector<> m_MenuSelector;
	Selector<> m_EquipSlotSelector;
	Selector<std::shared_ptr<Equipment>> m_EquipmentSelector;

	bool m_bExitGame;
	bool m_bInMenuSelect; 
	bool m_bInSlotSelect; 
	bool m_bRemoveEquipment;

	int m_ScreenWidth; 
	int m_ScreenHeight; 
	int m_CenterScreenW; 
	int m_PanelBarX; 
	int m_DiffPosY; 
	int m_PrevStatModPos; 
	int m_PrevIndex;

	std::wstring m_sCurrentSlot;
	Stats::EquipSlots m_eEquipSlots;
};
