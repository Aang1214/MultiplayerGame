#include "HiddenTextNode.h"

void HiddenTextNode::SetString(const std::string& text)
{
    if (m_visible)
    {
        TextNode::SetString(text);
    }
    else
    {
        TextNode::SetString(""); // Hide text
    }
}

void HiddenTextNode::SetVisible(bool visible)
{
    m_visible = visible;
}
