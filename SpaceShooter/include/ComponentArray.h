//
//  ComponentArray.h
//  SpaceShooter
//
//  Created by Kai Wang on 2022/11/30.
//

#ifndef ComponentArray_h
#define ComponentArray_h

#include "Entity.h"
#include <array>
#include <cassert>
#include <unordered_map>

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray
{
public:
    // for a given component type, add itself to entity
    void InsertData(Entity entity, T component)
    {
        assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() &&  "Component added to same entity more than once.");
        
        size_t newIndex = mSize;
        mEntityToIndexMap[entity] = newIndex;
        mIndexToEntityMap[newIndex] = entity;
        mComponentArray[newIndex] = component;
        ++mSize;
    }
    
    void RemoveData(Entity entity)
    {
        assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");
        
        // Copy element at end into deleted element's place to maintain density
        size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
        size_t indexOfLastElement = mSize - 1;
        mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];
        
        // Update map to point to moved spot
        Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
        mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;
        
        mEntityToIndexMap.erase(entity);
        mIndexToEntityMap.erase(indexOfLastElement);
        
        --mSize;
    }
    
    // get self data, see how many entity has registerred self.
    T& GetData(Entity entity)
    {
        assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");
        
        return mComponentArray[mEntityToIndexMap[entity]];
    }
    
    // remove component
    void EntityDestroyed(Entity entity) override
    {
        if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
        {
            // Remove the entity's component if it existed
            RemoveData(entity);
        }
    }
    
    
private:
    std::array<T, MAX_ENTITIES> mComponentArray{};
    std::unordered_map<Entity, size_t> mEntityToIndexMap{};
    std::unordered_map<size_t, Entity> mIndexToEntityMap{};
    size_t mSize{};
};
#endif /* ComponentArray_h */
