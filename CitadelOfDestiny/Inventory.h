#ifndef MYGAME_INVENTORY_H_
#define MYGAME_INVENTORY_H_

#include <vector>
#include "Artifact.h"  // Необходимо для Artifact

namespace MyGame {

	class Inventory {
	public:
		bool AddItem(const Artifact& artifact);
		void RemoveItem(int index);
		const std::vector<Artifact>& GetArtifacts() const;
		int GetInventorySize() const;

	private:
		std::vector<Artifact> artifacts_;
		const int kInventorySize_ = 5;  // Член класса константа
	};

}  // namespace MyGame

#endif  // MYGAME_INVENTORY_H_
