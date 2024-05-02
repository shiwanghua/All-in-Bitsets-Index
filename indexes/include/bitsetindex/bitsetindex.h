#pragma once

#include <cstdint>
#include <bitset>
#include <map>
#include <vector>

#include "summator.h"
#include "util.h"

#define NB 10000000 // number of data base vectors in a instance

class BitsetIndex {
public:
    explicit BitsetIndex(int32_t _d);

    bool build(std::vector<std::vector<float>> embeddings, std::vector<std::vector<AttrNo>> attributes);

    bool insert_attribute(VecId vid, AttrNo ano);
    bool insert_attributes(VecId vid, std::vector<AttrNo> ano);    

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
    int32_t d;
    int n;
    
    std::map<AttrNo, std::bitset<NB>> attr_bitset;
};
