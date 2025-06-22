#include "Artifact.h"

#include <string>

namespace MyGame {

    Artifact::Artifact(const std::string& name, const std::string& desc,
        const std::string& effect, float value, int artifact_cost)
        : artifact_name_(name),
        description_(desc),
        effect_type_(effect),
        effect_value_(value),
        cost_(artifact_cost) {}

    std::string Artifact::GetName() const { return artifact_name_; }

    std::string Artifact::GetDescription() const { return description_; }

    std::string Artifact::GetEffectType() const { return effect_type_; }

    float Artifact::GetEffectValue() const { return effect_value_; }

    int Artifact::GetCost() const { return cost_; }

}  