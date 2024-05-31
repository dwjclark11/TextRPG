add_executable(TextRPG
    "src/Actor.cpp"
    "src/Actor.h"
	"src/Console.cpp"
    "src/Console.h"
	"src/equipment.cpp"
    "src/equipment.h"
	"src/Game.cpp"
    "src/Game.h"
	"src/Inventory.cpp"
    "src/Inventory.h"
	"src/Logger.cpp"
    "src/Logger.h"
	
    "src/Item.h"
	
	"src/Party.cpp"
    "src/Party.h"
	
	"src/Player.cpp"
    "src/Player.h"
	"src/Potion.cpp"
    "src/Potion.h"
	"src/Stats.cpp"
    "src/Stats.h"
	
	"src/Selector.h"
	
	"src/inputs/Button.cpp"
	"src/inputs/Button.h"
	"src/inputs/Keyboard.cpp"
	"src/inputs/Keyboard.h"
	"src/inputs/Keys.h"
	
	"src/states/EquipmentMenuState.h"
	"src/states/EquipmentMenuState.cpp"
	"src/states/GameMenuState.h"
	"src/states/GameMenuState.cpp"
	"src/states/GameState.h"
	"src/states/GameState.cpp"
	"src/states/IState.h"
	"src/states/ItemMenuState.h"
	"src/states/ItemMenuState.cpp"
	"src/states/ShopState.h"
	"src/states/ShopState.cpp"
	"src/states/StateMachine.h"
	"src/states/StateMachine.cpp"
	"src/states/StatusMenuState.h"
	"src/states/StatusMenuState.cpp"
	
	"src/utilities/Colors.h"
	"src/utilities/EquipmentLoader.h"
	"src/utilities/EquipmentLoader.cpp"
	"src/utilities/Globals.h"
	"src/utilities/Globals.cpp"
	"src/utilities/ItemCreator.h"
	"src/utilities/ItemLoader.h"
	"src/utilities/ItemLoader.cpp"
	"src/utilities/Parser.h"
	"src/utilities/ShopLoader.h"
	"src/utilities/ShopLoader.cpp"
	"src/utilities/ShopParameters.h"
	"src/utilities/Timer.h"
	"src/utilities/Timer.cpp"
	"src/utilities/trpg_utilities.h"
	"src/utilities/trpg_utilities.cpp"
	"src/utilities/TypeWriter.h"
	"src/utilities/TypeWriter.cpp"
	
    "src/main.cpp"
)

target_include_directories(
    TextRPG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(TextRPG PRIVATE tinyxml2)

add_resources(TARGET Assets IN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets OUT_DIR assets)
add_dependencies(TextRPG Copy-Assets)

set_target_properties(TextRPG PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/")