// =================================
// Includes
// =================================
#define DO_DEBUG

#include <stdlib.h>
#include <random>
#include <time.h>

#include "Window.h"
#include "Logger.h"

#include "SkillTree.h"
#include "Stats.h"
#include "Perk.h"

// =================================
// Main
// =================================
int main(int argc, char* argv[]) {
	srand(time(0));
	Logger::instance().setLogFile("log.html", false, true);

	// SkillTree s;
	// Perk* p1 = new Perk("Tree access", Stats(), -1.0f);
	// Perk* p2 = new Perk("Fast as fuck", Stats(), -1.0f);
	// Perk* p3 = new Perk("Fly as fuck", Stats(), -1.0f);
	// Perk* p4 = new Perk("Sanic", Stats(), -1.0f);
	// SkillNode* n1 = new SkillNode(nullptr, p1);
	// n1 = s.addPerk(nullptr, p1);
	// CORE_INFO("1");
	// s.print(s.getHead());
	// CORE_INFO("2");
	// s.print(s.getHead());
	// SkillNode* n2 = s.addPerk(n1, p2);
	// CORE_INFO("3");
	// s.print(s.getHead());
	// SkillNode* n3 = s.addPerk(n1, p3);
	// CORE_INFO("4");
	// s.print(s.getHead());
	// SkillNode* n4 = s.addPerk(n2, p4);
	// CORE_INFO("5");
	// s.print(s.getHead());

	// The window must be created on the main thread due to OS limitations
   	Window w("Tower", 900, 600);
   	w.start();
	return 0;
}
