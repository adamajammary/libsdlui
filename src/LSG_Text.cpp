#include "LSG_Text.h"

LSG_Text::LSG_Text(const std::string& id, int layer, xmlNode* node, const std::string& nodeName, LSG_Component* parent)
	: LSG_Component(id, layer, node, nodeName, parent)
{
	this->ellipsisTexture = nullptr;
	this->textOverflow    = LSG_TEXT_OVERFLOW_NONE;

	auto xmlAttributes = LSG_XML::GetAttributes(node);

	this->wrap = (xmlAttributes.contains("wrap") && (xmlAttributes["wrap"] == "true"));

	if (xmlAttributes.contains("text-overflow"))
		this->textOverflow = (xmlAttributes["text-overflow"] == "ellipsis" ? LSG_TEXT_OVERFLOW_ELLIPSIS : LSG_TEXT_OVERFLOW_CLIP);
}

//std::string LSG_TextLabel::getText(const std::string& text)
//{
//	if (!text.empty())
//		return text;
//
//	auto xmlText = this->GetXmlValue();
//
//	//if (xmlText.empty())
//	//	xmlText = LSG_XML::GetAttribute(this->node, "text");
//
//	//if (!xmlText.empty() && (xmlText[0] == '%')
//	//	xmlText = LSG_Window::Labels[xmlText.substr(1)];
//
//	return xmlText;
//}

SDL_Surface* LSG_Text::getSurface(const std::string& text) const
{
	return LSG_TextUtils::GetSurface(text, this->getFontSize(), this->getFontStyle(), this->textColor, this->wrap);
}

SDL_Texture* LSG_Text::getTexture(const std::string& text) const
{
	return LSG_TextUtils::GetTexture(text, this->getFontSize(), this->getFontStyle(), this->textColor, this->wrap);
}

void LSG_Text::renderTextOverflowClip(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& destination, int maxWidth) const
{
	SDL_Rect textClip = { 0, 0, maxWidth, destination.h };

	SDL_Rect textDestination = destination;

	textDestination.w = textClip.w;

	LSG_Graphics::RenderTexture(renderer, texture, &textClip, &textDestination);
}

void LSG_Text::renderTextOverflowEllipse(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& destination, int maxWidth) const
{
	auto ellipsisSize = LSG_Graphics::GetTextureSize(this->ellipsisTexture);

	SDL_Rect textClip = { 0, 0, (maxWidth - ellipsisSize.width), destination.h };

	SDL_Rect textDestination = destination;

	textDestination.w = textClip.w;

	LSG_Graphics::RenderTexture(renderer, texture, &textClip, &textDestination);

	SDL_Rect ellipsisDestination = {
		(destination.x + maxWidth - ellipsisSize.width),
		(destination.y + destination.h - ellipsisSize.height),
		ellipsisSize.width,
		ellipsisSize.height
	};

	LSG_Graphics::RenderTexture(renderer, this->ellipsisTexture, nullptr, &ellipsisDestination);
}
