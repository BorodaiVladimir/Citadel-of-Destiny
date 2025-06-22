#ifndef MYGAME_ARTIFACT_H_
#define MYGAME_ARTIFACT_H_

#include <string>

namespace MyGame {

    class Artifact {
    public:
        Artifact(const std::string& name, const std::string& desc,
            const std::string& effect, float value, int artifact_cost);

        std::string GetName() const;
        std::string GetDescription() const;
        std::string GetEffectType() const;
        float GetEffectValue() const;
        int GetCost() const;

    private:
        std::string artifact_name_;
        std::string description_;
        std::string effect_type_;
        float effect_value_;
        int cost_;
    };

}  

#endif  