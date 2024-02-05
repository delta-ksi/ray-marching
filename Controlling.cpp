#include "Controlling.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Controlling::keyboardInput(sf::Vector3f& _camPos, float& _psiCamera, float& _phiCamera)
{
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Any))
//    {
//    }
    
	// Geting n vector of camera view
	sf::Vector3f nCamera(cos(_psiCamera)*cos(_phiCamera), sin(_psiCamera)*cos(_phiCamera), sin(_phiCamera));

	// Changing movement of camera
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) _camPos += sf::Vector3f(0.1*nCamera.x, 0.1*nCamera.y, 0.1*nCamera.z);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) _camPos -= sf::Vector3f(0.1*nCamera.x, 0.1*nCamera.y, 0.1*nCamera.z);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _camPos += sf::Vector3f(-nCamera.y*0.1, nCamera.x*0.1, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _camPos -= sf::Vector3f(-nCamera.y*0.1, nCamera.x*0.1, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) _camPos.z += 0.1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) _camPos.z -= 0.1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) exit(0);

	// Setting updated paratmeters
	m_shader.setUniform("pCamera", sf::Glsl::Vec3(_camPos));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Controlling::mouseInput(sf::Vector3f& _camPos, float& _psiCamera, float& _phiCamera)
{
    if (sf::Mouse::isButtonPressed)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2i nmp = sf::Mouse::getPosition();    // New mouse position
            sf::Vector2i dmp = nmp - m_mp;                  // Delta movement of mouse position
            m_mp = nmp;                                     // Rewrite mouse position by new one

            if (m_flag)
            {
                float dPsi = dmp.x / 5e2;   // Azimuth angle step
                float dPhi = dmp.y / 5e2;   // Zenith angle step

                // Changing azimuth angle of camera
                if (_psiCamera + dPsi < 0)
                    _psiCamera = 2*Pi;
                else if (_psiCamera + dPsi > 2*Pi)
                    _psiCamera = 0; 
                else
                    _psiCamera += dPsi;

                // Changing zenith angle of camera
                if (-Pi/2 < _phiCamera + dPhi && _phiCamera + dPhi < Pi/2)
                    _phiCamera += dPhi;
            }
            else
                m_flag = true;
        }
        else
            m_flag = false;

        // Setting updated paratmeters
        m_shader.setUniform("psiCamera", _psiCamera);
        m_shader.setUniform("phiCamera", _phiCamera);
    }
}
