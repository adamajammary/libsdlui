#include "main.h"

#ifndef LSG_TEXT_H
#define LSG_TEXT_H

class LSG_Text : public LSG_Component
{
public:
	LSG_Text(const std::string& id, int layer, xmlNode* node, const std::string& nodeName, LSG_Component* parent);
	~LSG_Text() {}

protected:
	SDL_Texture*     ellipsisTexture;
	LSG_TextOverflow textOverflow;
	bool             wrap;

protected:
	SDL_Surface* getSurface(const std::string& text) const;
	SDL_Texture* getTexture(const std::string& text) const;
	void         renderTextOverflowClip(SDL_Renderer*    renderer, SDL_Texture* texture, const SDL_Rect& destination, int maxWidth) const;
	void         renderTextOverflowEllipse(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& destination, int maxWidth) const;
};

#endif
