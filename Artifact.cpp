#include "Artifact.h"
namespace MyGame {
    Artifact::Artifact(std::string name, std::string desc, std::string effect, float value, int artifact_cost)
        : artifact_name(name), description(desc), effect_type(effect), effect_value(value), cost(artifact_cost) {}

    // Getters
    std::string Artifact::GetName() const { return artifact_name; }
    std::string Artifact::GetDescription() const { return description; }
    std::string Artifact::GetEffectType() const { return effect_type; }
    float Artifact::GetEffectValue() const { return effect_value; }
    int Artifact::GetCost() const { return cost; }
}