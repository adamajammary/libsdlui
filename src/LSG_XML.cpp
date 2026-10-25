#include "LSG_XML.h"

xmlNode* LSG_XML::AddChildNode(xmlNode* node, const std::string& xmlName)
{
	if (!node || xmlName.empty())
		return nullptr;

	auto xmlChildName = reinterpret_cast<const xmlChar*>(xmlName.c_str());
	auto xmlChildNode = xmlNewNode(nullptr, xmlChildName);

	if (xmlChildNode)
		xmlAddChild(node, xmlChildNode);

	return xmlChildNode;
}

void LSG_XML::Close(xmlDoc* doc)
{
	xmlFreeDoc(doc);
}

std::string LSG_XML::GetAttribute(xmlNode* node, const std::string& attribute)
{
	if (!node || attribute.empty())
		return "";

	auto xmlAttribute = node->properties;

	while (xmlAttribute)
	{
		if (xmlStrcmp(xmlAttribute->name, reinterpret_cast<const xmlChar*>(attribute.c_str())) == 0)
		{
			auto value = std::string(reinterpret_cast<const char*>(xmlAttribute->xmlChildrenNode->content));

			if (!value.empty())
				value = LSG_TextUtils::Replace(value, "\\n", "\n");

			return value;
		}

		xmlAttribute = xmlAttribute->next;
	}

	return "";
}

LSG_UMapStrStr LSG_XML::GetAttributes(xmlNode* node)
{
	if (!node)
		return {};

	LSG_UMapStrStr attributes;

	auto xmlAttribute = node->properties;

	while (xmlAttribute)
	{
		auto key   = std::string(reinterpret_cast<const char*>(xmlAttribute->name));
		auto value = std::string(reinterpret_cast<const char*>(xmlAttribute->xmlChildrenNode->content));

		if (!value.empty())
			value = LSG_TextUtils::Replace(value, "\\n", "\n");

		attributes[key] = value;

		xmlAttribute = xmlAttribute->next;
	}

	return attributes;
}

LSG_XmlNodes LSG_XML::GetChildNodes(xmlNode* node)
{
	if (!node)
		return {};

	LSG_XmlNodes nodes = {};

	auto xmlChildNode = node->xmlChildrenNode;

	while (xmlChildNode)
	{
		nodes.push_back(xmlChildNode);

		xmlChildNode = xmlChildNode->next;
	}

	return nodes;
}

xmlNode* LSG_XML::GetNode(const std::string& xpath)
{
	auto xmlDoc = LSG_UI::GetXmlDocument();

	if (xpath.empty() || !xmlDoc)
		return nullptr;

	auto pathContext = xmlXPathNewContext(xmlDoc);

	if (!pathContext)
		return nullptr;

	auto pathObject = xmlXPathEvalExpression(reinterpret_cast<const xmlChar*>(xpath.c_str()), pathContext);

	xmlXPathFreeContext(pathContext);

	if (!pathObject)
		return nullptr;

	xmlNode* result = nullptr;

	if (pathObject->nodesetval->nodeNr > 0)
		result = pathObject->nodesetval->nodeTab[0];

	xmlXPathFreeObject(pathObject);

	return result;
}

std::string LSG_XML::GetValue(xmlNode* node)
{
	auto xmlDoc = LSG_UI::GetXmlDocument();

	if (!node || !xmlDoc)
		return "";

	auto xmlValue = xmlNodeListGetString(xmlDoc, node->children, 1);
	auto value    = (xmlValue ? std::string(reinterpret_cast<char*>(xmlValue)) : "");

	xmlFree(xmlValue);

	if (!value.empty())
		value = LSG_TextUtils::Replace(value, "\\n", "\n");

	return value;
}

xmlDoc* LSG_XML::OpenFile(const std::string& filePath)
{
	return xmlReadFile(filePath.c_str(), nullptr, 0);
}

xmlDoc* LSG_XML::OpenXML(const std::string& xmlContent)
{
	return xmlReadMemory(xmlContent.c_str(), xmlContent.size(), nullptr, nullptr, 0);
}

void LSG_XML::RemoveChildNodes(xmlNode* node)
{
	if (!node)
		return;

	auto childNode = node->children;

	while (childNode) {
		LSG_XML::RemoveNode(childNode);
		childNode = childNode->next;
	}
}

void LSG_XML::RemoveNode(xmlNode* node)
{
	if (!node)
		return;

	xmlUnlinkNode(node);
	xmlFreeNode(node);
}

void LSG_XML::SetAttribute(xmlNode* node, const std::string& attribute, const std::string& value)
{
	if (!node || attribute.empty())
		return;

	auto xmlAttribute = reinterpret_cast<const xmlChar*>(attribute.c_str());
	auto xmlValue     = reinterpret_cast<const xmlChar*>(value.c_str());

	xmlSetProp(node, xmlAttribute, xmlValue);
}

void LSG_XML::SetValue(xmlNode* node, const std::string& value)
{
	if (!node)
		return;

	auto xmlValue = reinterpret_cast<const xmlChar*>(value.c_str());

	xmlNodeSetContent(node, xmlValue);
}
