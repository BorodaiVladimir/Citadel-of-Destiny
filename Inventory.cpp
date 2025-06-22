#include "Inventory.h"

#include <vector>  

namespace MyGame {

    bool Inventory::AddItem(const Artifact& artifact) {
        if (artifacts_.size() < kInventorySize_) {
            artifacts_.push_back(artifact);
            return true;
        }
        return false;
    }

    void Inventory::RemoveItem(int index) {
        if (index >= 0 && index < artifacts_.size()) {
            artifacts_.erase(artifacts_.begin() + index);
        }
    }

    const std::vector<Artifact>& Inventory::GetArtifacts() const {
        return artifacts_;
    }

    int Inventory::GetInventorySize() const { return kInventorySize_; }

}  