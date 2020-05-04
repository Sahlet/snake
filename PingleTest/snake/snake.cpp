#include "Snake.h"

namespace snake
{
	std::string Node::getSign(unsigned short fieldWidth, unsigned short fieldHeight)
	{
		auto prev = this->prev.lock();
		if (!prev)
		{
			return "?";
		}

		bool prevIsHorizontal = isHorizontal(prev.get(), fieldWidth, fieldHeight);

		if (next)
		{
			bool nextIsHorizontal = isHorizontal(next.get(), fieldWidth, fieldHeight);

			if (nextIsHorizontal == prevIsHorizontal)
			{
				return prevIsHorizontal ? "\xc4" : "\xb3";
			}
			else
			{
				Node* horNode = prev.get();
				Node* vertNode = next.get();

				if (!prevIsHorizontal)
				{
					std::swap(horNode, vertNode);
				}

				bool horIsLeft = horNode->x < this->x;
				bool vertIsUp = horNode->y < this->y;

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
			return prevIsHorizontal ? "\xc4" : "\xb3";
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

	Snake::Snake(unsigned short headX = 10, unsigned short headY = 10, unsigned short length = 3)
	{
		if (length < 3)
		{
			throw std::exception("snake length less then 3");
		}

		this->head = std::make_shared<Head>();
		this->head->x = headX;
		this->head->y = headY;

		auto prev = head;

		for (size_t i = 1; i < length; i++)
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
		std::shared_ptr<Node> node = head;
		while (node)
		{
			if (node->x < width && node->y < height)
			{
				buffer[node->y * width + node->x] = node->getSign(width, height);
			}

			node = node->next;
		}
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