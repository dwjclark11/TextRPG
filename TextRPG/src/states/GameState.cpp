#include "GameState.h"
#include "StateMachine.h"
#include "ShopState.h"
#include "../Logger.h"
#include "../Console.h"
#include "../inputs/Keyboard.h"
#include "../Potion.h"
#include "../utilities/ItemCreator.h"
#include "GameMenuState.h"
#include "../utilities/ItemLoader.h"
#include "../utilities/EquipmentLoader.h"
#include <cassert>

#include "../utilities/ShopLoader.h"

GameState::GameState(Console& console, Keyboard& keyboard, StateMachine& stateMachine)
	: m_Console(console)
	, m_Keyboard(keyboard)
	, m_StateMachine(stateMachine)
	, m_Selector(console, keyboard, {L"Start", L"Settings", L"Exit"})
	, m_Party{nullptr}
	, m_Timer{}
	, m_TypeWriter{console,
				   45,
				   15,
				   L"This is the new Typewriter\n"
				   L"The Typewriter will be used for various dialogs within the game!"
				   L"The Typewriter will be used for various dialogs within the game!"
				   L"The Typewriter will be used for various dialogs within the game!"
				   L"The Typewriter will be used for various dialogs within the game!"
				   L"The Typewriter will be used for various dialogs within the game!"
				   L"We will also use this for talking and yadda yadda?",
				   60,
				   30,
				   WHITE,
				   BLUE}
{
	m_Party = std::make_unique<Party>();

	// create an item to add to inventory test
	auto potion = ItemCreator::CreateItem(Item::ItemType::HEALTH, L"Potion", L"Heals a small bit of health", 25, 50);
	m_Party->GetInventory().AddItem(std::move(potion));

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

	m_Party->GetInventory().AddEquipment(std::move(sword));
	m_Party->GetInventory().AddEquipment(std::move(chest_armor));

	auto player = std::make_shared<Player>(L"Test Player", L"test_player", m_Party->GetInventory(), 1, 200);
	auto Dustin = std::make_shared<Player>(L"Dustin", L"warrior", m_Party->GetInventory(), 1, 200);
	auto Jonah = std::make_shared<Player>(L"Jonah", L"thief", m_Party->GetInventory(), 1, 200);
	m_Party->AddMember(std::move(player));
	m_Party->AddMember(std::move(Dustin));
	m_Party->AddMember(std::move(Jonah));
}

GameState::~GameState()
{
}

void GameState::OnEnter()
{
	m_Console.ClearBuffer();
	EquipmentLoader el{"./assets/xml_files/ArmorDefs.xml", false};
	auto equipment = el.CreateObjectFromFile("Leather Shirt");
	assert(equipment);
}

void GameState::OnExit()
{
	m_Console.ClearBuffer();
}

void GameState::Update()
{
	m_TypeWriter.UpdateText();
}

void GameState::Draw()
{

	std::wstring time_ms = L"MS: " + std::to_wstring(m_Timer.ElapsedMS());
	std::wstring time_sec = L"SEC: " + std::to_wstring(m_Timer.ElapsedSec());

	m_Console.Write(25, 25, time_ms, RED);
	m_Console.Write(25, 26, time_sec, RED);

	m_Selector.Draw();

	m_TypeWriter.Draw();

	m_Console.Draw();
}

void GameState::ProcessInputs()
{
	if (m_Keyboard.IsKeyJustPressed(KEY_ESCAPE))
	{
		m_StateMachine.PopState();
		return;
	}

	if (m_Keyboard.IsKeyJustPressed(KEY_M))
	{
		m_StateMachine.PushState(std::make_unique<GameMenuState>(*m_Party, m_Console, m_StateMachine, m_Keyboard));
		return;
	}

	if (m_Keyboard.IsKeyJustPressed(KEY_ENTER))
	{
		m_StateMachine.PushState(std::make_unique<ShopState>(
			*m_Party, m_Console, m_StateMachine, m_Keyboard, "./assets/xml_files/WeaponShopDef_1.xml"));
		return;
	}

	if (m_Keyboard.IsKeyJustPressed(KEY_ENTER))
	{
		m_Timer.Start();
	}
	else if (m_Keyboard.IsKeyJustPressed(KEY_P))
	{
		m_Timer.Pause();
	}
	else if (m_Keyboard.IsKeyJustPressed(KEY_R))
	{
		m_Timer.Resume();
	}
	else if (m_Keyboard.IsKeyJustPressed(KEY_T))
	{
		m_Timer.Stop();
		m_Console.ClearBuffer();
	}

	m_Selector.ProcessInputs();
}

bool GameState::Exit()
{
	return false;
}
