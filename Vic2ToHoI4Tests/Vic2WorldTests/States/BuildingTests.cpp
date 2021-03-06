#include "../../../Vic2ToHoI4/Source/V2World/States/BuildingReader.h"
#include "gtest/gtest.h"
#include <sstream>



TEST(Vic2World_States_BuildingReaderTests, LevelDefaultsToZero)
{
	Vic2::BuildingReader reader;

	std::stringstream input;
	ASSERT_EQ(0, reader.getLevel(input));
}


TEST(Vic2World_States_BuildingReaderTests, LevelCanBeRead)
{
	Vic2::BuildingReader reader;

	std::stringstream input;
	input << "= {\n";
	input << "\tlevel = 7\n";
	input << "}";
	ASSERT_EQ(7, reader.getLevel(input));
}