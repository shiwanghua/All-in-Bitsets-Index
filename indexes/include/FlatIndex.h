#pragma once

#include <map>
#include <vector>

#include "util.h"
#include "AttrIndex.h"
class FlatIndex : public AttrIndex
{
public:
    explicit FlatIndex(int32_t _d, int32_t max_attr_num_); // 每个向量最大含有的属性值个数

    bool build(std::vector<std::vector<float>> embeddings, std::vector<std::vector<AttrNo>> attributes);

    bool insert_attribute(VecId vid, AttrNo ano);
    void insert_attributes(VecId vid, std::vector<AttrNo> ano);

    bool delete_attribute(VecId vid, AttrNo ano);
    bool delete_attributes(VecId vid, std::vector<AttrNo> ano);

    bool add_new_attribute(AttrNo ano, std::vector<VecId> vid);

    std::vector<VecId> get_ids(AttrNo ano);
    std::vector<AttrNo> get_attributes(VecId vid);

    std::vector<VecId> attr_search_and(std::vector<AttrNo> ano);
    std::vector<VecId> attr_search_or(std::vector<AttrNo> ano);

    std::vector<VecId> attr_search_andor(std::vector<std::vector<AttrNo>> ano);
    std::vector<VecId> attr_search_orand(std::vector<std::vector<AttrNo>> ano);

private:
    int32_t total_d;
    std::vector<std::vector<float>> raw_data;
};
