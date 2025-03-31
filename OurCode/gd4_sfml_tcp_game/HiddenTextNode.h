#pragma once
#include "TextNode.hpp"

class HiddenTextNode : public TextNode
{
public:
    HiddenTextNode(const FontHolder& fonts, std::string& text)
        : TextNode(fonts, text), m_visible(true) {}

    void SetString(const std::string& text) override;

    void SetVisible(bool visible);

private:
    bool m_visible;
};

