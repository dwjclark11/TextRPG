#include "states/GameState.h"
#include "states/StateMachine.h"
#include "states/ShopState.h"
#include "states/GameMenuState.h"

#include "ui/Console.h"
#include "inputs/Keyboard.h"

#include "utilities/Logger.h"
#include "utilities/ItemCreator.h"
#include "utilities/ShopLoader.h"
#include "utilities/ItemLoader.h"
#include "utilities/EquipmentLoader.h"

#include "actor/Player.h"
#include "actor/Party.h"

#include "items/Inventory.h"
#include "items/Potion.h"

#include <cassert>

GameState::GameState(StateContext& context)
	: m_Context{ context }
	, m_Selector(context.console, context.keyboard, {L"Start", L"Settings", L"Exit"})
	, m_Timer{}
{
	// create an item to add to inventory test
	auto potion = ItemCreator::CreateItem(Item::ItemType::HEALTH, L"Potion", L"Heals a small bit of health", 25, 50);
	m_Context.party.GetInventory().AddItem(std::move(potion));

	auto sword = ItemCreator::CreateEquipment(Equipment::EquipType::WEAPON,
											  WeaponProperties(15, WeaponProperties::WeaponType::SWORD),
											  ArmorProperties(),
											  StatModifier(3, StatModifier::ModifierType::STRENGTH),
											  L"Short Sword",
											  L"A small sword of shabby material",
											  10);

	auto chest_armor = ItemCreator::CreateEquipment(Equipment::EquipType::ARMOR,
													WeaponProperties(),
													ArmorProperties(10, ArmorProperties::ArmorType::CHEST_BODY),
													StatModifier(3, StatModifier::ModifierType::STRENGTH),
													L"Chest Plate",
													L"A sturdy chest plate made of iron",
													100);

	m_Context.party.GetInventory().AddEquipment(std::move(sword));
	m_Context.party.GetInventory().AddEquipment(std::move(chest_armor));

	auto player = std::make_shared<Player>(m_Context.party.GetInventory(), PlayerProperties{ L"Test Player", L"test_player", 1, 200 } );
	auto Dustin = std::make_shared<Player>(m_Context.party.GetInventory(), PlayerProperties{ L"Dustin", L"warrior", 1, 200 });
	auto Jonah = std::make_shared<Player>(m_Context.party.GetInventory(), PlayerProperties{ L"Jonah", L"thief", 1, 200 });

	m_Context.party.AddMember(std::move(player));
	m_Context.party.AddMember(std::move(Dustin));
	m_Context.party.AddMember(std::move(Jonah));
}

GameState::~GameState()
{
}

void GameState::OnEnter()
{
	m_Context.console.ClearBuffer();
	EquipmentLoader el{"./assets/xml_files/ArmorDefs.xml", false};
	auto equipment = el.CreateObjectFromFile("Leather Shirt");
	assert(equipment);
}

void GameState::OnExit()
{
	m_Context.console.ClearBuffer();
}

void GameState::Update()
{
	
}

void GameState::Draw()
{

	std::wstring time_ms = L"MS: " + std::to_wstring(m_Timer.ElapsedMS());
	std::wstring time_sec = L"SEC: " + std::to_wstring(m_Timer.ElapsedSec());

	m_Context.console.Write(25, 25, time_ms, RED);
	m_Context.console.Write(25, 26, time_sec, RED);

	m_Selector.Draw();

	m_Context.console.Draw();
}

void GameState::ProcessInputs()
{
	if (m_Context.keyboard.IsKeyJustPressed(KEY_ESCAPE))
	{
		m_Context.stateMachine.PopState();
		return;
	}

	if (m_Context.keyboard.IsKeyJustPressed(KEY_M))
	{
		m_Context.stateMachine.PushState(std::make_unique<GameMenuState>( m_Context ) );
		return;
	}

	if (m_Context.keyboard.IsKeyJustPressed(KEY_ENTER))
	{
		m_Context.stateMachine.PushState(
			std::make_unique<ShopState>(m_Context, "./assets/xml_files/WeaponShopDef_1.xml")
		);

		return;
	}

	if (m_Context.keyboard.IsKeyJustPressed(KEY_ENTER))
	{
		m_Timer.Start();
	}
	else if (m_Context.keyboard.IsKeyJustPressed(KEY_P))
	{
		m_Timer.Pause();
	}
	else if (m_Context.keyboard.IsKeyJustPressed(KEY_R))
	{
		m_Timer.Resume();
	}
	else if (m_Context.keyboard.IsKeyJustPressed(KEY_T))
	{
		m_Timer.Stop();
		m_Context.console.ClearBuffer();
	}

	m_Selector.ProcessInputs();
}

bool GameState::Exit()
{
	return false;
}
