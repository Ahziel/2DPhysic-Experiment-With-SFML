#include "Particle.h"

#include <iostream>


float dot(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return vec1.x * vec2.x + vec1.y + vec2.y;
}

float squareLength(sf::Vector2f vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

float length(sf::Vector2f vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f normalise(sf::Vector2f &vec)
{
	float vecLength = length(vec);
	return sf::Vector2f(vec.x / vecLength, vec.y / vecLength);
}

sf::Vector2f reflect(sf::Vector2f vec, sf::Vector2f normal)
{
	sf::Vector2f reflect = vec - 2.0f * (normal.x * vec.x + normal.y * vec.y) * normal;
	return reflect;
}


Particle::Particle() : m_position(0.0f, 0.0f), m_speed(0.0f, 0.0f), m_acceleration(0.0f, 0.0f), 
						 m_maxLifeTime(0.0f), m_size(0.0f)
{

}

Particle::Particle(sf::Vector2f position, float size, float maxLifeTime /*= -1.0f*/, sf::Vector2f speed /*= sf::Vector2f(0.0f, 0.0f)*/, 
	sf::Vector2f acceleration /*= sf::Vector2f(0.0f, 0.0f)*/, float mass/* = 1*/, sf::Color color /*= sf::Color::White*/, float massOfAir /*= 0.9999f*/)
{
	m_position = position;
	m_speed = speed;
	m_acceleration = acceleration;
	m_maxLifeTime = maxLifeTime;
	m_size = size;
	m_mass = mass;
	m_drag = std::pow((m_mass / (m_mass + massOfAir)), size);

	m_shape.setOrigin(m_shape.getOrigin() + sf::Vector2f(m_size,m_size));
	m_shape.setPosition(m_position);
	m_shape.setRadius(m_size);

	m_shape.setFillColor(color);
	
}

sf::Vector2f Particle::getOrigin() const
{ 
	return m_origin; 
}

void Particle::setOrigin(sf::Vector2f val)
{
	m_origin = val; 
}

sf::Vector2f Particle::getPosition() const
{
	return m_position;
}

void Particle::setPosition(sf::Vector2f val)
{
	m_position = val;
	m_shape.setPosition(m_position);
}

sf::Vector2f Particle::getSpeed() const
{
	return m_speed;
}

void  Particle::setSpeed(sf::Vector2f val) 
{ 
	m_speed = val; 
}

sf::Vector2f  Particle::getAcceleration() const 
{ 
	return m_acceleration; 
}

void  Particle::setAcceleration(sf::Vector2f val) 
{ 
	m_acceleration = val; 
}

float Particle::getMaxLifeTime() const 
{ 
	return m_maxLifeTime; 
}

void Particle::setMaxLifeTime(float val) 
{ 
	m_maxLifeTime = val; 
}

float Particle::getCurrentLifeTime() const 
{ 
	return m_currentLifeTime; 
}

void Particle::setCurrentLifeTime(float val) 
{ 
	m_currentLifeTime = val; 
}

int Particle::getSize() const
{
	return m_size;
}

void Particle::setSize(int val)
{
	m_size = val;
	m_shape.setRadius(m_size);
}

sf::CircleShape Particle::getShape() const
{
	return m_shape;
}

void Particle::setShape(sf::CircleShape val)
{
	m_shape = val;
}

sf::Color Particle::getColor() const 
{ 
	return m_shape.getFillColor(); 
}

void Particle::setColor(sf::Color val) 
{
	m_shape.setFillColor(val);
}

float Particle::getMass() const
{ 
	return m_mass; 
}

void Particle::setMass(float val)
{ 
	m_mass = val;
}

float Particle::getDrag() const
{ 
	return m_drag;
}

void Particle::setDrag(float val)
{ 
	m_drag = val; 
}

sf::Vector2f Particle::getAttraction() const
{ 
	return m_attraction; 
}

void Particle::setAttraction(sf::Vector2f val)
{
	m_attraction = val;
}

bool Particle::isPointOnParticle(sf::Vector2f point)
{
	sf::Vector2f difference = m_position - sf::Vector2f(point);
	float distance = length(difference);

	if (distance < m_size)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Particle::isColliding(Particle &particle, float elasticity)
{
	sf::Vector2f difference = m_position - particle.getPosition();

	float distance = (m_size + particle.getSize()) - length(difference);

	if (distance > 0.1f)
	{
		difference = normalise(difference);

		m_position += difference * distance * 0.5f;
		particle.setPosition(particle.getPosition() - difference * distance * 0.5f);

		float totalMass = m_mass + particle.getMass();

		sf::Vector2f temp = m_speed;

		m_speed = (m_mass - particle.getMass()) * m_speed / totalMass + (2.0f * particle.getMass()) * particle.getSpeed() / totalMass;
		particle.setSpeed((2.0f * m_mass) * temp / totalMass + ( particle.getMass() - m_mass) * particle.getSpeed() / totalMass);

		m_speed = m_speed * elasticity;
		particle.setSpeed(particle.getSpeed() * elasticity);

		return true;
	}
	else
	{
		return false;
	}
}


void Particle::move(float deltaTime, int width, int height, sf::Vector2f gravity)
{
	sf::Vector2f temp = m_speed;
	m_speed = m_speed + m_acceleration * deltaTime;
	m_speed *= m_drag;
	m_position = m_position + m_speed * deltaTime;

	if (m_position.x <= m_size)
	{
		m_position.x = m_size ;
	}
	else if (m_position.x >= width - m_size)
	{
		m_position.x = width - m_size;
	}
	if (m_position.y <= m_size)
	{
		m_position.y = m_size;
	}
	else if (m_position.y >= height - m_size)
	{
		m_position.y = height - m_size;
	}

	m_shape.setPosition(m_position);
	m_currentLifeTime = m_currentLifeTime + deltaTime;
}

void Particle::bounceWindow(int width, int height, float elasticity, sf::Vector2f gravity, float deltaTime)
{
	if (m_position.x <= m_size)
	{
		sf::Vector2f normal(1.0f, 0.0f);
		if ((normal.x * m_speed.x + normal.y * m_speed.y) < 0.0f)
		{
				sf::Vector2f reflect = m_speed - 2.0f * (normal.x * m_speed.x + normal.y * m_speed.y) * normal;
				m_speed = reflect * elasticity;
		}
	}
	else if (m_position.x >= width - m_size)
	{
		sf::Vector2f normal(-1.0f, 0.0f);
		if ((normal.x * m_speed.x + normal.y * m_speed.y) < 0.0f)
		{
				sf::Vector2f reflect = m_speed - 2.0f * (normal.x * m_speed.x + normal.y * m_speed.y) * normal;
				m_speed = reflect * elasticity;
		}
	}
	if (m_position.y <= m_size)
	{
		sf::Vector2f normal(0.0f, 1.0f);
		if ((normal.x * m_speed.x + normal.y * m_speed.y) < 0.0f)
		{
				sf::Vector2f reflect = m_speed - 2.0f * (normal.x * m_speed.x + normal.y * m_speed.y) * normal;
				m_speed = reflect * elasticity;
		}
	}
	else if (m_position.y >= height - m_size )
	{
		sf::Vector2f normal(0.0f, -1.0f);
		if ((normal.x * m_speed.x + normal.y * m_speed.y) < 0.0f)
		{
				sf::Vector2f reflect = m_speed - 2.0f * (normal.x * m_speed.x + normal.y * m_speed.y) * normal;
				m_speed = reflect * elasticity;
		}
	}
}

void Particle::attract(Particle p , float deltaTime)
{
	sf::Vector2f difference = m_position - p.getPosition();
	float distance = length(difference);

	if (distance > 25.0f)
	{
		float force = 0.2f * (m_mass * p.getMass()) / (distance * distance);

		setAcceleration(m_attraction - (difference * 0.5f * force / m_mass));
		p.setAcceleration(p.getAttraction() + (difference * 0.5f * force / p.getMass()));
	}
	

}

void Particle::update(const sf::RenderWindow &window, float deltaTime, float drag, float elasticity, sf::Vector2f gravity)
{
	move(deltaTime, window.getSize().x, window.getSize().y, gravity);
	bounceWindow(window.getSize().x, window.getSize().y, elasticity, gravity, deltaTime);
}

