#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

class Particle 
{

public :

	Particle();
	Particle(sf::Vector2f position, float size, float maxLifeTime = -1.0f, sf::Vector2f speed = sf::Vector2f(0.0f, 0.0f),
		sf::Vector2f acceleration = sf::Vector2f(0.0f, 0.0f), float mass = 1, sf::Color color = sf::Color::White, float massOfAir = 0.9999f);

	sf::Vector2f getOrigin() const;
	void setOrigin(sf::Vector2f val);

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f val);

	sf::Vector2f getSpeed() const;
	void setSpeed(sf::Vector2f val);

	sf::Vector2f getAcceleration() const;
	void setAcceleration(sf::Vector2f val);

	float getMaxLifeTime() const;
	void setMaxLifeTime(float val);

	float getCurrentLifeTime() const;
	void setCurrentLifeTime(float val);

	int getSize() const;
	void setSize(int val);

	sf::CircleShape getShape() const;
	void setShape(sf::CircleShape val);

	sf::Color getColor() const;
	void setColor(sf::Color val);

	bool isPointOnParticle(sf::Vector2f point);
	bool isColliding(Particle &particle, float elasticity);

	float getMass() const;
	void setMass(float val);

	float getDrag() const;
	void setDrag(float val);

	sf::Vector2f getAttraction() const;
	void setAttraction(sf::Vector2f val);

	void move(float deltaTime, int width, int height, sf::Vector2f gravity);
	void bounceWindow(int width, int height, float elasticity, sf::Vector2f gravity, float deltaTime);
	void attract(Particle p, float deltaTime);

	void update(const sf::RenderWindow &window, float deltaTime, float drag, float elasticity, sf::Vector2f gravity);

private :

	sf::Vector2f m_origin;
	
	sf::Vector2f m_position;
	sf::Vector2f m_speed;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_attraction;

	float m_maxLifeTime;
	float m_currentLifeTime;
	
	float m_size;

	float m_mass;
	float m_drag;



	sf::CircleShape m_shape;

};

