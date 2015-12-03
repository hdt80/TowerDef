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

///////////////////////////////////////////////////////////////////////////////
// SkillTree
///////////////////////////////////////////////////////////////////////////////
SkillTree::SkillTree() {
	_attached = nullptr;
	_head = nullptr;
}

SkillTree::~SkillTree() {

}

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

const int SkillTree::maxDepth(SkillNode* node) {
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

SkillNode* SkillTree::addPerk(SkillNode* parent, Perk* perk) {
	SkillNode* node = new SkillNode(parent, perk);

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

void SkillTree::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape b(sf::Vector2f(target.getSize().x, target.getSize().y));
	b.setFillColor(sf::Color::Black);
	b.setOutlineColor(sf::Color(124, 124, 124));
	b.setOutlineThickness(3);
	target.draw(b);
	drawNode(_head, target, states);
}

void SkillTree::drawNode(SkillNode* node, sf::RenderTarget& target,
	sf::RenderStates states) const {

	if (node != nullptr) {
		drawNode(node->left, target, states);

		sf::RectangleShape b(sf::Vector2f(50, 50));
		b.setFillColor(sf::Color::White);
		b.setOutlineColor(sf::Color(120, 120, 120));
		b.setOutlineThickness(2);

		b.setPosition(target.getSize().x / 2, 150 * node->depth);

		target.draw(b);

		drawNode(node->right, target, states);
	}
}