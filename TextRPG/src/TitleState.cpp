#include "states/GameState.h"
#include "states/TitleState.h"
#include "states/ShopState.h"
#include "states/StateMachine.h"

#include "ui/Console.h"

#include "inputs/Keys.h"
#include "inputs/Keyboard.h"

#include "actor/Actor.h"
#include "actor/Player.h"
#include "actor/Party.h"
#include "utilities/Logger.h"
#include "utilities/ActorLoader.h"
#include "utilities/Timer.h"
#include <filesystem>

namespace fs = std::filesystem;

void TitleState::DrawTitleScreen()
{
	m_Context.console.Write(25, 3, L"                                     _                                 ", YELLOW);
	m_Context.console.Write(25, 4, L"                                    (_)                                 ", YELLOW);
	m_Context.console.Write(25, 5, L"                                    |_|                                 ", BLUE);
	m_Context.console.Write(25, 6, L"                                    |_|                                 ", BLUE);
	m_Context.console.Write(25, 7, L"                                    |_|                                 ", BLUE);
	m_Context.console.Write(25, 8, L"                                    |_|                                 ", BLUE);
	m_Context.console.Write(25, 9, L"                                    |_|                                 ", BLUE);
	m_Context.console.Write(25, 10, L"                                o=========o                             ", BLUE);

	m_Context.console.Write(23, 11, L"_________ _______           _________       _______  _______  _______ ", GREEN);
	m_Context.console.Write(23, 12, L"\\__   __/(  ____ \\|\\     /| \\__   __/      (  ____ )(  ____ )(  ____ \\", GREEN);
	m_Context.console.Write(23, 13, L"   ) (   | (    \\/( \\   / )    ) (         | (    )|| (    )|| (    \\/", GREEN);
	m_Context.console.Write(23, 14, L"   | |   | (__     \\ (_) /     | |         | (____)|| (____)|| |      ", GREEN);
	m_Context.console.Write(23, 15, L"   | |   |  __)     ) _ (      | |         |     __)|  _____)| | ____ ", GREEN);
	m_Context.console.Write(23, 16, L"   | |   | (       / ( ) \\     | |         | (\\ (   | (      | | \\_  )", GREEN);
	m_Context.console.Write(23, 17, L"   | |   | (____/\\( /   \\ )    | |         | ) \\ \\__| )      | (___) |", GREEN);
	m_Context.console.Write(23, 18, L"   )_(   (_______/|/     \\|    )_(         |/   \\__/|/       (_______)", GREEN);
	m_Context.console.Write(21, 19, L"=========================================================================", BLUE);
	m_Context.console.Write(21, 20, L"=========================================================================", RED);
	m_Context.console.Write(61, 11, L"| |", GRAY);
	m_Context.console.Write(61, 12, L"| |", GRAY);
	m_Context.console.Write(61, 13, L"| |", GRAY);
	m_Context.console.Write(61, 14, L"| |", GRAY);
	m_Context.console.Write(61, 15, L"| |", GRAY);
	m_Context.console.Write(61, 16, L"| |", GRAY);
	m_Context.console.Write(61, 17, L"| |", GRAY);
	m_Context.console.Write(61, 18, L"| |", GRAY);
	m_Context.console.Write(61, 19, L"| |", GRAY);
	m_Context.console.Write(61, 20, L"| |", GRAY);
	m_Context.console.Write(61, 21, L"| |", GRAY);
	m_Context.console.Write(61, 22, L"| |", GRAY);
	m_Context.console.Write(61, 23, L"| |", GRAY);
	m_Context.console.Write(61, 24, L"| |", GRAY);
	m_Context.console.Write(61, 25, L"| |", GRAY);
	m_Context.console.Write(61, 26, L"\\ /", GRAY);
}

TitleState::TitleState(StateContext& context)
	: m_Context{ context }
	, m_Selector{ 
		context.console, context.keyboard, 
		{ L"New Game", L"Settings", L"Exit" }, SelectorParams{.x = 40, .y = 25, .columns = 1, .spacingX = 10, .spacingY = 3 } 
	}
	, m_bExitGame{ false }
	, m_bShowContinue{ false }
{
	if (fs::exists("SaveFile.xml"))
	{
		m_Selector.SetData({ L"Continue", L"New Game", L"Settings", L"Exit" });
		m_bShowContinue = true;
	}

	m_Selector.SetSelectionFunc(std::bind(&TitleState::OnMenuSelect, this, std::placeholders::_1, std::placeholders::_2));
}

TitleState::~TitleState()
{
	TRPG_LOG("DESTROY TITLE STATE");
}

void TitleState::OnEnter()
{
	TRPG_LOG("ENTER TITLE STATE");
	m_Context.console.ClearBuffer();
}

void TitleState::OnExit()
{
	TRPG_LOG("EXIT TITLE STATE");
	m_Context.console.ClearBuffer();
}

void TitleState::Update()
{

}

void TitleState::Draw()
{
	m_Selector.Draw();
	DrawTitleScreen();
	m_Context.console.Draw();

}

void TitleState::ProcessInputs()
{
	m_Selector.ProcessInputs();
}

bool TitleState::Exit()
{
	return m_bExitGame;
}

void TitleState::OnMenuSelect(int index, std::vector<std::wstring> data)
{
	int tIndex = index;
	if (!m_bShowContinue)
		tIndex = index + 1;

	switch (tIndex)
	{
	case 0:
		// TODO: Load the saved game
		TRPG_LOG("SHOULD LOAD A SAVED GAME HERE!");
		break;
	case 1:
	{
		std::unique_ptr<Party> party = std::make_unique<Party>();
		ActorLoader aLoader{ *party };
		aLoader.LoadFile("assets/xml_files/playerDefs.xml");

		std::shared_ptr<Player> player =
			std::make_shared<Player>(*static_cast<Player*>(aLoader.CreateObjectFromFile("Dustin").get()));
		std::shared_ptr<Player> mage =
			std::make_shared<Player>(*static_cast<Player*>(aLoader.CreateObjectFromFile("Donavan").get()));
		std::shared_ptr<Player> red_mage =
			std::make_shared<Player>(*static_cast<Player*>(aLoader.CreateObjectFromFile("Elijah").get()));

		party->AddMember(player);
		party->AddMember(mage);
		party->AddMember(red_mage);

		m_Context.stateMachine.PushState(std::make_unique<GameState>(m_Context));
		break;
	}
	case 2:
	{
		ActorLoader aLoader{ m_Context.party };
		aLoader.LoadFile("assets/xml_files/playerDefs.xml");

		std::shared_ptr<Player> player =
			std::make_shared<Player>(*static_cast<Player*>(aLoader.CreateObjectFromFile("Dustin").get()));
		std::shared_ptr<Player> mage =
			std::make_shared<Player>(*static_cast<Player*>(aLoader.CreateObjectFromFile("Donavan").get()));
		std::shared_ptr<Player> red_mage =
			std::make_shared<Player>(*static_cast<Player*>(aLoader.CreateObjectFromFile("Elijah").get()));

		m_Context.party.AddMember(player);
		m_Context.party.AddMember(mage);
		m_Context.party.AddMember(red_mage);

		auto oldstate = m_Context.stateMachine.PopState();
		m_Context.stateMachine.PushState(std::make_unique<ShopState>(m_Context, "assets/xml_files/ArmorShopDefs_1.xml"));

		break;
	}
	case 3:
		m_bExitGame = true;
		break;
	}
}

