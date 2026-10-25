#include "main.h"

#ifndef LSG_XML_H
#define LSG_XML_H

class LSG_XML
{
private:
	LSG_XML()  {}
	~LSG_XML() {}

public:
	static xmlNode*       AddChildNode(xmlNode* node, const std::string& xmlName);
	static void           Close(xmlDoc* doc);
	static std::string    GetAttribute(xmlNode* node, const std::string& attribute);
	static LSG_UMapStrStr GetAttributes(xmlNode* node);
	static LSG_XmlNodes   GetChildNodes(xmlNode* node, xmlElementType type = (xmlElementType)0);
	static xmlNode*       GetNode(const std::string& xpath);
	static LSG_XmlNodes   GetNodes(xmlDoc* doc, const std::string& xpath, xmlElementType type = (xmlElementType)0);
	static std::string    GetValue(xmlNode* node);
	static xmlDoc*        OpenFile(const std::string& filePath);
	static xmlDoc*        OpenXML(const std::string&  xmlContent);
	static void           RemoveChildNodes(xmlNode* node);
	static void           RemoveNode(xmlNode* node);
	static void           SetAttribute(xmlNode* node, const std::string& attribute, const std::string& value);
	static void           SetValue(xmlNode* node, const std::string& value);

private:
	static std::string getAttributeValue(xmlChar* content);
	static xmlNode*    getPathNode(xmlDoc* doc, const std::string& xpath);
};

#endif
