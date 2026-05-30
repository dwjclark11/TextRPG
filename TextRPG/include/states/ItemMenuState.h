#pragma once
#include "IState.h"
#include "ui/Selector.h"

class Player;
class Item;

class ItemState : public IState
{
  public:
	ItemState(StateContext& context, Player& player);
	~ItemState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void ProcessInputs() override;

	virtual bool Exit() override;

private:
	enum class ItemChoice
	{
		ITEM = 0,
		KEY_ITEM
	};

	enum class SelectType
	{
		DRAW,
		PROCESS_INPUTS,
		HIDE,
		SHOW
	};

	void DrawInventory();
	void DrawPlayerInfo();

	void SelectorFunc(int index, SelectType type);
	void OnMenuSelect(int index, std::vector<std::wstring> data);
	void OnItemSelect(int index, std::vector<std::shared_ptr<Item>> data);
	void RenderItem(int x, int y, std::shared_ptr<Item> item);

	void FocusOnMenu();

private:
	StateContext& m_Context;
	Player& m_Player;

	Selector<> m_MenuSelector;
	Selector<std::shared_ptr<Item>> m_ItemSelector;

	bool m_bExitGame;
	bool m_bInMenuSelect;
	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_CenterScreenW;
	int m_PanelBarX;
};
