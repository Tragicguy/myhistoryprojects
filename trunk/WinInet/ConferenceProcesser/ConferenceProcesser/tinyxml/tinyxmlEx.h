#ifndef TINY_XML_EX_H
#define TINY_XML_EX_H
#include "tinyxml.h"
class TinyXmlFunction
{
public:
	static void SetText(TiXmlElement* pElement, const string& text, bool bcdata = false)
	{
		return SetText(pElement, text.c_str(), bcdata);
	}
	static void SetText(TiXmlElement* pElement, const char* text, bool bcdata = false)
	{
		if (text == NULL || strlen(text) == 0)
			return;
		TiXmlText* pText = new TiXmlText(text, bcdata);
		if (!pText) return;
		pElement->LinkEndChild(pText);
	}
	
	static TiXmlElement* DocNewElement(const char* name, TiXmlDocument* pParentDocument = NULL, const char* text = NULL, bool bcdata = false)
	{
		TiXmlElement* pElement = new TiXmlElement(name);
		if (!pElement) return NULL;
		SetText(pElement, text, bcdata);
		if(pParentDocument) pParentDocument->LinkEndChild(pElement);
		return pElement;
	}

	static TiXmlElement* NewElement(const char* name, TiXmlElement* pParentElement = NULL, const char* text = NULL, bool bcdata = false)
	{
		TiXmlElement* pElement = new TiXmlElement(name);
		if (!pElement) return NULL;
		SetText(pElement, text, bcdata);
		if(pParentElement) pParentElement->LinkEndChild(pElement);
		return pElement;
	}
protected:
private:
};
#endif