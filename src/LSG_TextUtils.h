#include "main.h"

#ifndef LSG_TEXT_UTILS_H
#define LSG_TEXT_UTILS_H

class LSG_TextUtils
{
private:
	LSG_TextUtils() {}
	~LSG_TextUtils() {}

private:
	static std::mutex surfaceLock;

public:
	static bool                FontSupportsText(TTF_Font* font, uint16_t* text);
	static TTF_Font*           GetFont(int size, uint16_t* text = nullptr);
	static TTF_Font*           GetFontCJK(int size);
	static std::string         GetFormattedBitRate(size_t bitRate);
	static std::string         GetFormattedCount(size_t count);
	static std::string         GetFormattedDateTime();
	static std::string         GetFormattedDuration(size_t seconds);
	static std::string         GetFormattedSize(size_t size);
	static std::string         GetFormattedTime();
	static std::string         GetFormattedTimeAgo(time_t timestamp);
	static std::string         GetFullPath(const std::string& path);
	static std::string         GetMediaCodecName(const std::string& codecName);
	static SDL_Surface*        GetSurface(const std::string& text, int fontSize, int fontStyle, const SDL_Color& textColor, bool wrap);
	static LSG_TableRowCompare GetTableRowCompare(int column);
	static SDL_Texture*        GetTexture(const std::string& text, int fontSize, int fontStyle, const SDL_Color& textColor, bool wrap);
	static std::string         Join(const LSG_Strings& strings, const std::string& separator);
	static std::string         Replace(const std::string& text, const std::string& oldSubstring, const std::string& newSubstring);
	static LSG_Strings         Split(const   std::string& text, char separator);
	static std::string         ToLower(const std::string& text);
	static std::string         ToUpper(const std::string& text);
	static std::string         ToUTF8(const std::wstring& wide);
	static uint16_t*           ToUTF16(const std::string& text);
	static std::wstring        ToWide(const  std::string& text);
	static std::string         Trim(const    std::string& text);

private:
	static std::string getFormattedTimeAgo(int64_t periods, const std::string period);
};

#endif
