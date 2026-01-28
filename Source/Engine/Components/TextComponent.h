#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include <string>
#include "BaseComponent.h"

namespace Papyrus
{
    class Texture2D;

    class PAPYRUS_API TextComponent final : public BaseComponent
    {
    public:
        TextComponent(const std::string& fontTexturePath,
            int glyphWidth,
            int glyphHeight,
            std::string text = "");

        void render() const override;

        void setText(std::string text);
        const std::string& getText() const { return m_Text; }

        void setFontTexture(const std::string& fontTexturePath);
        void setGlyphSize(int width, int height);

        // Spacing is in *pixels before scaling* (it is multiplied by Transform scale at render time)
        void setSpacing(float spacingPixels) { m_Spacing = spacingPixels; }
        void setLineSpacing(float spacingPixels) { m_LineSpacing = spacingPixels; }
        void setTabSpaces(int spaces) { m_TabSpaces = (spaces < 1) ? 1 : spaces; }

        // If true: spaces just advance; if false: space glyph is drawn from atlas.
        void setSkipDrawingSpaces(bool skip) { m_SkipDrawingSpaces = skip; }

        // ASCII range control if you ever change atlas.
        void setAsciiRange(unsigned char first, unsigned char last)
        {
            m_FirstChar = first;
            m_LastChar = last;
        }

    private:
        void ensureTextureLoaded() const;

    private:
        std::string m_FontTexturePath;
        mutable Texture2D* m_FontTexture{ nullptr };

        std::string m_Text;

        int m_GlyphWidth{ 0 };
        int m_GlyphHeight{ 0 };

        float m_Spacing{ 0.0f }; 
        float m_LineSpacing{ 0.0f };
        int   m_TabSpaces{ 4 };
        bool  m_SkipDrawingSpaces{ true };

        unsigned char m_FirstChar{ 32 }; // ' '
        unsigned char m_LastChar{ 127 }; // last slot in a 32..127 sheet (96 glyphs total)
    };
}

#endif