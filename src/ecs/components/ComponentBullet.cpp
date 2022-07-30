#include "ComponentBullet.h"
#include "CxhUtils.h"
#include <iostream>
#include "fmt/format.h"

namespace danMuGame::component
{
    void ComponentBullet::bulletRun(const Vector2 &startVector, const Vector2 &endVector, float speed, float injure, entt::entity attacker, entt::entity target)
    {
        this->moveRoute.clear();
        this->endIndex = 0;
        this->startIndex = 0;
        this->injure = injure;
        this->attacker = attacker;
        this->targetEntity = target;

        float angle = CxhUtils::getAngle(startVector, endVector);
        float distance = CxhUtils::getEuclideanDistance(startVector, endVector);

        int num = distance / speed;
        this->moveRoute.push_back(startVector);

        float rx = cos(angle * DEC);
        float ry = sin(angle * DEC);

        for (int index = 1; index < num - 1; ++index)
        {
            this->moveRoute.push_back(Vector2(rx * speed * index + startVector.x(), ry * speed * index + startVector.y()));
        }
        
        this->moveRoute.push_back(endVector);
        this->isUse = true;
    }

}
