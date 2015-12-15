#include "SkillTree.h"

#include "Perk.h"
#include "Object.h"
#include "Common.h"

#include "Logger.h"

#include <algorithm> // std::max
#include <cmath>	 // pow

///////////////////////////////////////////////////////////////////////////////
// SkillNode
///////////////////////////////////////////////////////////////////////////////
SkillNode::SkillNode(SkillNode* parent, Perk* perk) : 
	nodePrereq(parent), perk(perk) {

	maxPoints = perk->getMaxStacks();
	points = 0;
	depth = 0;

	left  = nullptr;
	right = nullptr;

	isLeft = false;
}

SkillNode::SkillNode() {
	nodePrereq = nullptr;
	perk = nullptr;
	maxPoints = 0;
	points = 0;
	depth = 0;

	left = nullptr;
	right = nullptr;
	isLeft = false;
}

SkillNode::~SkillNode() {

}

// Add a newly created Node as one of our children
bool SkillNode::add(SkillNode* node) {
	node->nodePrereq = this;
	node->depth = depth + 1;

	// Any child options left?
	if (left != nullptr && right != nullptr) {
		CORE_ERROR("Node[%x] has 2 children and cannot have another", this);
		return false;
	}
	// Find child node in priority, left>right
	if (left == nullptr) {
		left = node;
	} else if (right == nullptr) {
		right = node;
	} else {
		CORE_ERROR("This should never be called");
		return false;
	}
	return true;
}

// Is that point within our bounding box?
bool SkillNode::contains(float px, float py) {
	return (px >= getX() - SKILL_TREE_NODE_WIDTH &&
			px <= getX() + SKILL_TREE_NODE_WIDTH &&
			py >= getY() - SKILL_TREE_NODE_HEIGHT &&
			py <= getY() + SKILL_TREE_NODE_HEIGHT);
}

SkillNode* SkillNode::clone() {
	if (this == nullptr) {
		return nullptr;
	}
	SkillNode* node = new SkillNode();
	node->isLeft = isLeft;
	node->depth = depth;
	node->perk = perk->clone();
	node->points = 0;
	node->maxPoints = maxPoints;
	node->pos = pos;

	node->left = left->clone();
	if (node->left != nullptr) {
		node->left->nodePrereq = node;
	}
	
	node->right = right->clone();
	if (node->right != nullptr) {
		node->right->nodePrereq = node;
	}

	//tree->data().push_back(node);
	return node;
}

SkillNode* SkillNode::copy() {
	if (this == nullptr) {
		return nullptr;
	}
	SkillNode* node = new SkillNode();
	node->perk = perk->clone();
	return node;
}

void SkillNode::print() {
	if (this == nullptr) {
		return;
	}
	char rel = 'L';
		if (nodePrereq != nullptr) {
			if (nodePrereq->right == this) {
				rel = 'R';
			}
			printf("%s> (\'%s\':%x, %i%c) [%c:\'%s\':%x]\n",
				(unlocked() == true) ? "UNLOCK" : " LOCK ",
				name().c_str(), this, depth,
				(isLeft) ? 'L' : 'R', rel,
				nodePrereq->name().c_str(), nodePrereq);
		} else {
			printf("%s> (\'%s\':%x, %i) [%s]\n",
				(unlocked() == true) ? "UNLOCK" : " LOCK ",
				name().c_str(), this, depth, "Root");
		}
}

///////////////////////////////////////////////////////////////////////////////
// SkillTree
///////////////////////////////////////////////////////////////////////////////
SkillTree::SkillTree(Vector2 size) {
	_size = size;
	_attached = nullptr;
	_head = nullptr;
	_count = 0;
	_comp = false;

	_lines.setPrimitiveType(sf::Lines);
	_nodes.setPrimitiveType(sf::Quads);
}

SkillTree::~SkillTree() {

}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void SkillTree::print(SkillNode* node, bool pos) {
	if (node != nullptr) {
		print(node->left, pos);
		char rel = 'L';
		if (node->nodePrereq != nullptr) {
			if (node->nodePrereq->right == node) {
				rel = 'R';
			}
			if (pos) {
				printf("(%g, %g) ", node->getX(), node->getY());
			}
			printf("%s> (\'%s\':%x, %i%c) [%c:\'%s\':%x]\n",
				(node->unlocked() == true) ? "UNLOCK" : " LOCK ",
				node->name().c_str(), node, node->depth,
				(node->isLeft) ? 'L' : 'R', rel,
				node->nodePrereq->name().c_str(), node->nodePrereq);
		} else {
			printf("%s> (\'%s\':%x, %i) [%s]\n",
				(node->unlocked() == true) ? "UNLOCK" : " LOCK ",
				node->name().c_str(), node, node->depth, "Root");
		}
		print(node->right, pos);
	}
}

SkillTree* SkillTree::clone() {
	SkillTree* tree = new SkillTree();
	tree->setHead(_head->clone());
	tree->_lines = _lines;
	tree->_nodes = _nodes;
	tree->setComp(true);
	return tree;
}

///////////////////////////////////////////////////////////////////////////////
// Tree helper methods
///////////////////////////////////////////////////////////////////////////////
const int SkillTree::maxDepth(const SkillNode* node) {
	if (node == nullptr) {
		return 0;
	}
	return std::max(depth(node->left), depth(node->right)) + 1;
}

const int SkillTree::depth(const SkillNode* node) {
	if (node == nullptr) {
		return 0;
	}
	return depth(node->nodePrereq) + 1;
}

const int SkillTree::nodesOnDepth(const SkillNode* node, int depth) {
	if (node == nullptr) {
		return 0;
	}
	int count = nodesOnDepth(node->left, depth);
	if (node->depth == depth) {
		++count;
	}
	return count + nodesOnDepth(node->right, depth);
}

const int SkillTree::childCount(const SkillNode* node) {
	if (node == nullptr || (node->left == nullptr && node->right == nullptr)) {
		return 0;
	}
	int count = childCount(node->left);
	if (node->left != nullptr) {
		++count;
	}
	if (node->right != nullptr) {
		++count;
	}
	return count + childCount(node->right);
}

SkillNode* SkillTree::getNode(float x, float y) {
	for (unsigned int i = 0; i < _data.size(); ++i) {
		if (_data[i]->contains(x, y)) {
			return _data[i];
		}
	}
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// Tree creation methods
///////////////////////////////////////////////////////////////////////////////
SkillNode* SkillTree::addPerk(SkillNode* parent, Perk* perk) {
	SkillNode* node = new SkillNode(parent, perk);
	++_count;
	_data.push_back(node);

	// No head? Newly added Node is it
	if (_head == nullptr) {
		node->nodePrereq = nullptr;
		_head = node;
		return node;
	}
	// Dunno why this would be called
	if (parent == nullptr) {
		CORE_ERROR("Parent of new Node is nullptr!");
		return nullptr;
	}
	parent->add(node);
	// Update the Node to tell us if it's to the left or right of the head Node
	if (parent == _head) {
		if (_head->left == node) {
			node->isLeft = true;
		} else {
			node->isLeft = false;
		}
	} else {
		node->isLeft = parent->isLeft;
	}
	return node;
}

// We've finished adding Node to this tree, create the VertexArrays
void SkillTree::end() {
	if (_comp) {
		CORE_ERROR("Tried to complete an already completed tree");
		return;
	}
	// For each Node a line will be drawn from it to its parent. Lines are 
	// defined as two points, so we'll have #nodes * 2 for all lines
	_lines.resize(getCount() * 2);
	// Each Node will be drawn as a quad, a series of four points. They are
	// not connected but will be stored in one array for effieciency. Multiples
	// of 4 are the beginning of each quad
	_nodes.resize(getCount() * 4);

	// Set the position of each Node
	for (unsigned int i = 0; i < _data.size(); ++i) {
		_data[i]->setPos(pos(_data[i]));
	}

	genLines();
	genNodes();

	_comp = true;
}

Vector2 SkillTree::pos(SkillNode* node) {
	if (node == _head) {
		return Vector2(getWidth() / 2.0f, getHeight() / 2.0f);
	}
	//CORE_INFO("/ Position for \'%s\': %i (\'%s\')", node->name().c_str(), node->depth, node->nodePrereq->name().c_str());

	// Find how far each branch to the left and right goes to determine
	// where along the X axis a node's position should be drawn
	int maxDepthLeft = 0;
	int maxDepthRight = 0;
	for (unsigned int i = 0; i < _data.size(); ++i) {
		if (_data[i]->isLeft && _data[i]->depth > maxDepthLeft) {
			maxDepthLeft = _data[i]->depth;
		}
		if (!_data[i]->isLeft && _data[i]->depth > maxDepthRight) {
			maxDepthRight = _data[i]->depth;
		}
	}

	//CORE_INFO("max depth left: %i, right: %i", maxDepthLeft, maxDepthRight);

	Vector2 pos;
	pos.X = (getWidth() / 2.0f); // Start in the center
	// If it's on the left subtract the distances each node should be from
	// each other by it's depth, or how far in the tree it is
	if (node->isLeft) {
		pos.X -= pos.X / (maxDepthLeft + 1) * node->depth;
	} else {
		pos.X += pos.X / (maxDepthRight + 1) * node->depth;
	}

	// Each node is given half the amount of space from the previous one
	// so 2^node->depth gives up how much space a node is alloted
	float dist = getHeight() / (float)(pow(2, node->depth));

	//CORE_INFO("| origin.Y: %g", origin);

	// If parent node only has 1 child don't move it up or down
	// If parent is head, don't move it either cause childs go left and right
	// to head, not up and down like all other nodes
	if (node->nodePrereq->left == nullptr ||
		node->nodePrereq->right == nullptr ||
		node->nodePrereq == _head) {

		pos.Y = node->nodePrereq->pos.Y;
		//CORE_INFO("| no offset");
	} else {
		if (node->nodePrereq->left == node) {
			pos.Y = node->nodePrereq->pos.Y + dist;
			//CORE_INFO("| offset up");
		} else {
			pos.Y = node->nodePrereq->pos.Y - dist;
			//CORE_INFO("| offset down");
		}
		//CORE_INFO("| dist: %g", dist);
	}
	//CORE_INFO("\\_(%g, %g)\n", pos.X, pos.Y);

	return pos;
}


///////////////////////////////////////////////////////////////////////////////
// Drawing
///////////////////////////////////////////////////////////////////////////////
void SkillTree::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!_comp) {
		CORE_ERROR("Tried to draw an incomplete tree!");
		return;
	}

	target.draw(_lines);
	target.draw(_nodes);
}

void SkillTree::genLines() {
	sf::Vertex* line;
	SkillNode* node;
	// Draw each line from the Node to its parent
	for (unsigned int i = 0; i < _data.size(); ++i) {
		node = _data[i];
		// Parent? If not, move to next node
		if (node->nodePrereq == nullptr) {
			continue;
		}
		line = &_lines[i * 2];
		line[0].position = sf::Vector2f(node->getX(), node->getY());
		line[1].position = sf::Vector2f(node->nodePrereq->getX(),
			node->nodePrereq->getY());
	}
}

void SkillTree::genNodes() {
	sf::Vertex* quad;
	SkillNode* node;

	int nodeWidth  = 15; // Size on the box, from the center, not end to end
	int nodeHeight = 15;

	for (unsigned int i = 0; i < _data.size(); ++i) {
		node = _data[i];
		quad = &_nodes[i * 4];
		// Draw clockwise, top left, top right, bottom right, bottom left
		quad[0].position = sf::Vector2f(node->getX() - nodeWidth,
			node->getY() - nodeHeight);
		quad[1].position = sf::Vector2f(node->getX() + nodeWidth,
			node->getY() - nodeHeight);
		quad[2].position = sf::Vector2f(node->getX() + nodeWidth,
			node->getY() + nodeHeight);
		quad[3].position = sf::Vector2f(node->getX() - nodeWidth,
			node->getY() + nodeHeight);
	}
}

namespace SkillTrees {
	SkillTree* basicTree;

	void createTrees(Vector2 size) {
		CORE_INFO("Creating trees with (%g, %g)", size.X, size.Y);
		basicTree = new SkillTree(size);
		Perk* p1 = new Perk("A", Stats(), -1.0f);
		Perk* p2 = new Perk("B", Stats(), -1.0f);
		Perk* p3 = new Perk("C", Stats(), -1.0f);
		Perk* p4 = new Perk("D", Stats(), -1.0f);
		Perk* p5 = new Perk("E", Stats(), -1.0f);
		Perk* p6 = new Perk("F", Stats(), -1.0f);
		Perk* p7 = new Perk("G", Stats(), -1.0f);
		Perk* p8 = new Perk("H", Stats(), -1.0f);
		Perk* p9 = new Perk("I", Stats(), -1.0f);
		Perk* p10 = new Perk("J", Stats(), -1.0f);
		Perk* p11 = new Perk("K", Stats(), -1.0f);
		Perk* p12 = new Perk("L", Stats(), -1.0f);
		Perk* p13 = new Perk("M", Stats(), -1.0f);
		Perk* p14 = new Perk("N", Stats(), -1.0f);
		Perk* p15 = new Perk("O", Stats(), -1.0f);
		Perk* p16 = new Perk("P", Stats(), -1.0f);
		Perk* p17 = new Perk("Q", Stats(), -1.0f);

		SkillNode* n1 = new SkillNode(nullptr, p1); // Head node
		n1 = basicTree->addPerk(nullptr, p1);
		SkillNode* n2 = basicTree->addPerk(n1, p2);
		SkillNode* n3 = basicTree->addPerk(n2, p3);
		SkillNode* n4 = basicTree->addPerk(n2, p4);
		SkillNode* n5 = basicTree->addPerk(n1, p5);
		SkillNode* n6 = basicTree->addPerk(n5, p6);
		SkillNode* n7 = basicTree->addPerk(n5, p7);
		SkillNode* n8 = basicTree->addPerk(n4, p8);
		SkillNode* n9 = basicTree->addPerk(n4, p9);
		SkillNode* n10 = basicTree->addPerk(n3, p10);
		SkillNode* n11 = basicTree->addPerk(n3, p11);
		SkillNode* n12 = basicTree->addPerk(n9, p12);
		SkillNode* n13 = basicTree->addPerk(n9, p13);
		SkillNode* n14 = basicTree->addPerk(n8, p14);
		SkillNode* n15 = basicTree->addPerk(n8, p15);
		SkillNode* n16 = basicTree->addPerk(n13, p16);
		SkillNode* n17 = basicTree->addPerk(n13, p17);

		basicTree->end();
		basicTree->print(basicTree->getHead(), true);
	}
}
