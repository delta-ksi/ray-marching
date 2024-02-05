#pragma once

#ifndef _CONTROLLING_
#define _CONTROLLING_

#include <SFML/Graphics.hpp>
#include <math.h>

#define Pi 3.14

class Controlling
{
public:
    /* Class constructor */
    Controlling(sf::Shader& _shader) : m_shader(_shader) {};

    /* Class methods */ 
    // Proccesing keyboard input
    void keyboardInput(sf::Vector3f& _cam_pos, float& psiCamera, float& phiCamera);

    // Proccesing mouse input
    void mouseInput(sf::Vector3f& _cam_pos, float& psiCamera, float& phiCamera);
    
    /* Class fields */
    bool m_flag;            // Left button press flag
    sf::Vector2i m_mp;      // Mouse position
    sf::Shader& m_shader;   // Shader link
};

#endif 