//
// Created by e.kravchenkova on 25.03.2025.
//

#pragma once
#include <vector>
#include <cstdint>


template<class Model>
class Repository
{
public:
    virtual ~Repository() = default;
    virtual std::pair<bool,uint64_t> Add(const Model& model) = 0;
    virtual bool Delete(uint64_t id) = 0;
    virtual uint64_t DeleteAll() = 0;
    virtual bool Modify(const Model& model) = 0;
    virtual std::vector<Model> getAll() = 0;
    virtual std::pair<bool,Model> getById(uint64_t id) = 0;
};