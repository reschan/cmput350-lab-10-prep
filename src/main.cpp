#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <chrono>

int main() {
    // OpenGL settings we request for:
    sf::ContextSettings settings;
    settings.depthBits = 24;   // 24-bit depth buffer
    settings.stencilBits = 8;  // 8-bit stencil buffer
    // Request OpenGL 2.1 for compatibility with both SFML/Graphics AND macOS:
    //  - SFML's Graphics module implementation relies on legacy fixed-function pipeline features
    //     of OpenGL, which means only up to OpenGL 2.1. Any newer version requires Compatibility
    //     profile (i.e., maintaining legacy features).
    //  - macOS has deprecated OpenGL support, meaning for anything newer than 2.1 (i.e., 3.0 and
    //      newer), only Core profile is supported, i.e., REMOVING legacy fixed-function features.
    // As a result, if you want to both support macOS AND use SFML's Graphics features (e.g.,
    // sf::RenderWindow), you can use at latest OpenGL 2.1, which is a nightmare.
    settings.majorVersion = 2;
    settings.minorVersion = 1;
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Shaders", sf::State::Windowed, settings);
    window.setFramerateLimit(60);
    // Query the settings we actually received (may be different):
    const auto& actualSettings = window.getSettings();
    std::cout << "OpenGL version " << actualSettings.majorVersion << "."
              << actualSettings.minorVersion << " flags: " << actualSettings.attributeFlags << '\n';
    // Fullscreen triangle, as described in the prep.
    sf::VertexArray fullscreenTriangle(sf::PrimitiveType::Triangles, 3);
    fullscreenTriangle[0].position = {-1, -1};  // bottom-left
    fullscreenTriangle[1].position = {3, -1};   // bottom-right
    fullscreenTriangle[2].position = {-1, 3};   // top-left
    fullscreenTriangle[0].texCoords = {0, 0};
    fullscreenTriangle[1].texCoords = {2, 0};
    fullscreenTriangle[2].texCoords = {0, 2};
    // load vertex and fragment shaders:
    sf::Shader shader(std::filesystem::path{"main.vert"}, std::filesystem::path{"main.frag"});
    shader.setUniform("resolution", (sf::Vector2f)window.getSize());
    
    // TODO: eventually add another Shader here which uses the blur fragment shader,
    // also create a sf::RenderTexture...
    sf::Shader postProcessShader;
    postProcessShader.loadFromFile("blur.frag", sf::Shader::Type::Fragment);
    postProcessShader.setUniform("resolution", (sf::Vector2f)window.getSize());

    sf::RenderTexture texture({1280, 720});

    while (window.isOpen()) {
        while (auto e = window.pollEvent()) {
            if (e->is<sf::Event::Closed>()) {
                return 0;
            }
            if (const auto* mouseMoved = e->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2f pos = (sf::Vector2f)mouseMoved->position;
                pos.y = window.getSize().y - pos.y;
                shader.setUniform("mousePos", pos);
            }
        }
        auto duration = std::chrono::system_clock::now().time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        float funny = (millis / 100) % 10;
        shader.setUniform("time", funny);
        // TODO: set shader uniforms and eventually draw into
        // a RenderTexture first before postprocessing into the window target.
        texture.clear();
        texture.draw(fullscreenTriangle, &shader);
        texture.display();

        sf::Sprite sprite(texture.getTexture());

        postProcessShader.setUniform("texture", texture.getTexture());
        window.clear(sf::Color::Black);
        window.draw(sprite, &postProcessShader);
        window.display();
    }
    return 0;
}
