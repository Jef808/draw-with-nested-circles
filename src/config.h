#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cstdint>

namespace conf
{
    // Window configuration
    const sf::Vector2u window_size{ 1920, 1080 };
    const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
    const float max_frame_rate = 144;
    const float dt = 1.0f / max_frame_rate;

    // Background configuration
    const sf::Color background_color{40, 40, 40};

    // Disks configuration
    const float scale = 0.2f;
    const int max_frequencies = 2500;

    // Brush configuration
    const sf::Color brush_color{205, 83, 4};
    const float brush_size = 5.0f;
}
