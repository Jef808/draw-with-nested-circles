#include <SFML/Graphics.hpp>
#include "events.h"
#include "config.h"
#include "dft.h"
#include <algorithm>
#include <vector>

void drawTrail(sf::RenderWindow& window, const std::vector<sf::Vector2f>& drawing)
{
    static sf::CircleShape brush{conf::brush_size};
    static bool initialized = false;
    if (not initialized)
    {
        brush.setFillColor(conf::brush_color);
        brush.setOrigin(conf::brush_size * 0.5f, conf::brush_size * 0.5f);
        initialized = true;
    }

    for (const auto& point : drawing)
    {
        brush.setPosition(point);
        window.draw(brush);
    }
}

int main()
{
    sf::RenderWindow window{{conf::window_size.x, conf::window_size.y}, "Circle Drawing Machine" };
    window.setFramerateLimit(conf::max_frame_rate);

    std::vector<sf::CircleShape> circles;
    std::vector<sf::Vector2f> drawing;
    std::vector<std::complex<float>> points;
    std::vector<sf::Vertex> lines;

    DFT dft;
    bool isCapturing = false;
    sf::Clock clock;

    while (window.isOpen())
    {
        processEvents(window, isCapturing);
        window.clear(conf::background_color);

        if (isCapturing)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            drawing.push_back(window.mapPixelToCoords(mousePos));
            points.push_back({drawing.back().x, drawing.back().y});
            drawTrail(window, drawing);
        }
        else if (not drawing.empty())
        {
            static bool drawing_emptied = false;
            if (not drawing_emptied)
            {
                drawing.clear();
                drawing_emptied = true;
            }
            if (circles.empty())
            {
                for (int k = 0; k < points.size(); ++k)
                {
                    dft.addCoefficient(points);
                }
                std::sort(dft.coefficients.begin(), dft.coefficients.end(),
                          [](const auto& a, const auto& b) {
                              return std::abs(a.c) > std::abs(b.c);
                          });
                auto size = std::min(conf::max_frequencies, static_cast<int>(dft.coefficients.size()));
                dft.coefficients.resize(size);
                // auto size = dft.coefficients.size();

                std::transform(dft.coefficients.begin(), dft.coefficients.end(), std::back_inserter(circles),
                               [](const auto& coef) {
                                   float radius = std::abs(coef.c);
                                   sf::CircleShape circle{radius};
                                   circle.setOrigin(radius, radius);
                                   circle.setOutlineColor(sf::Color(100, 100, 100));
                                   circle.setOutlineThickness(1);
                                   circle.setFillColor(sf::Color::Transparent);
                                   return circle;
                               });
                std::fill_n(std::back_inserter(lines), circles.size() + 1, sf::Vertex{});

                clock.restart();
            }

            float total_time = points.size() * conf::dt;
            float time = clock.getElapsedTime().asSeconds();
            sf::Vector2f pos{0.0f, 0.0f};

            for (uint32_t i = 0; i < dft.coefficients.size(); ++i)
            {
                sf::CircleShape& circle = circles[i];
                float radius = circle.getRadius();
                float angle = std::arg(dft.coefficients[i].c) + time * dft.coefficients[i].k / total_time * 2.0f * M_PIf;

                circle.setPosition(pos);
                lines[i] = sf::Vertex{pos, i < 2 ? sf::Color::Transparent : sf::Color::White};

                sf::Vector2f offset{radius * std::cos(angle), radius * std::sin(angle)};
                pos += offset;
            }

            lines[circles.size()] = sf::Vertex{pos, sf::Color::White};
            drawing.push_back(pos);

            drawTrail(window, drawing);

            for (auto circle = circles.begin() + 1; circle != circles.end(); ++circle)
            {
                window.draw(*circle);
            }

            window.draw(lines.data(), lines.size(), sf::LineStrip);
        }

        window.display();
    }
}
