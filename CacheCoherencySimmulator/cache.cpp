#include "cache.h"

void Cache::connectToMemory(MemoryBlock *memoryConnection)
{
    this->memoryConnection = memoryConnection;
    this->memoryPort.connectToMemory(memoryConnection);
}

void Cache::reset()
{
    this->setCache(this->cacheSizeInBytes, this->cacheLineSizeInBytes, this->sizeOfConnectedMemoryInBytes);
}

void Cache::reset(int cacheSizeInBytes, int cacheLineSizeInBytes, int sizeOfConnectedMemoryInBytes)
{
    this->setCache(cacheSizeInBytes, cacheLineSizeInBytes, sizeOfConnectedMemoryInBytes);
}

Cache::Cache(int cacheSizeInBytes, int cacheLineSizeInBytes, int sizeOfConnectedMemoryInBytes)
{
    this->setCache(cacheSizeInBytes, cacheLineSizeInBytes, sizeOfConnectedMemoryInBytes);
}

void Cache::setCache(int cacheSizeInBytes, int cacheLineSizeInBytes, int sizeOfConnectedMemoryInBytes)
{
    int noOfLines = cacheSizeInBytes/cacheLineSizeInBytes;
    this->cacheLineSizeInBytes = cacheLineSizeInBytes;
    this->cacheSizeInBytes = cacheSizeInBytes;
    std::vector<std::bitset<8>> cacheLine = std::vector<std::bitset<8>> (cacheLineSizeInBytes, 0);
    this->space = std::vector<std::vector<std::bitset<8>>> ( noOfLines , cacheLine);
    this->lruBlock.resize( noOfLines , -1);
    this->tagBlock.resize(noOfLines,"");
    this->sizeOfConnectedMemoryInBytes = sizeOfConnectedMemoryInBytes;
}

std::bitset<8> Cache::locateByDirectMapping(std::bitset<13> requestedAddress, int &cacheHit)
{
    /*
    for 13 bit mem, 256+32 cache
    BI<8>| BO<5>
    TB<5>| CA<8>
    TB<5>| CI<3>| BO<5>

    256+64
    BI<7>| BO<6>
    TB<5>| CA<8>
    TB<5>| CI<2>| BO<6>

    take CI, convert to integer, use as index in tagBlock vector
    compare the TB value with the indexed value
    */
    //ca bits = log2(CS);
    //bo = log2(ls);
    int pa = ceil(log2(this->sizeOfConnectedMemoryInBytes));
    int ca = ceil(log2(this->cacheSizeInBytes));
    this->essentialTagBits = pa - ca;
    std::string requestedAddressString = requestedAddress.to_string();

    int cacheIndexBits = log2(this->cacheSizeInBytes/this->cacheLineSizeInBytes);
    int blockOffsetBits = log2(this->cacheLineSizeInBytes);
    int cacheIndex = std::stoul(requestedAddressString.substr(this->essentialTagBits, cacheIndexBits),nullptr,2);

    std::string tbValue = this->tagBlock[cacheIndex];
    std::string tbTrimmed = "";
    if(tbValue != "")
        tbTrimmed = tbValue.substr(16-this->essentialTagBits, this->essentialTagBits);
    std::string requestedTagString = requestedAddressString.substr(0, this->essentialTagBits);

    int blockIndexbits = pa - blockOffsetBits;
    int blockIndex = std::stoul(requestedAddressString.substr(0, blockIndexbits),nullptr,2);
    BigPacket packet(this->cacheLineSizeInBytes);
    if(tbTrimmed == requestedTagString)
    {
        ++cacheHit;
        //std::cout<<"Cache hit for "<< std::hex << requestedAddress.to_ulong()<<std::endl;
        packet.populateBlockData(this->space[cacheIndex]);
    }else
    {
        //std::cout<<"Cache miss for "<< std::hex << requestedAddress.to_ulong()<<std::endl;
        //std::cout<<"Fetching from memory"<<std::endl;
        //fetch from memory
        this->tagBlock[cacheIndex] = (std::bitset<16>(requestedTagString)).to_string();
        
        this->retrieveFromMemory(blockIndex, packet);
        this->space[cacheIndex] = packet.extractBlock();
        
    }
    int blockOffset = std::stoul(requestedAddressString.substr(pa - blockOffsetBits, blockOffsetBits),nullptr,2);
    return packet.extractByteByIndex(blockOffset);
    //return packet;
}

void Cache::retrieveFromMemory(int blockIndex, BigPacket &packet)
{
    //extract TagBits and CI
    //use as index in memory vector
    this->memoryPort.getDataByBlock(this->cacheLineSizeInBytes, blockIndex, packet);
}

std::bitset<8> Cache::locateByFullyAssociativeMapping(std::bitset<13> requestedAddress, int &cacheHit)
{
    /*
    for 13 bit mem, 256+32 cache
    BI<8>| BO<5>
    TB<8>| BO<5>

    extract essentialTagBits
    compare each and every entry in the tag block with the requestedTagValue/block index
    when found, use the index from tab block, as index to fetch block from cache

    */
    
    int pa = ceil(log2(this->sizeOfConnectedMemoryInBytes));
    int blockOffsetBits = log2(this->cacheLineSizeInBytes);
    //int ca = ceil(log2(this->cacheSizeInBytes));
    this->essentialTagBits = pa - blockOffsetBits;
    int blockIndexbits = this->essentialTagBits;
    std::string requestedAddressString = requestedAddress.to_string();

    std::string requestedTagString = requestedAddressString.substr(0, this->essentialTagBits);
    bool found = false;
    int tagIndex = 0;
    for(int itr = 0; itr < this->tagBlock.size(); ++itr)
    {
        std::string tbValue = this->tagBlock[itr];
        std::string tbTrimmed = "";
        if(tbValue != "")
            tbTrimmed = tbValue.substr(16-this->essentialTagBits, this->essentialTagBits);
        if(tbTrimmed == requestedTagString)
        {
            found = true;
            tagIndex = itr;
            break;
        }
    }    

    int blockIndex = std::stoul(requestedAddressString.substr(0, blockIndexbits),nullptr,2);
    BigPacket packet(this->cacheLineSizeInBytes);
    if(found)
    {
        //std::cout<<"found @ "<<tagIndex<<std::endl;
        ++cacheHit;
        //std::cout<<"Cache hit for "<< std::hex << requestedAddress.to_ulong()<<std::endl;
        packet.populateBlockData(this->space.at(tagIndex));
        //update lruBlock
        this->updateLRUBlock(tagIndex);
    }else
    {
        //fetch from memory
        //std::cout<<"Cache miss for "<< std::hex << requestedAddress.to_ulong()<<std::endl;
        //std::cout<<"Fetching from memory"<<std::endl;
        int lruIndex = this->getLRUIndex();
        this->tagBlock[lruIndex] = (std::bitset<16>(requestedTagString)).to_string();
        //std::cout<<"placed @ "<<lruIndex<<std::endl;
        this->retrieveFromMemory(blockIndex, packet);
        this->space[lruIndex] = packet.extractBlock();        
    }
    int blockOffset = std::stoul(requestedAddressString.substr(pa - blockOffsetBits, blockOffsetBits),nullptr,2);
    return packet.extractByteByIndex(blockOffset);
    //return packet;
}

int Cache::getLRUIndex()
{
    int lruIndex = 0;
    if(this->lruBlock[0] != -1)
    {
        lruIndex = this->lruBlock[0];
    }
    this->lruBlock.erase(this->lruBlock.begin());
    this->lruBlock.push_back(lruIndex);
    return lruIndex;
}

void Cache::updateLRUBlock(int lruValue)
{
    std::vector<int>::iterator it;
    for(it = this->lruBlock.begin(); it != this->lruBlock.end(); it++)
    {
        if(lruValue == *it)
            break;
    }
    this->lruBlock.erase(it);
    this->lruBlock.push_back(lruValue);
}

std::bitset<8> Cache::locateBySetAssociativeMapping(int setSize, std::bitset<13> requestedAddress, int &cacheHit)
{
    /*
    setSize * setIndex will give tagIndex
    using the tagIndex, scan setSize number of blocks in the tagBlock
    compare value with tagBits from address space

    */
    int pa = ceil(log2(this->sizeOfConnectedMemoryInBytes));
    int noOfLines = this->cacheSizeInBytes/this->cacheLineSizeInBytes;
    int noOfSets = noOfLines/setSize;
    int setIndexBits = ceil(log2(noOfSets));
    int blockOffsetBits = log2(this->cacheLineSizeInBytes);
    this->essentialTagBits = pa - setIndexBits - blockOffsetBits;
    std::string requestedAddressString = requestedAddress.to_string();
    int setIndex = std::stoul(requestedAddressString.substr(this->essentialTagBits, setIndexBits),nullptr,2);

    int tagIndex = setIndex * setSize;
    
    std::string requestedTagString = requestedAddressString.substr(0, this->essentialTagBits);
    bool found = false;
    int cacheIndex = 0;
    for(int itr = 0; itr < setSize; ++itr)
    {
        std::string tbValue = this->tagBlock[tagIndex + itr];
        std::string tbTrimmed = "";
        if(tbValue != "")
            tbTrimmed = tbValue.substr(16-this->essentialTagBits, this->essentialTagBits);
        if(tbTrimmed == requestedTagString)
        {
            found = true;
            cacheIndex = tagIndex + itr;
            break;
        }
    }    

    int blockIndexbits = pa - blockOffsetBits;
    int blockIndex = std::stoul(requestedAddressString.substr(0, blockIndexbits),nullptr,2);
    BigPacket packet(this->cacheLineSizeInBytes);
    
    if(found)
    {
        //std::cout<<"found @ "<<cacheIndex<<std::endl;
        ++cacheHit;
        //std::cout<<"Cache hit for "<< std::hex << requestedAddress.to_ulong()<<std::endl;
        packet.populateBlockData(this->space.at(cacheIndex));
        //update lruBlock
        //this->updateLRUBlock(tagIndex);
    }else
    {
        //fetch from memory
        //std::cout<<"Cache miss for "<< std::hex << requestedAddress.to_ulong()<<std::endl;
        //std::cout<<"Fetching from memory"<<std::endl;
        int lruIndex = this->getLRUIndex(setSize, setIndex);
        this->tagBlock[lruIndex] = (std::bitset<16>(requestedTagString)).to_string();
        //std::cout<<"placed @ "<<lruIndex<<std::endl;
        this->retrieveFromMemory(blockIndex, packet);
        this->space[lruIndex] = packet.extractBlock();        
    }

    int blockOffset = std::stoul(requestedAddressString.substr(pa - blockOffsetBits, blockOffsetBits),nullptr,2);
    return packet.extractByteByIndex(blockOffset);
    //return packet;
}

int Cache::getLRUIndex(int setSize, int setIndex)
{
    int index = setSize * setIndex;
    int lruIndex = 0;
    if(this->lruBlock[index] != -1)
    {
        lruIndex = this->lruBlock[index];
    }
    this->lruBlock.insert(this->lruBlock.begin() + index + setSize, lruIndex);
    this->lruBlock.erase(this->lruBlock.begin() + index);
    
    return 0;
}

void Cache::updateLRUBlock(int lruValue, int setSize, int setIndex)
{
    int index = setSize * setIndex;
    std::vector<int>::iterator it;
    for(it = this->lruBlock.begin() + index; it != this->lruBlock.begin() + index + setSize; it++)
    {
        if(lruValue == *it)
            break;
    }
    this->lruBlock.insert(this->lruBlock.begin() + index + setSize, lruValue);
    this->lruBlock.erase(it);
}