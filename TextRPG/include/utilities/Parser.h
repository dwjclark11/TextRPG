#pragma once
#include <tinyxml2/tinyxml2.h>
#include <memory>
#include <string>

template <typename T>
class Parser
{
  public:
	virtual ~Parser(){};

	tinyxml2::XMLError LoadFile(const std::string& filepath) { return m_pXMLDoc->LoadFile(filepath.c_str()); }

	virtual std::shared_ptr<T> CreateObjectFromFile(const std::string& objName) = 0;
	virtual void SaveObjectToFile(T* obj) = 0;

protected:
	std::unique_ptr<tinyxml2::XMLDocument> m_pXMLDoc;
};
