#pragma once
#include "Parser.h"
#include "actor/Actor.h"

class Party;

class ActorLoader : public Parser<Actor>
{
public:
	ActorLoader(Party& party);
	~ActorLoader();

	std::shared_ptr<Actor> CreateObjectFromFile(const std::string& objName) override;
	void SaveObjectToFile(Actor* obj) override;

private:
	Party& m_Party;
};
