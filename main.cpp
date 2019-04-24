#include "imgui.h"
#include "imgui-SFML.h"

#include "Particle.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <random>


std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0.0f, 1.0f);
std::uniform_real_distribution<> disNeg(-1.0f, 1.0f);
std::uniform_real_distribution<> disSize(10.0f, 20.0f);
std::uniform_real_distribution<> disDensity(1.0f, 20.0f);


void checkIsMouseOnMenu(bool &isOnMenu)
{
	if (ImGui::IsMouseHoveringAnyWindow() || ImGui::IsAnyWindowFocused())
	{
		isOnMenu = true;
	}
}

std::shared_ptr<Particle> spawnParticule(sf::RenderWindow &window, sf::Vector2f gravity)
{
	sf::Vector2u windowSize = window.getSize();

	float posX = dis(gen) * (windowSize.x - 55.0f);
	float posY = dis(gen) * (windowSize.y - 55.0f);

	float speedX = disNeg(gen) * 0.0f;
	float speedY = disNeg(gen) * 0.0f;

	float size = disSize(gen);
	float density = disDensity(gen);

	// Color based on density value
	sf::Color col(200.0f - density * 10.0f, 200.0f - density * 10.0f, 255.0f);

	return std::make_shared<Particle>(sf::Vector2f(posX, posY), size, -1.0, sf::Vector2f(speedX, speedY), gravity, density * size * size, col);

}

std::shared_ptr<Particle> explosionEmission(sf::Vector2f origin, sf::Vector2f gravity)
{
	float speedX = disNeg(gen) * 1000.0f;
	float speedY = disNeg(gen) * 1000.0f;

	return std::make_shared<Particle>(origin, 5.0f, -1.0, sf::Vector2f(speedX, speedY), gravity);
}

std::shared_ptr<Particle> findParticule(std::vector<std::shared_ptr<Particle>> &particles, sf::Vector2i mousePosition)
{
	for (std::shared_ptr<Particle> particle : particles)
	{
		bool isMouseOnParticle = particle->isPointOnParticle(sf::Vector2f(mousePosition));
		if (isMouseOnParticle)
		{
			return particle;
		}
	}

	return nullptr;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Physic2D");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	float color[3];
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;

	sf::Color background = sf::Color::Black;
	bool isMouseOnMenu = false;
	bool isMenuVisible = true;

	//PARTICLES

	std::vector<std::shared_ptr<Particle>> particles;
	std::shared_ptr<Particle> selectedParticle = nullptr;

	// PHYSIC & SCENE VARIABLES

	sf::Vector2f gravity(0.0f, 0.0f);
	float elasticity = 1.0f;
	float massOfAir = 0.0;

	// TIME
	sf::Clock deltaClock;
	float elapsedTime = 0.0f;

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (selectedParticle != nullptr)
				{
					sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
					sf::Vector2f particlePosition = selectedParticle->getPosition();
					// TODO : Change this scalar and compute a better value depending on mouse movement between frames
					selectedParticle->setSpeed((mousePosition - particlePosition) * 50.0f);
					selectedParticle->setPosition(mousePosition);
				}
				else
				{
					selectedParticle = findParticule(particles, sf::Mouse::getPosition(window));
				}
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				selectedParticle = nullptr;
			}

			//////// Hide and show the menu ////////
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
			{
				isMenuVisible = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
			{
				isMenuVisible = true;
			}
			////////////////////////////////////////

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		isMouseOnMenu = false;

		if (isMenuVisible)
		{
			ImGui::Begin("Look at me!");
			if (ImGui::Button("Reset"))
			{
				particles.clear();
				for (int i = 0; i < 10; ++i)
				{
					particles.push_back(spawnParticule(window, gravity));
				}
				particles.push_back(std::make_shared<Particle>(sf::Vector2f(250, 250), 25, -1.0, sf::Vector2f(0, 0), gravity, 1000.0f * 25 * 25, sf::Color::Yellow));
			}
			checkIsMouseOnMenu(isMouseOnMenu);
			ImGui::End();
		}

		window.clear(background);

		
		for (int i = 0; i < particles.size(); i++)
		{
			if (particles[i] != selectedParticle)
			{
				particles[i]->update(window, elapsedTime, massOfAir, elasticity, gravity);

			}

			for (int j = 0; j < particles.size(); j++)
			{
				if (i != j)
				{
					particles[i]->attract(*particles[j],elapsedTime);
				}
			}


			for (int j = 0; j < particles.size(); j++)
			{
				if (i != j)
				{
					bool collide = particles[i]->isColliding(*particles[j], elasticity);
				}
			}

			window.draw(particles[i]->getShape());
		}
		

		ImGui::SFML::Render(window);
		window.display();

		// Do this at the end to take in account the rendering 
		elapsedTime = deltaClock.getElapsedTime().asMilliseconds() / 1000.0f;
	}


	ImGui::SFML::Shutdown();
}