// TextComponent.cpp
#include "TextComponent.h"

#include <SDL3/SDL.h>

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

namespace Papyrus
{
    TextComponent::TextComponent(const std::string& fontTexturePath,
        int glyphWidth,
        int glyphHeight,
        std::string text)
        : m_FontTexturePath(fontTexturePath)
        , m_FontTexture(nullptr)
        , m_Text(std::move(text))
        , m_GlyphWidth(glyphWidth)
        , m_GlyphHeight(glyphHeight)
    {
        m_FontTexture = ResourceManager::getInstance().loadTexture(m_FontTexturePath);
    }

    void TextComponent::ensureTextureLoaded() const
    {
        if (!m_FontTexture && !m_FontTexturePath.empty())
        {
            m_FontTexture = ResourceManager::getInstance().loadTexture(m_FontTexturePath);
        }
    }

    void TextComponent::render() const
    {
        ensureTextureLoaded();

        if (!m_FontTexture || m_Text.empty() || m_GlyphWidth <= 0 || m_GlyphHeight <= 0)
            return;

        const int texW = m_FontTexture->width();
        const int texH = m_FontTexture->height();

        const int columns = texW / m_GlyphWidth;
        const int rows = texH / m_GlyphHeight;

        if (columns <= 0 || rows <= 0)
            return;

        const auto* owner = getOwner();
        if (!owner)
            return;

        const auto& tr = owner->m_Transform;

        // World is rotated +90°.
        // To keep text upright in screen-space, rotate glyph quads by -90° relative to the world.
        constexpr float kTextRotationOffsetDeg = -90.0f;

        const float scaledGlyphW = float(m_GlyphWidth) * tr.scale.x;
        const float scaledGlyphH = float(m_GlyphHeight) * tr.scale.y;

        const float scaledSpacing = m_Spacing * tr.scale.x;
        const float scaledLineSpacing = m_LineSpacing * tr.scale.y;

        const float advancePrimary = scaledGlyphW + scaledSpacing;   // one character advance (text-space X)
        const float advanceLine = scaledGlyphH + scaledLineSpacing; // one line advance (text-space Y)

        float cursorX = 0.0f;
        float cursorY = 0.0f;

        for (unsigned char c : m_Text)
        {
            if (c == '\n')
            {
                // New line: reset horizontal and move down one line.
                // With your current coordinate mapping (origin behaving like top-right),
                // the correct "down one line" direction ends up being +X.
                cursorY = 0.0f;
                cursorX += advanceLine;
                continue;
            }

            if (c == '\t')
            {
                // Tab: advance in text-space X.
                // With your current mapping, that corresponds to -Y in world.
                cursorY -= advancePrimary * float(m_TabSpaces);
                continue;
            }

            if (c < m_FirstChar || c > m_LastChar)
                continue;

            // Optional: don’t draw spaces, only advance
            if (m_SkipDrawingSpaces && c == ' ')
            {
                cursorY -= advancePrimary;
                continue;
            }

            const int idx = int(c - m_FirstChar); // ASCII 32..127 atlas
            const int col = idx % columns;
            const int row = idx / columns;

            if (row >= rows)
                continue;

            const SDL_FRect src{
                float(col * m_GlyphWidth),
                float(row * m_GlyphHeight),
                float(m_GlyphWidth),
                float(m_GlyphHeight)
            };

            const float drawX = tr.position.x + cursorX;
            const float drawY = tr.position.y + cursorY;

            const float drawRot = tr.rotation + kTextRotationOffsetDeg;

            // Swap w/h because the quad is rotated by -90° in world space.
            Renderer::getInstance().renderTexture(
                *m_FontTexture,
                src,
                drawX,
                drawY,
                scaledGlyphH, // swapped
                scaledGlyphW, // swapped
                drawRot
            );

            // Advance one character: text-space +X => world -Y (for your current mapping)
            cursorY -= advancePrimary;
        }
    }

    void TextComponent::setText(std::string text)
    {
        m_Text = std::move(text);
    }

    void TextComponent::setFontTexture(const std::string& fontTexturePath)
    {
        m_FontTexturePath = fontTexturePath;
        m_FontTexture = ResourceManager::getInstance().loadTexture(m_FontTexturePath);
    }

    void TextComponent::setGlyphSize(int width, int height)
    {
        m_GlyphWidth = width;
        m_GlyphHeight = height;
    }
}
