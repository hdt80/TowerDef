#ifndef _SKILL_TREE_H
#define _SKILL_TREE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Perk.h"
#include "Vector2.h"

class Object;
class SkillTree;

// Each Node has 2 child Nodes, each child Node requiring the parent node
// to be unlocked.
class SkillNode {
public:
	SkillNode();
	SkillNode(SkillNode* parent, Perk* perk);
	~SkillNode();

	SkillNode* clone(std::vector<SkillNode*>* vec);

	// Add a new child Node in the order of left then right
	bool add(SkillNode* node);

	void print();

	// A Node is considered unlocked if it has the max points it can hold
	bool unlocked();
	std::string name() { return perk->getName(); }

	void setPos(Vector2 newPos) { pos = newPos; }
	float getX() { return pos.X; }
	float getY() { return pos.Y; }
	bool contains(float x, float y);

	Vector2 pos; // Position of the Node when drawing

	int points;    // Current points in Node
	int maxPoints; // Max points in a Node
	int depth;     // Level this Node is on
	bool isLeft;   // Is to the left of the root Node

	SkillNode* nodePrereq; // Node we must have to have this perk, parent Node
	Perk* perk;

	SkillNode* left;
	SkillNode* right;
};

class SkillTree : public sf::Drawable {
public:
	SkillTree(Vector2 size = Vector2(0.0f, 0.0f));
	~SkillTree();

	void print(SkillNode* node, bool pos = false);
	SkillTree* clone();

	// Max depth of the tree
	const int maxDepth(const SkillNode* node);
	// Depth of the specific node
	const int depth(const SkillNode* node);
	// Number of Nodes with that depth
	const int nodesOnDepth(const SkillNode* node, int d);
	// Number of children (Including children of children)
	const int childCount(const SkillNode* node);

	// Add a new Perk with a preq. Node
	SkillNode* addPerk(SkillNode* parent, Perk* perk);

	void setComp(bool b) { _comp = b; }
	void setHead(SkillNode* h) { _head = h; }

	void setAttached(Object* o) { _attached = o; }
	void setData(std::vector<SkillNode*>* v) { _data = *v; }

	// We've finished creating this Tree, create the drawable arrays
	void end();
	// Methods for creating the Arrays
	void genLines();
	void genNodes();

	void setSize(float w, float h) { _size.X = w; _size.Y = h; }
	float getWidth() { return _size.X; }
	float getHeight() { return _size.Y; }

	// Get the Node that is at the (x, y)
	SkillNode* getNode(float x, float y);
	
	const int getCount() { return _count; }
	SkillNode* getHead() { return _head; }
	std::vector<SkillNode*> data() { return _data; }

	sf::VertexArray _lines;
	sf::VertexArray _nodes;

protected:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	// Return the drawing position of a Node
	Vector2 pos(SkillNode* node);

	Object* _attached; // Object we're attached to

	SkillNode* _head;
	std::vector<SkillNode*> _data;

	int _count;
	bool _comp; // If the tree is done being created

	Vector2 _size; // Width and Height of the sf::Window it's being drawn to
};

namespace SkillTrees {
	extern void createTrees(Vector2 size = Vector2(0.0f, 0.0f));

	extern SkillTree* basicTree;
}

#endif
