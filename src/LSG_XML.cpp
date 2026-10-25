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
		if (xmlAttribute->name && xmlAttribute->children)
		{
			auto key = std::string(reinterpret_cast<const char*>(xmlAttribute->name));

			if (key == attribute)
				return LSG_XML::getAttributeValue(xmlAttribute->children->content);
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
		if (xmlAttribute->name && xmlAttribute->children)
		{
			auto key = reinterpret_cast<const char*>(xmlAttribute->name);

			attributes[key] = LSG_XML::getAttributeValue(xmlAttribute->children->content);
		}

		xmlAttribute = xmlAttribute->next;
	}

	return attributes;
}

std::string LSG_XML::getAttributeValue(xmlChar* content)
{
	if (!content)
		return "";

	auto value = std::string(reinterpret_cast<const char*>(content));

	return (!value.empty() ? LSG_TextUtils::Replace(value, "\\n", "\n") : value);
}

LSG_XmlNodes LSG_XML::GetChildNodes(xmlNode* node, xmlElementType type)
{
	if (!node)
		return {};

	LSG_XmlNodes nodes = {};

	auto xmlChildNode = node->children;

	while (xmlChildNode)
	{
		if ((type == 0) || (xmlChildNode->type == type))
			nodes.push_back(xmlChildNode);

		xmlChildNode = xmlChildNode->next;
	}

	return nodes;
}

xmlNode* LSG_XML::GetNode(const std::string& xpath)
{
	return LSG_XML::getPathNode(LSG_UI::GetXmlDocument(), xpath);
}

LSG_XmlNodes LSG_XML::GetNodes(xmlDoc* doc, const std::string& xpath, xmlElementType type)
{
	auto pathNode = LSG_XML::getPathNode(doc, xpath);

	return (pathNode ? LSG_XML::GetChildNodes(pathNode, type) : LSG_XmlNodes());
}

xmlNode* LSG_XML::getPathNode(xmlDoc* doc, const std::string& xpath)
{
	if (!doc || xpath.empty())
		return nullptr;

	auto pathContext = xmlXPathNewContext(doc);

	if (!pathContext)
		return nullptr;

	auto pathObject = xmlXPathEvalExpression(reinterpret_cast<const xmlChar*>(xpath.c_str()), pathContext);

	xmlXPathFreeContext(pathContext);

	if (!pathObject)
		return nullptr;

	auto pathNode = (pathObject->nodesetval->nodeNr > 0 ? pathObject->nodesetval->nodeTab[0] : nullptr);

	xmlXPathFreeObject(pathObject);

	return pathNode;
}

std::string LSG_XML::GetValue(xmlNode* node)
{
	auto xmlDoc = LSG_UI::GetXmlDocument();

	if (!node || !xmlDoc)
		return "";

	auto xmlValue = xmlNodeListGetString(xmlDoc, node->children, 1);
	auto value    = LSG_XML::getAttributeValue(xmlValue);

	xmlFree(xmlValue);

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
