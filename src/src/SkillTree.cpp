#include "SkillTree.h"

#include "Perk.h"
#include "Object.h"

#include "Logger.h"

#include <algorithm> // std::max

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
}

SkillNode::~SkillNode() {

}

bool SkillNode::add(SkillNode* node) {
	node->nodePrereq = this;
	node->depth = depth + 1;

	// Any child options left?
	if (left != nullptr && right != nullptr) {
		CORE_ERROR("Node[%i] has 2 children", this);
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

// Return true if this Node is to the left of node
bool SkillNode::isLeft(SkillNode* node) {
	SkillNode* iter = nodePrereq;
	int maxIter = depth;
	int currIter = 0;
	while (currIter++ < maxIter) {
		if (iter->left == this) {
			return true;
		}
		iter = iter->nodePrereq;
	}
	return false;
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
void SkillTree::print(SkillNode* node) {
	if (node != nullptr) {
		print(node->left);
		char rel = 'L';
		if (node->nodePrereq != nullptr) {
			if (node->nodePrereq->right == node) {
				rel = 'R';
			}
			printf("%s> (\'%s\':%x, %i) [%c:\'%s\':%x]\n",
				(node->unlocked() == true) ? "UNLOCK" : " LOCK ",
				node->name().c_str(), node, node->depth, rel,
				node->nodePrereq->name().c_str(), node->nodePrereq);
		} else {
			printf("%s> (\'%s\':%x, %i) [%s]\n",
				(node->unlocked() == true) ? "UNLOCK" : " LOCK ",
				node->name().c_str(), node, node->depth, "Root");
		}
		print(node->right);
	}
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

bool SkillTree::left(SkillNode* node) {
	return node->isLeft(_head);
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
	if (parent == nullptr) {
		CORE_ERROR("Parent of new Node is nullptr!");
		return nullptr;
	}
	parent->add(node);
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
	Vector2 pos;
	pos.X = (getWidth() / 2.0f) / childCount(node) * depth(node);
	pos.Y = getHeight() / 2.0f;
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

}