#include "Snake.h"

namespace snake
{
	std::string Node::getSign(unsigned short fieldWidth, unsigned short fieldHeight)
	{
		auto first = this->prev.lock();
		auto second = next;
		
		if (!first)
		{
			return "?";
		}

		bool firstIsNear = isNear(first.get(), fieldWidth, fieldHeight);
		if (!firstIsNear)
		{
			if (!second)
			{
				return "?";
			}

			std::swap(first, second);

			firstIsNear = isNear(first.get(), fieldWidth, fieldHeight);
			if (!firstIsNear)
			{
				return "?";
			}
		}
		
		bool firstIsHorizontal = isHorizontal(first.get(), fieldWidth, fieldHeight);
		bool secondIsNear = second ? isNear(second.get(), fieldWidth, fieldHeight) : false;

		if (secondIsNear)
		{
			bool secondIsHorizontal = isHorizontal(second.get(), fieldWidth, fieldHeight);

			if (secondIsHorizontal == firstIsHorizontal)
			{
				return firstIsHorizontal ? "\xc4" : "\xb3";
			}
			else
			{
				Node* horNode = first.get();
				Node* vertNode = second.get();

				if (!firstIsHorizontal)
				{
					std::swap(horNode, vertNode);
				}

				bool horIsLeft = horNode->x < this->x;
				bool vertIsUp = vertNode->y < this->y;

				if (horIsLeft && vertIsUp)
				{
					return "\xd9";
				}

				if (!horIsLeft && vertIsUp)
				{
					return "\xc0";
				}

				if (horIsLeft && !vertIsUp)
				{
					return "\xbf";
				}

				if (!horIsLeft && !vertIsUp)
				{
					return "\xda";
				}
			}
		}
		else
		{
			return firstIsHorizontal ? "\xc4" : "\xb3";
		}

		return "?";
	}

	bool Node::isNear(Node* node, unsigned short /*fieldWidth*/, unsigned short /*fieldHeight*/)
	{
		if (!node)
		{
			throw std::exception("node is null");
		}

		return (std::abs((int)node->x - (int)this->x) + std::abs((int)node->y - (int)this->y)) == 1;
	}

	bool Node::isHorizontal(Node* node, unsigned short fieldWidth, unsigned short fieldHeight)
	{
		if (!isNear(node, fieldWidth, fieldHeight))
		{
			throw std::exception("node is not near");
		}

		return std::abs((int)node->x - (int)this->x) == 1;
	}

	std::string Head::getSign(unsigned short fieldWidth, unsigned short fieldHeight)
	{
		if (x >= fieldWidth || y >= fieldHeight)
		{
			return "#";
		}

		auto next = this->next;
		while (next)
		{
			if (next->x == x && next->y == y)
			{
				return "#";
			}

			next = next->next;
		}

		return "@";
	}

	Snake::Snake(unsigned short headX, unsigned short headY, unsigned short length)
	{
		if (length < 3)
		{
			throw std::exception("snake length less then 3");
		}

		this->head = std::make_shared<Head>();
		this->head->x = headX;
		this->head->y = headY;

		std::shared_ptr<Node> prev = head;

		for (unsigned short i = 1; i < length; i++)
		{
			auto node = std::make_shared<Node>();
			node->x = headX + i;
			node->y = headY;

			prev->next = node;
			node->prev = prev;
			prev = node;
		}

		this->last = prev;
		this->dir = Direction::Left;
	}

	void Snake::move()
	{
		auto prevHeadCoordinates = getHeadCoordinates();
		moveHead();

		last->x = prevHeadCoordinates.first;
		last->y = prevHeadCoordinates.second;

		auto newLast = last->prev.lock();

		last->next = head->next;
		last->prev = head;
		last->next->prev = last;

		head->next = last;

		newLast->next = nullptr;
		last = newLast;
	}

	bool Snake::turn(Direction dir)
	{
		if (((int)dir % 2) == ((int)this->dir % 2))
		{
			return false;
		}

		this->dir = dir;
		return true;
	}

	void Snake::grow()
	{
		auto prevHeadCoordinates = getHeadCoordinates();
		moveHead();

		auto node = std::make_shared<Node>();
		node->x = prevHeadCoordinates.first;
		node->y = prevHeadCoordinates.second;

		node->next = head->next;
		node->prev = head;
		
		head->next = node;

		node->next->prev = node;
	}

	void Snake::printToBuffer(std::string* buffer, unsigned short width, unsigned short height)
	{
		std::shared_ptr<Node> node = last;
		while (node)
		{
			if (node->x < width && node->y < height)
			{
				buffer[node->y * width + node->x] = node->getSign(width, height);
			}

			node = node->prev.lock();
		}
	}

	bool Snake::isHeadOnTail()
	{
		auto next = head->next;
		while (next)
		{
			if (next->x == head->x && next->y == head->y)
			{
				return true;
			}

			next = next->next;
		}

		return false;
	}

	void Snake::moveHead()
	{
		switch (dir)
		{
		case snake::Direction::Left:
			this->head->x--;
			break;
		case snake::Direction::Up:
			this->head->y--;
			break;
		case snake::Direction::Right:
			this->head->x++;
			break;
		case snake::Direction::Down:
			this->head->y++;
			break;
		default:
			break;
		}
	}
} // namespace snake