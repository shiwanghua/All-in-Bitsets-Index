#include <FlatIndex.h>

#include <iostream>
#include <cstring>

FlatIndex::FlatIndex(int32_t _d, int32_t _max_attr_num) : embedding_d(_d), max_attr_num(_max_attr_num), n(0)
{
    total_d = embedding_d + max_attr_num * 2; // 属性是 64 位的，相当于 2 个 32 位
}

bool FlatIndex::build(std::vector<std::vector<float>> embeddings, std::vector<std::vector<AttrNo>> attributes)
{
    if (embeddings.size() != attributes.size())
    {
        std::cerr << "FlatIndex::build: embeddings size != attributes size" << std::endl;
        return false;
    }
    raw_data.resize(embeddings.size());
    for (int i = 0; i < embeddings.size(); i++)
    {
        if (embeddings[i].size() != embedding_d)
        {
            raw_data.clear();
            std::cerr << "FlatIndex::build: " << i << "-th embedding'd is " << embeddings[i].size() << " != " << embedding_d << std::endl;
            return false;
        }
        raw_data[i].resize(total_d);
        std::memcpy(raw_data[i].data(), embeddings[i].data(), sizeof(float) * embedding_d);
        if (attributes[i].size() > max_attr_num)
        {
            std::cerr << "FlatIndex::build: " << i << "th attribute size is " << attributes[i].size() << " > " << max_attr_num << std::endl;
            return false;
        }
        std::memcpy((char *)(raw_data[i].data() + embedding_d), attributes[i].data(), sizeof(AttrNo) * attributes[i].size());
        std::memset((char *)(raw_data[i].data() + embedding_d + attributes[i].size() * 2), 0LL, sizeof(AttrNo) * (max_attr_num - attributes[i].size()));
    }
    n = embeddings.size();
    return true;
}

bool FlatIndex::insert_attribute(VecId vid, AttrNo ano)
{
    if (vid > raw_data.size() || vid < 0)
    {
        std::cerr << "FlatIndex::insert_attribute: invalid vid " << vid << std::endl;
        return false;
    }
    if (ano <= 0)
    {
        std::cerr << "FlatIndex::insert_attribute: invalid ano " << ano << std::endl;
        return false;
    }
    int attr_num = 0, s = embedding_d;
    while (attr_num < max_attr_num)
    {
        AttrNo attr_no = *(AttrNo *)&raw_data[vid][s];
        if (attr_no == 0)
        {
            break;
        }
        else if (attr_no == ano)
        {
            std::cerr << "FlatIndex::insert_attribute: exist attr_no " << attr_no << " at position " << attr_num << std::endl;
            return false;
        }
        attr_num++;
    }
    if (attr_num == max_attr_num)
    {

        return false;
    }
}
void FlatIndex::insert_attributes(VecId vid, std::vector<AttrNo> ano)
{
    // for (auto &a : ano)
    //     attr_bitset[a][vid] = 1;
    // return true;
}

void FlatIndex::delete_attribute(VecId vid, AttrNo ano)
{
    if (attr_bitset.find(ano) == attr_bitset.end())
        return;
    attr_bitset[ano][vid] = 0;
}

void FlatIndex::delete_attributes(VecId vid, std::vector<AttrNo> ano)
{
    for (auto &a : ano)
    {
        if (attr_bitset.find(a) != attr_bitset.end())
            attr_bitset[a][vid] = 0;
    }
}

std::vector<VecId> FlatIndex::get_ids(AttrNo ano)
{
    std::vector<VecId> vids;
    for (VecId i = 0; i < NB; i++)
        if (attr_bitset[ano][i])
            vids.push_back(i);
    return vids;
}
std::vector<AttrNo> FlatIndex::get_attributes(VecId vid)
{
    std::vector<AttrNo> anos;
    for (auto &[attr_no, at_bs] : attr_bitset)
        if (at_bs[vid])
            anos.push_back(attr_no);
    return anos;
}

std::vector<VecId> FlatIndex::attr_search_and(std::vector<AttrNo> ano)
{
    std::bitset<NB> bs;
    for (int i = 0; i < ano.size(); i++)
    {
        if (attr_bitset.find(ano[i]) == attr_bitset.end())
            return std::vector<VecId>(); // 所有向量都没有这个属性
        if (i == 0)
            bs = attr_bitset[ano[i]];
        else
            bs &= attr_bitset[ano[i]];
    }
    std::vector<VecId> vids;
    for (VecId i = 0; i < NB; i++) // to be optimized
        if (bs[i])
            vids.push_back(i);
    return vids;
}

std::vector<VecId> FlatIndex::attr_search_or(std::vector<AttrNo> ano)
{
    std::bitset<NB> bs;
    bool init = false;
    for (int i = 0; i < ano.size(); i++)
    {
        if (attr_bitset.find(ano[i]) == attr_bitset.end())
            continue;
        if (!init)
        {
            bs = attr_bitset[ano[i]];
            init = true;
        }
        else
            bs |= attr_bitset[ano[i]];
    }

    if (!init)
        return std::vector<VecId>(); // 所有向量都没有这些属性

    std::vector<VecId> vids;
    for (VecId i = 0; i < NB; i++) // to be optimized
        if (bs[i])
            vids.push_back(i);
    return vids;
}

std::vector<VecId> FlatIndex::attr_search_andor(std::vector<std::vector<AttrNo>> ano)
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

std::vector<VecId> FlatIndex::attr_search_orand(std::vector<std::vector<AttrNo>> ano)
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
            if (!init)
            {
                bs_tmp = attr_bitset[as[i]];
                init = true;
            }
            else
                bs_tmp |= attr_bitset[as[i]];
        }
        if (!init)
            return std::vector<VecId>();
        bs &= bs_tmp;
    }

    std::vector<VecId> vids;
    for (VecId i = 0; i < NB; i++) // to be optimized
        if (bs[i])
            vids.push_back(i);
    return vids;
}