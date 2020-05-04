#pragma once
#include <memory>
#include <string>
#include <math.h>

namespace snake
{
	class Node
	{
	public:
		virtual ~Node() = default;

		virtual std::string getSign(unsigned short fieldWidth, unsigned short fieldHeight);

	protected:
		//checks if node is near with current
		bool isNear(Node* node, unsigned short fieldWidth, unsigned short fieldHeight);

		//checks if node is horizontal with current; the node has to be near with current
		bool isHorizontal(Node* node, unsigned short fieldWidth, unsigned short fieldHeight);

	public:
		unsigned short x = 0, y = 0;
		std::shared_ptr<Node> next;
		std::weak_ptr<Node> prev;
	};
	
	class Head : public Node
	{
	public:
		virtual std::string getSign(unsigned short fieldWidth, unsigned short fieldHeight) override;
	};

	enum class Direction : int
	{
		Left = 0,
		Up = 1,
		Right = 2,
		Down = 3
	};

	class Snake
	{
	public:
		Snake(unsigned short headX = 10, unsigned short headY = 10, unsigned short length = 3);

		void move();
		//returns true if turned
		bool turn(Direction dir);
		void grow();

		void printToBuffer(std::string* buffer, unsigned short width, unsigned short height);

		bool isHeadOnTail();

		inline std::pair<unsigned short, unsigned short> getHeadCoordinates()
		{
			return { head->x, head->y };
		}

	private:
		void moveHead();

	private:
		std::shared_ptr<Head> head;
		std::shared_ptr<Node> last;
		Direction dir;
	};
} // namespace snake