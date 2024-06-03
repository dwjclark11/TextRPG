#pragma once
#include "IState.h"
#include "../Selector.h"
#include "../Equipment.h"
#include "../Stats.h"

class Console;
class StateMachine;
class Keyboard;
class Player;

class EquipmentMenuState : public IState
{
  private:
	const int PANEL_BARS = 90;
	const int EQUIP_SIZE = 52;
	const int STAT_PREDICT_X_POS = 75;
	const int STAT_VAL_X_POS = 50;
	const int STAT_LABEL_X_POS = 30;
	const int STAT_LABEL_START_Y_POS = 23;

	Console& m_Console;
	StateMachine& m_StateMachine;
	Keyboard& m_Keyboard;
	Player& m_Player;

	Selector<> m_MenuSelector;
	Selector<> m_EquipSlotSelector;
	Selector<std::shared_ptr<Equipment>> m_EquipmentSelector;

	bool m_bExitGame, m_bInMenuSelect, m_bInSlotSelect, m_bRemoveEquipment;
	int m_ScreenWidth, m_ScreenHeight, m_CenterScreenW, m_PanelBarX, m_DiffPosY, m_PrevStatModPos, m_PrevIndex;

	std::wstring m_sCurrentSlot;
	Stats::EquipSlots m_eEquipSlots;

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

  public:
	EquipmentMenuState(Player& player, Console& console, StateMachine& stateMachine, Keyboard& keyboard);
	~EquipmentMenuState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void ProcessInputs() override;

	virtual bool Exit() override;
};
