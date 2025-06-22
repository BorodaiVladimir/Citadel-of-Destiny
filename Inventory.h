#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "Artifact.h"

namespace MyGame {

    class Inventory {
    private:
        std::vector<Artifact> artifacts;
        const int kInventorySize = 5;

    public:
        bool AddItem(const Artifact& artifact);
        void RemoveItem(int index);
        const std::vector<Artifact>& GetArtifacts() const; // Getter for artifacts
        int GetInventorySize() const;
    };

} // namespace MyGame

#endif
