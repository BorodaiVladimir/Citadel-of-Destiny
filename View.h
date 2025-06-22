#ifndef MYGAME_VIEW_H_
#define MYGAME_VIEW_H_

#include <iostream>
#include <string>
#include <vector>

#include "Unit.h"  
#include "Artifact.h"  

namespace MyGame {

	
	void PrintMessage(const std::string& message);
	void DrawHealthBar(int current_hp, int max_hp, int bar_length = 10);
	void DisplayTeam(const std::vector<Unit>& team, bool is_player);
	void DisplayUnitDetails(const Unit& unit);

}  

#endif  