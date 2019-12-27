#include "gtest/gtest.h"
#include "../Vic2ToHoI4/Source/HOI4World/ScriptedLocalisations/ScriptedLocalisations.h"



TEST(HoI4World_ScriptedLocalisations_SciptedLocalisationsTests, LocalisationsDefaultsToEmpty)
{
	const HoI4::ScriptedLocalisations theLocalisations;

	ASSERT_TRUE(theLocalisations.getLocalisations().empty());
}


TEST(HoI4World_ScriptedLocalisations_SciptedLocalisationsTests, GetStrengthRatioAdded)
{
	HoI4::ScriptedLocalisations theLocalisations;
	theLocalisations.initialize("TAG", "TWO");

	ASSERT_TRUE(!theLocalisations.getLocalisations().empty());
	ASSERT_EQ(theLocalisations.getLocalisations()[0].getName(), "GetStrengthRatioBritain");
}


TEST(HoI4World_ScriptedLocalisations_SciptedLocalisationsTests, GetRelevantNavalTreatyNationAdded)
{
	HoI4::ScriptedLocalisations theLocalisations;
	theLocalisations.initialize("TAG", "TWO");

	ASSERT_TRUE(theLocalisations.getLocalisations().size() > 1);
	ASSERT_EQ(theLocalisations.getLocalisations()[1].getName(), "GetRelevantNavalTreatyNation");
}


TEST(HoI4World_ScriptedLocalisations_SciptedLocalisationsTests, TagsGetApplied)
{
	HoI4::ScriptedLocalisations theLocalisations;
	theLocalisations.initialize("TAG", "TWO");

	ASSERT_TRUE(theLocalisations.getLocalisations().size() > 1);
	ASSERT_EQ(2, theLocalisations.getLocalisations()[1].getTexts().size());

	std::string textTwo = "\t\ttrigger = { \n";
	textTwo += "\t\t\tTAG = { \n";
	textTwo += "\t\t\t\tnot = { has_naval_treaty_trigger = yes }\n";
	textTwo += "\t\t\t}\n";
	textTwo += "\t\t\tTWO = { \n";
	textTwo += "\t\t\t\thas_naval_treaty_trigger = yes\n";
	textTwo += "\t\t\t}\n";
	textTwo += "\t\t}\n";
	textTwo += "\t\tlocalization_key = TWO_Naval_treaty_nation\n";
	ASSERT_EQ(theLocalisations.getLocalisations()[1].getTexts()[1], textTwo);
}