#pragma once
#include "RangesPCH.hpp"
// Only Prototype to answer some Job related question!!


template<class... Functs>
struct CompilerModule:Functs...
{
	using Functs::operator()...;
};

template<class... Functs>
CompilerModule(Functs...)->CompilerModule<Functs...>;


template<class EntryBlock, class...Blocks>
struct Function
{

};

class Block
{
public:
	Block() = default;
	Block(bool trueOrFalse, Block* successor, std::string tag="") : successors{ {trueOrFalse, successor, std::move(tag)} } {}

private:
	// tried to use shared_ptr but since each block reference to each other it might create circular ref.
	std::vector<std::tuple<bool, Block*, std::string>>successors;
};