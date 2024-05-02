#include <BitsetIndex.h>

#include<unordered_set>

BitsetIndex::BitsetIndex(int32_t _d) : d(_d) {}

void BitsetIndex::insert_attribute(VecId vid, AttrNo ano)
{
    attr_bitset[ano][vid] = 1;
}
void BitsetIndex::insert_attributes(VecId vid, std::vector<AttrNo> ano)
{
    for (auto &a : ano)
        attr_bitset[a][vid] = 1;
}

void BitsetIndex::delete_attribute(VecId vid, AttrNo ano)
{
    if(attr_bitset.find(ano) == attr_bitset.end())
        return;
    attr_bitset[ano][vid] = 0;
}
void BitsetIndex::delete_attributes(VecId vid, std::vector<AttrNo> ano)
{
    for (auto &a : ano){
        if (attr_bitset.find(a) != attr_bitset.end())
            attr_bitset[a][vid] = 0;
    }
}

std::vector<VecId> BitsetIndex::get_ids(AttrNo ano)
{
    std::vector<VecId> vids;
    for (VecId i = 0; i < NB; i++)
        if (attr_bitset[ano][i])
            vids.push_back(i);
    return vids;
}
std::vector<AttrNo> BitsetIndex::get_attributes(VecId vid)
{
    std::vector<AttrNo> anos;
    for (auto& [attr_no, at_bs] : attr_bitset)
        if (at_bs[vid])
            anos.push_back(attr_no);
    return anos;
}

std::vector<VecId> BitsetIndex::attr_search_and(std::vector<AttrNo> ano)
{
    std::bitset<NB> bs;
    for (int i = 0;i<ano.size();i++){
        if(attr_bitset.find(ano[i]) == attr_bitset.end())
            return std::vector<VecId>(); // 所有向量都没有这个属性
        if(i==0) bs=attr_bitset[ano[i]];
        else bs&=attr_bitset[ano[i]];
    }
    std::vector<VecId> vids;
    for (VecId i = 0; i < NB; i++) // to be optimized
        if (bs[i])
            vids.push_back(i);
    return vids;
}

std::vector<VecId> BitsetIndex::attr_search_or(std::vector<AttrNo> ano)
{
    std::bitset<NB> bs;
    bool init = false;
    for (int i = 0; i < ano.size(); i++)
    {
        if(attr_bitset.find(ano[i]) == attr_bitset.end())
            continue;
        if (!init){
            bs = attr_bitset[ano[i]];
            init = true;
        }  
        else bs|=attr_bitset[ano[i]];
    }
    
    if (!init) return std::vector<VecId>(); // 所有向量都没有这些属性
    
    std::vector<VecId> vids;
    for (VecId i = 0; i < NB; i++) // to be optimized
        if (bs[i])
            vids.push_back(i);
    return vids;
}

std::vector<VecId> BitsetIndex::attr_search_andor(std::vector<std::vector<AttrNo>> ano)
{
    std::unordered_set<VecId> vids;
    for (auto &a : ano)
    {
        std::vector<VecId> vids_tmp = attr_search_and(a);
        vids.insert(vids_tmp.begin(), vids_tmp.end());
    }
    std::vector<VecId> vids_vec(vids.begin(), vids.end());
    return vids_vec;
}

std::vector<VecId> BitsetIndex::attr_search_orand(std::vector<std::vector<AttrNo>> ano)
{
    std::bitset<NB> bs;
    bs.set(); // all 1

    for (auto &as : ano)
    {
        std::bitset<NB> bs_tmp;
        bool init = false;
        for (int i = 0; i < as.size(); i++)
        {
            if (attr_bitset.find(as[i]) == attr_bitset.end())
                continue;
            if (!init){
                bs_tmp = attr_bitset[as[i]];
                init=true;
            }
            else
                bs_tmp |= attr_bitset[as[i]];
        }
        if(!init) return std::vector<VecId>();
        bs &= bs_tmp;
    }
    
    std::vector<VecId> vids;
    for (VecId i = 0; i < NB; i++) // to be optimized
        if (bs[i])
            vids.push_back(i);
    return vids;
}