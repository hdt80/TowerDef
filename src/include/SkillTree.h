#ifndef _SKILL_TREE_H
#define _SKILL_TREE_H

#include <SFML/Graphics.hpp>
#include <string>

#include "Perk.h"

class Object;

// Each Node has 2 child Nodes, each child Node requiring the parent node
// to be unlocked.
class SkillNode {
public:
	SkillNode(SkillNode* parent, Perk* perk);
	~SkillNode();

	// Add a new child Node in the order of left then right
	bool add(SkillNode* node);

	// A Node is considered unlocked if it has the max points it can hold
	bool unlocked() { return points >= maxPoints; }
	std::string name() { return perk->getName(); }

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
	SkillTree();
	~SkillTree();

	void print(SkillNode* node);

	// Max depth of the tree
	const int maxDepth(SkillNode* node);
	// Depth of the specific node
	const int depth(const SkillNode* node);

	SkillNode* addPerk(SkillNode* parent, Perk* perk);

	SkillNode* getHead() { return _head; }

protected:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	virtual void drawNode(SkillNode*, sf::RenderTarget&, sf::RenderStates) const;

	Object* _attached; // Object we're attached to

	SkillNode* _head;
};

class SkillTreeDrawable : public sf::Drawable {
public:
	SkillTreeDrawable(SkillTree* tree);
	~SkillTreeDrawable();

protected:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;


};

#endif