#pragma once

class Texture
{
public:
    Texture(const char* texturePath);

    void Bind(unsigned int slot);

private:
    unsigned int m_id;
};