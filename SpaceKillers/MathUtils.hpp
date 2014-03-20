#pragma once

#include "SFML/Graphics/Sprite.hpp"

enum class RelativeSide
	{
	Null,
	Left,
	Right,
	Up,
	Down,
	};

enum class RelativeCorner
	{
	Null,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
	};

// the return is based on the second objects relative position
// so if relativeObjectRect is on the right, then Side::Right is returned.
inline RelativeSide GetSideLR(const sf::FloatRect & baseObjectRect, const sf::FloatRect & relativeObjectRect)
	{
	if ( baseObjectRect.left + (baseObjectRect.width / 2.0f) <= relativeObjectRect.left + (relativeObjectRect.width / 2.0f) )
		{
		return RelativeSide::Right;
		}
	else
		{
		return RelativeSide::Left;
		}
	}

inline sf::Vector2f GetCenterPoint( const sf::FloatRect & object )
	{
	return sf::Vector2f( object.left + (object.width / 2.0f), object.top + (object.height / 2.0f) );
	}

inline sf::Vector2f GetHalfWidths( const sf::FloatRect & object )
	{
	return sf::Vector2f( object.width / 2.0f, object.height / 2.0f );
	}
