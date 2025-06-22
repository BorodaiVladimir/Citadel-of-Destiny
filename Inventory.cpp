#include "Inventory.h"

namespace MyGame {
    bool Inventory::AddItem(const Artifact& artifact) {
        if (artifacts.size() < kInventorySize) {
            artifacts.push_back(artifact);
            return true;
        }
        return false;
    }

    void Inventory::RemoveItem(int index) {
        if (index >= 0 && index < artifacts.size()) {
            artifacts.erase(artifacts.begin() + index);
        }
    }

    const std::vector<Artifact>& Inventory::GetArtifacts() const { return artifacts; } // Getter for artifacts
    int Inventory::GetInventorySize() const { return kInventorySize; }
}