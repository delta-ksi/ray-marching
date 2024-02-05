// v0.0.1:4

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "Controlling.hpp"

#include <iostream>
#include <string.h>

using namespace sf;
using namespace std;

int main (int argc, char *argv[])
{

    // Conneting a frаgment shader
    Shader shader;
    if (!shader.isAvailable())
    {
        cout << "Error 1: Shaders are not availabel on this device!" << endl;
        return 1;
    }
    if (!shader.loadFromFile(argv[1], Shader::Fragment))
    {
        cout << "Error 2: Can't connect shader file!" << endl;
        return 2;
    }

    // Creating program window
    RenderWindow window(VideoMode(1280, 720), "Ray marching", Style::Close);
    window.setFramerateLimit(60);

    // Creating view of window
    View view;
    view.setSize(Vector2f(window.getSize()));
    view.setCenter(Vector2f(window.getSize())/2.f);

    // Setting parameters
    float psiCamera = Pi/2;
    float phiCamera = 0;
    Vector3f pCamera(0, -5, 2);
    Vector3f light(-0.5, -0.5, 2);
    float time = 0;

    // Setting uniforms before start the cycle
    shader.setUniform("screenSize", Glsl::Vec2(window.getSize()));
    shader.setUniform("psiCamera", psiCamera);
    shader.setUniform("phiCamera", phiCamera);
    shader.setUniform("pCamera", Glsl::Vec3(pCamera));
    shader.setUniform("light", Glsl::Vec3(light));
    shader.setUniform("time", time);

    Controlling controlling(shader);

    RectangleShape screen(Vector2f(window.getSize()));

    Clock clock;

    while(window.isOpen())
    {
        // Program clock
        float frametime = clock.getElapsedTime().asMicroseconds();
        float fps = 1e6 / frametime;
        time += 1/fps;
        window.setTitle(std::to_string(fps));
        clock.restart();
        
        // Event proccesing 
        Event event;
        if (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // Input processing and drawing
        if (window.hasFocus())
        {
            // Keyboard input
            controlling.keyboardInput(pCamera, psiCamera, phiCamera);
            
            // Mouse input
            if (screen.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
                controlling.mouseInput(pCamera, psiCamera, phiCamera); 

            // Updating shader time
            shader.setUniform("time", time);

            // Setting view and draw the shader
            window.setView(view);
            window.draw(screen, &shader);      
        }

        // Display all drawing stuff on sceen
        window.display();  
    }

    return 0;
}