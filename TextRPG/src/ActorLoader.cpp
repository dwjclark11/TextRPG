#include "utilities/ActorLoader.h"
#include "utilities/Logger.h"
#include "utilities/Utilities.h"

#include "actor/Player.h"
#include "actor/Party.h"


ActorLoader::ActorLoader(Party& party)
	: m_Party(party)
{
	m_pXMLDoc = std::make_unique<tinyxml2::XMLDocument>();
}

ActorLoader::~ActorLoader() = default;

std::shared_ptr<Actor> ActorLoader::CreateObjectFromFile(const std::string& objName)
{
	// Get the root element
	tinyxml2::XMLElement* pRootElement = m_pXMLDoc->RootElement();

	if (pRootElement)
	{
		// Get the actors Child
		tinyxml2::XMLElement* pActors = pRootElement->FirstChildElement("Actors");

		if (pActors)
		{
			tinyxml2::XMLElement* pActor = pActors->FirstChildElement("Actor");

			if (!pActor)
			{
				TRPG_ERROR("Unable to get the Actor from [");
				return nullptr;
			}

			std::shared_ptr<Actor> newActor = nullptr;

			while (pActor)
			{
				tinyxml2::XMLElement* pActorName = pActor->FirstChildElement("Name");
				if (!pActorName)
				{
					TRPG_ERROR("Unable to get the Actor Name from [");
					return nullptr;
				}

				std::string name = pActorName->GetText();

				if (name != objName)
				{
					pActor = pActor->NextSiblingElement("Actor");
					continue;
				}

				tinyxml2::XMLElement* pID = pActor->FirstChildElement("ID");

				if (!pID)
				{
					TRPG_ERROR("Unable to get the Actor ID from [");
					return nullptr;
				}

				auto id_str = pID->GetText();


				tinyxml2::XMLElement* pActorType = pActor->FirstChildElement("Type");
				if (!pActorType)
				{
					TRPG_ERROR("Unable to get the Actor Type from [");
					return nullptr;
				}

				std::string type = pActorType->GetText();

				if (type == "Player")
				{
					auto& inventory = m_Party.GetInventory();
					std::shared_ptr<Player> newPlayer = std::make_shared<Player>(
						inventory, PlayerProperties{ Utilities::CharToWide(name.c_str()), Utilities::CharToWide(id_str) }
					);

					return newPlayer;
				}
				else if (type == "Enemy")
				{


				}

				// If it gets here, return the actor
				return newActor;
			}
		}
	}
	return nullptr;
}

void ActorLoader::SaveObjectToFile(Actor* obj)
{

}