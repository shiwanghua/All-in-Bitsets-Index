#include <cstdint>
#include <bitset>
#include <map>
#include <vector>

#include "util.h"

class AttrIndex
{
public:
    explicit AttrIndex(int32_t _d) { embedding_d = _d; }

    virtual bool build(std::vector<std::vector<float>> embeddings, std::vector<std::vector<AttrNo>> attributes) = 0;

    virtual bool insert_attribute(VecId vid, AttrNo ano) = 0;
    virtual bool insert_attributes(VecId vid, std::vector<AttrNo> ano) = 0;

    virtual bool delete_attribute(VecId vid, AttrNo ano) = 0;
    virtual bool delete_attributes(VecId vid, std::vector<AttrNo> ano) = 0;

    virtual bool add_new_attribute(AttrNo ano, std::vector<VecId> vid) = 0; // these vector whose ids are vid has these attributes (ano)

    virtual std::vector<VecId> get_ids(AttrNo ano) = 0;
    virtual std::vector<AttrNo> get_attributes(VecId vid) = 0;

    virtual std::vector<VecId> attr_search_and(std::vector<AttrNo> ano) = 0;
    virtual std::vector<VecId> attr_search_or(std::vector<AttrNo> ano) = 0;

    virtual std::vector<VecId> attr_search_andor(std::vector<std::vector<AttrNo>> ano) = 0;
    virtual std::vector<VecId> attr_search_orand(std::vector<std::vector<AttrNo>> ano) = 0;

private:
    int32_t embedding_d;
    int32_t max_attr_num;
    int n; // number of embedding in the index
};
