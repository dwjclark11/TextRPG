#include "GameState.h"
#include "StateMachine.h"
#include "../Logger.h"
#include "../Console.h"
#include "../inputs/Keyboard.h"
#include "../Potion.h"
#include "../utilities/ItemCreator.h"
#include "GameMenuState.h"

GameState::GameState(Console& console, Keyboard& keyboard, StateMachine& stateMachine)
	: m_Console(console)
	, m_Keyboard(keyboard)
	, m_StateMachine(stateMachine)
	, m_Selector(console, keyboard, {L"Start", L"Settings", L"Exit"})
	, m_Party{nullptr}
{
	m_Party = std::make_unique<Party>();

	// create an item to add to inventory test
	auto potion = ItemCreator::CreateItem(Item::ItemType::HEALTH, L"Potion", L"Heals a small bit of health", 25, 50);
	m_Party->GetInventory().AddItem(std::move(potion));

	auto sword = ItemCreator::CreateEquipment(Equipment::EquipType::WEAPON,
		WeaponProperties(15, WeaponProperties::WeaponType::SWORD),
		ArmorProperties(),
		StatModifier(3, StatModifier::ModifierType::STRENGTH),
		L"Short Sword", L"A small sword of shabby material", 100, 50
	);

	auto chest_armor = ItemCreator::CreateEquipment(Equipment::EquipType::ARMOR,
		WeaponProperties(),
		ArmorProperties(10, ArmorProperties::ArmorType::CHEST_BODY),
		StatModifier(3, StatModifier::ModifierType::STRENGTH),
		L"Chest Plate", L"A sturdy chest plate made of iron", 100, 50
	);


	m_Party->GetInventory().AddEquipment(std::move(sword));
	m_Party->GetInventory().AddEquipment(std::move(chest_armor));

	auto player = std::make_shared<Player>(L"Test Player", L"test_player", m_Party->GetInventory(), 1, 200);
	m_Party->AddMember(std::move(player));
}

GameState::~GameState()
{

}

void GameState::OnEnter()
{
	m_Console.ClearBuffer();
}

void GameState::OnExit()
{
	m_Console.ClearBuffer();
}

void GameState::Update()
{
	
}

void GameState::Draw()
{
	// Test to draw the player stats
	for (const auto& member : m_Party->GetParty())
	{

		const auto& name = member->GetName();
		std::wstring hp = std::to_wstring(member->GetHP());
		std::wstring max_hp = std::to_wstring(member->GetMaxHP());

		m_Console.Write(50, 30, name, BLUE);
		m_Console.Write(50, 32, L"HP: " + hp + L"/" + max_hp, BLUE);

		const auto& stats_list = member->GetStats().GetStatList();
		// Draw the player attributes
		int i = 0;
		for (const auto& [stat, value] : stats_list)
		{
			const auto& mod_value = member->GetStats().GetModifier(stat);
			m_Console.Write(50, 34 + i, stat + L":");
			m_Console.Write(70, 34 + i, std::to_wstring(value + mod_value));
			i++;
		}
	}
	

	m_Selector.Draw();
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

	m_Selector.ProcessInputs();
}

bool GameState::Exit()
{
	return false;
}
