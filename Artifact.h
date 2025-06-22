#ifndef ARTIFACT_H
#define ARTIFACT_H

#include <string>

namespace MyGame {

    class Artifact {
    private:
        std::string artifact_name;
        std::string description;
        std::string effect_type;
        float effect_value;
        int cost;            // Стоимость артефакта

    public:
        Artifact(std::string name, std::string desc, std::string effect, float value, int artifact_cost);

        // Getters
        std::string GetName() const;
        std::string GetDescription() const;
        std::string GetEffectType() const;
        float GetEffectValue() const;
        int GetCost() const;
    };

} // namespace MyGame

#endif
