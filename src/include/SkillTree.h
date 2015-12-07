#ifndef _SKILL_TREE_H
#define _SKILL_TREE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Perk.h"
#include "Vector2.h"

class Object;

// Each Node has 2 child Nodes, each child Node requiring the parent node
// to be unlocked.
class SkillNode {
public:
	SkillNode(SkillNode* parent, Perk* perk);
	~SkillNode();

	// Add a new child Node in the order of left then right
	bool add(SkillNode* node);

	// Return if the Node is to the left of the node, including parents
	bool isLeft(SkillNode* node);
	bool isLight(SkillNode* node);

	// A Node is considered unlocked if it has the max points it can hold
	bool unlocked() { return points >= maxPoints; }
	std::string name() { return perk->getName(); }

	void setPos(Vector2 newPos) { pos = newPos; }
	float getX() { return pos.X; }
	float getY() { return pos.Y; }

	Vector2 pos; // Position of the Node when drawing

	int points;    // Current points in Node
	int maxPoints; // Max points in a Node
	int depth;     // Level this Node is on

	SkillNode* nodePrereq; // Node we must have to have this perk, parent Node
	Perk* perk;

	SkillNode* left;
	SkillNode* right;
};

class SkillTree : public sf::Drawable {
public:
	SkillTree(Vector2 size = Vector2(0.0f, 0.0f));
	~SkillTree();

	void print(SkillNode* node);

	// Max depth of the tree
	const int maxDepth(const SkillNode* node);
	// Depth of the specific node
	const int depth(const SkillNode* node);
	// Number of Nodes with that depth
	const int nodesOnDepth(const SkillNode* node, int d);
	// Number of children (Including children of children)
	const int childCount(const SkillNode* node);

	// Return if that node is to the left/right of head
	bool left(SkillNode* node);
	bool right(SkillNode* node);

	// Add a new Perk with a preq. Node
	SkillNode* addPerk(SkillNode* parent, Perk* perk);

	// We've finished creating this Tree, create the drawable arrays
	void end();

	void setSize(float w, float h) { _size.X = w; _size.Y = h; }
	float getWidth() { return _size.X; }
	float getHeight() { return _size.Y; }

	const int getCount() { return _count; }
	SkillNode* getHead() { return _head; }
	std::vector<SkillNode*> data() { return _data; }

protected:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	// Methods for creating the Arrays
	void genLines();
	void genNodes();

	// Return the drawing position of a Node
	Vector2 pos(SkillNode* node);

	Object* _attached; // Object we're attached to

	SkillNode* _head;
	std::vector<SkillNode*> _data;

	int _count;
	bool _comp; // If the tree is done being created

	Vector2 _size;

	sf::VertexArray _lines;
	sf::VertexArray _nodes;
};

#endif