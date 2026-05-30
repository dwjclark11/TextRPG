add_executable(
	TextRPG
    "src/Actor.cpp"
	"src/Console.cpp"
	"src/equipment.cpp"
	"src/Game.cpp"
	"src/Inventory.cpp"
	"src/Logger.cpp"
	"src/Party.cpp"	
	"src/Player.cpp"
	"src/Potion.cpp"
	"src/Stats.cpp"
	"src/Button.cpp"
	"src/Keyboard.cpp"
	
	"src/EquipmentMenuState.cpp"
	"src/GameMenuState.cpp"
	"src/GameState.cpp"
	"src/ItemMenuState.cpp"
	"src/ShopState.cpp"
	"src/StateMachine.cpp"
	"src/StatusMenuState.cpp"

	"src/EquipmentLoader.cpp"
	"src/Globals.cpp"
	"src/ActorLoader.cpp"
	"src/ItemLoader.cpp"
	"src/ShopLoader.cpp"
	"src/Timer.cpp"
	"src/Utilities.cpp"
	"src/TypeWriter.cpp"
	
    "src/main.cpp"
	"src/TitleState.cpp"
)

target_include_directories( TextRPG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_link_libraries(TextRPG PRIVATE tinyxml2)

add_resources(TARGET Assets IN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets OUT_DIR assets)

add_dependencies(TextRPG Copy-Assets)

set_target_properties(TextRPG PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/")