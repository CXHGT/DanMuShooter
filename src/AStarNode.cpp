#include "AStarNode.h"
#include <cmath>
#include "fmt/format.h"
#include "CxhUtils.h"
namespace danMuGame
{
    void AStarNode_Type::setValue(const Point2 &nowPoint, const Point2 &targetPoint, int walkCost, AStarNode_Type *parent)
    {
        this->point = nowPoint;
        this->gCost = walkCost;
        this->m_parent = parent;
        this->hCost = CxhUtils::getEuclideanDistance(nowPoint, targetPoint);
    }
}