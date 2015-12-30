#pragma once

#include <unordered_map>
#include <string>

namespace io
{

class Image;
class Font;
class Sound;
class Music;

class Resources
{
public:
    // Retrieves resources (loads them from file if not found).
    Image GetImage(const std::string &filename);
    Font GetFont(const std::string &filename, int size);
    Sound GetSound(const std::string &filename);
    Music GetMusic(const std::string &filename);

    // Manually unload all resources.
    void Unload();

private:
    std::unordered_map<std::string, Image> images;
    std::unordered_map<std::string, Font> fonts;
    std::unordered_map<std::string, Sound> sounds;
    std::unordered_map<std::string, Music> music;
};

}