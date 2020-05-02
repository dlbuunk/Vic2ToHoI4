#include "OutHoi4Country.h"
#include "../HOI4World/Diplomacy/Faction.h"
#include "../HOI4World/HoI4Country.h"
#include "../HOI4World/Leaders/Advisor.h"
#include "../HOI4World/Military/DivisionTemplate.h"
#include "../HOI4World/Names.h"
#include "../HOI4World/Navies/NavyNames.h"
#include "../Mappers/GraphicsMapper.h"
#include "../V2World/Country.h"
#include "Date.h"
#include "Leaders/OutAdmiral.h"
#include "Leaders/OutAdvisor.h"
#include "Leaders/OutGeneral.h"
#include "Navies/OutLegacyNavyNames.h"
#include "Navies/OutMtgNavyNames.h"
#include "Navies/OutNavies.h"
#include "OSCompatibilityLayer.h"
#include "OutFocusTree.h"
#include "OutTechnologies.h"
#include <string>



void HoI4::outputToCommonCountriesFile(std::ostream& countriesFile, const Country& theCountry)
{
	countriesFile << theCountry.getTag() << " = \"countries/"
					  << Utils::normalizeUTF8Path(theCountry.getCommonCountryFile()) << "\"\n";
}


void HoI4::outputColors(std::ostream& out, const Country& theCountry)
{
	out << theCountry.getTag() << " = {\n";
	out << "\tcolor = rgb { " << theCountry.getColor() << " }\n";
	out << "\tcolor_ui = rgb { " << theCountry.getColor() << " }";
	out << "}\n";
}


void outputNamesSet(std::ostream& namesFile,
	 const std::optional<std::vector<std::string>>& names,
	 const std::string& tabs);

void HoI4::outputToNamesFiles(std::ostream& namesFile, const namesMapper& theNames, const Country& theCountry)
{
	const auto primaryCulture = theCountry.getSourceCountry().getPrimaryCulture();
	auto femaleSurnames = theNames.getFemaleSurnames(primaryCulture);

	namesFile << theCountry.getTag() << " = {\n";
	namesFile << "\tmale = {\n";
	namesFile << "\t\tnames = {\n";
	outputNamesSet(namesFile, theNames.getMaleNames(primaryCulture), "\t\t\t");
	namesFile << "\t\t}\n";
	if (!femaleSurnames->empty())
	{
		namesFile << "\t\tsurnames = {\n";
		outputNamesSet(namesFile, theNames.getSurnames(primaryCulture), "\t\t");
		namesFile << "\t\t}\n";
	}
	namesFile << "\t}\n";

	namesFile << "\tfemale = {\n";
	namesFile << "\t\tnames = {\n";
	outputNamesSet(namesFile, theNames.getFemaleNames(primaryCulture), "\t\t\t");
	namesFile << "\t\t}\n";
	if (!femaleSurnames->empty())
	{
		namesFile << "\t\tsurnames = {\n";
		outputNamesSet(namesFile, femaleSurnames, "\t\t");
		namesFile << "\t}\n";
	}
	namesFile << "\t}\n";

	if (femaleSurnames->empty())
	{
		namesFile << "\tsurnames = {\n";
		outputNamesSet(namesFile, theNames.getSurnames(primaryCulture), "\t\t");
		namesFile << "\t}\n";
	}

	namesFile << "\tcallsigns = {\n";
	outputNamesSet(namesFile, theNames.getCallsigns(primaryCulture), "\t\t");
	namesFile << "\t}\n";

	namesFile << "}\n";
}


void outputNamesSet(std::ostream& namesFile,
	 const std::optional<std::vector<std::string>>& names,
	 const std::string& tabs)
{
	if (names)
	{
		namesFile << tabs;

		for (unsigned int i = 0; i < names->size(); i++)
		{
			namesFile << '\"' << (*names)[i] << '\"';
			if ((i + 1) == names->size())
			{
				continue;
			}
			if (((i + 1) % 10) == 0)
			{
				namesFile << "\n";
				namesFile << tabs;
			}
			else
			{
				namesFile << " ";
			}
		}

		namesFile << '\n';
	}
}


void HoI4::outputToUnitNamesFiles(const Country& theCountry, const Configuration& theConfiguration)
{
	const auto& tag = theCountry.getTag();

	std::ofstream legacyUnitNamesFile(
		 "output/" + theConfiguration.getOutputName() + "/common/units/names/" + tag + "_names.txt");
	if (!legacyUnitNamesFile.is_open())
	{
		throw std::runtime_error(
			 "Could not open output/" + theConfiguration.getOutputName() + "/common/units/names/" + tag + "_names.txt");
	}
	legacyUnitNamesFile << "\xEF\xBB\xBF"; // add the BOM to make HoI4 happy
	outLegacyNavyNames(legacyUnitNamesFile, theCountry.getNavyNames().getLegacyShipTypeNames(), tag);
	legacyUnitNamesFile.close();

	std::ofstream mtgUnitNamesFile(
		 "output/" + theConfiguration.getOutputName() + "/common/units/names_ships/" + tag + "_ship_names.txt");
	if (!mtgUnitNamesFile.is_open())
	{
		throw std::runtime_error("Could not open output/" + theConfiguration.getOutputName() +
										 "/common/units/names_ships/" + tag + "_ship_names.txt");
	}
	mtgUnitNamesFile << "\xEF\xBB\xBF"; // add the BOM to make HoI4 happy
	outMtgNavyNames(mtgUnitNamesFile, theCountry.getNavyNames().getMtgShipTypeNames(), tag);
	mtgUnitNamesFile.close();
}


void HoI4::outputIdeaGraphics(std::ostream& ideasFile, const Country& theCountry)
{
	const auto& tag = theCountry.getTag();
	const auto& primaryCultureGroup = theCountry.getSourceCountry().getPrimaryCultureGroup();

	ideasFile << "\tspriteType = {\n";
	ideasFile << "\t\tname = \"GFX_idea_" << tag << "_communist_advisor\"\n";
	ideasFile << "\t\ttexturefile = \"" << theCountry.getCommunistAdvisorPortrait() << "\"\n";
	ideasFile << "\t}\n";

	ideasFile << "\tspriteType = {\n";
	ideasFile << "\t\tname = \"GFX_idea_" << tag << "_democratic_advisor\"\n";
	ideasFile << "\t\ttexturefile = \"" << theCountry.getDemocraticAdvisorPortrait() << "\"\n";
	ideasFile << "\t}\n";

	ideasFile << "\tspriteType = {\n";
	ideasFile << "\t\tname = \"GFX_idea_" << tag << "_neutrality_advisor\"\n";
	ideasFile << "\t\ttexturefile = \"" << theCountry.getNeutralityAdvisorPortrait() << "\"\n";
	ideasFile << "\t}\n";

	ideasFile << "\tspriteType = {\n";
	ideasFile << "\t\tname = \"GFX_idea_" << tag << "_absolutist_advisor\"\n";
	ideasFile << "\t\ttexturefile = \"" << theCountry.getAbsolutistAdvisorPortrait() << "\"\n";
	ideasFile << "\t}\n";

	ideasFile << "\tspriteType = {\n";
	ideasFile << "\t\tname = \"GFX_idea_" << tag << "_radical_advisor\"\n";
	ideasFile << "\t\ttexturefile = \"" << theCountry.getRadicalAdvisorPortrait() << "\"\n";
	ideasFile << "\t}\n";

	ideasFile << "\tspriteType = {\n";
	ideasFile << "\t\tname = \"GFX_idea_" << tag << "_fascist_advisor\"\n";
	ideasFile << "\t\ttexturefile = \"" << theCountry.getFascistAdvisorPortrait() << "\"\n";
	ideasFile << "\t}\n";
}


void outputHistory(const HoI4::Country& theCountry, const Configuration& theConfiguration);
void outputOOB(const std::vector<HoI4::DivisionTemplateType>& divisionTemplates,
	 const HoI4::Country& theCountry,
	 const Configuration& theConfiguration);
void outputCommonCountryFile(const HoI4::Country& theCountry, const Configuration& theConfiguration);
void outputAdvisorIdeas(const std::string& tag,
	 const std::set<HoI4::Advisor>& ideologicalAdvisors,
	 const Configuration& theConfiguration);

void HoI4::outputCountry(const std::set<Advisor>& ideologicalMinisters,
	 const std::vector<DivisionTemplateType>& divisionTemplates,
	 const Country& theCountry,
	 const Configuration& theConfiguration)
{
	if (theCountry.getCapitalState())
	{
		outputHistory(theCountry, theConfiguration);
		outputOOB(divisionTemplates, theCountry, theConfiguration);
		outputCommonCountryFile(theCountry, theConfiguration);
		outputAdvisorIdeas(theCountry.getTag(), ideologicalMinisters, theConfiguration);

		if (auto nationalFocus = theCountry.getNationalFocus(); nationalFocus)
		{
			outputFocusTree(*nationalFocus,
				 "output/" + theConfiguration.getOutputName() + "/common/national_focus/" + theCountry.getTag() +
					  "_NF.txt");
		}
	}
}


void outputCapital(std::ostream& output, const std::optional<int>& capitalStateNum);
void outputResearchSlots(std::ostream& output, const bool& greatPower, const bool& civilized);
void outputThreat(std::ostream& output, const double& threat);
void outputWars(std::ostream& output, const std::vector<HoI4::War>& wars);
void outputOOBLines(std::ostream& output, const std::string& tag);
void outputConvoys(std::ostream& output, const int& convoys);
void outputEquipmentStockpile(std::ostream& output,
	 const std::map<std::string, unsigned int>& equipmentStockpile,
	 const std::string& tag);
void outputPuppets(std::ostream& output,
	 const std::string& tag,
	 const std::string& governmentIdeology,
	 const std::set<std::string>& puppets,
	 const std::string& puppetMaster,
	 const std::map<std::string, double>& spherelings);
void outputPolitics(std::ostream& output,
	 const std::string& governmentIdeology,
	 const date& lastElection,
	 const bool& areElectionsAllowed,
	 const std::map<std::string, int>& ideologySupport);
void outputRelations(std::ostream& output,
	 const std::string& tag,
	 const std::map<std::string, HoI4::Relations>& relations);
void outputFactions(std::ostream& output,
	 const std::string& tag,
	 const std::optional<HoI4::Faction>& faction,
	 const std::optional<std::string>& possibleLeaderName);
void outputGuaranteedSpherelings(std::ostream& output, const std::vector<std::string>& guaranteed);
void outputIdeas(std::ostream& output,
	 const bool& greatPower,
	 const bool& civilized,
	 const std::set<std::string>& ideas,
	 const std::string& mobilizationLaw,
	 const std::string& economicLaw,
	 const std::string& tradeLaw,
	 const std::string& primaryCulture);
void outputStability(std::ostream& output, const int& stability);
void outputWarSupport(std::ostream& output, const int& warSupport);
void outputCountryLeader(std::ostream& output,
	 const std::string& leaderPortrait,
	 const std::string& leaderName,
	 const std::string& leaderSurname,
	 const std::string& leaderIdeology,
	 const std::string& tag);
void outputCommanders(std::ostream& output,
	 const std::vector<HoI4::General>& generals,
	 const std::vector<HoI4::Admiral>& admirals);


void outputHistory(const HoI4::Country& theCountry, const Configuration& theConfiguration)
{
	const auto& tag = theCountry.getTag();
	const auto& governmentIdeology = theCountry.getGovernmentIdeology();
	const auto& primaryCulture = theCountry.getSourceCountry().getPrimaryCulture();

	std::ofstream output("output/" + theConfiguration.getOutputName() + "/history/countries/" +
								Utils::normalizeUTF8Path(theCountry.getFilename()));
	if (!output.is_open())
	{
		throw std::runtime_error("Could not open output/" + theConfiguration.getOutputName() + "/history/countries/" +
										 Utils::normalizeUTF8Path(theCountry.getFilename()));
	}
	output << "\xEF\xBB\xBF"; // add the BOM to make HoI4 happy

	outputCapital(output, theCountry.getCapitalState());
	outputResearchSlots(output, theCountry.isGreatPower(), theCountry.isCivilized());
	outputThreat(output, theCountry.getThreat());
	outputWars(output, theCountry.getWars());
	outputOOBLines(output, tag);
	if (const auto& theTechnologies = theCountry.getTechnologies(); theTechnologies)
	{
		outputTechnology(*theTechnologies, output);
		outputResearchBonuses(*theTechnologies, output);
	}
	outputConvoys(output, theCountry.getConvoys());
	outputEquipmentStockpile(output, theCountry.getEquipmentStockpile(), tag);
	outputPuppets(output,
		 tag,
		 governmentIdeology,
		 theCountry.getPuppets(),
		 theCountry.getPuppetMaster(),
		 theCountry.getSpherelings());
	outputPolitics(output,
		 governmentIdeology,
		 theCountry.getLastElection(),
		 theCountry.areElectionsAllowed(),
		 theCountry.getIdeologySupport());
	outputRelations(output, tag, theCountry.getRelations());
	outputFactions(output, tag, theCountry.getFaction(), theCountry.getSourceCountry().getName("english"));
	outputGuaranteedSpherelings(output, theCountry.getGuaranteed());
	outputIdeas(output,
		 theCountry.isGreatPower(),
		 theCountry.isCivilized(),
		 theCountry.getIdeas(),
		 theCountry.getMobilizationLaw(),
		 theCountry.getEconomicLaw(),
		 theCountry.getTradeLaw(),
		 primaryCulture);
	if (theCountry.hasProvinces())
	{
		outputStability(output, theCountry.getStability());
		outputWarSupport(output, theCountry.getWarSupport());
	}
	outputCountryLeader(output,
		 theCountry.getLeaderPortrait(),
		 theCountry.getLeaderName(),
		 theCountry.getLeaderSurname(),
		 theCountry.getLeaderIdeology(),
		 tag);
	outputCommanders(output, theCountry.getGenerals(), theCountry.getAdmirals());
	output << theCountry.getTheShipVariants();

	output.close();
}


void outputCapital(std::ostream& output, const std::optional<int>& capitalStateNum)
{
	if (capitalStateNum)
	{
		output << "capital = " << *capitalStateNum << '\n';
	}
	else
	{
		output << "capital = 1\n";
	}
}


void outputResearchSlots(std::ostream& output, const bool& greatPower, const bool& civilized)
{
	if (greatPower)
	{
		output << "set_research_slots = 4\n";
	}
	else if (civilized)
	{
		output << "set_research_slots = 3\n";
	}
	else
	{
		output << "set_research_slots = 2\n";
	}
}


void outputThreat(std::ostream& output, const double& threat)
{
	if (threat > std::numeric_limits<float>::epsilon())
	{
		output << "add_named_threat = { threat = " << threat << " name = infamy }\n";
	}
	output << "\n";
}


void outputWars(std::ostream& output, const std::vector<HoI4::War>& wars)
{
	for (const auto& war: wars)
	{
		output << war;
	}
}


void outputOOBLines(std::ostream& output, const std::string& tag)
{
	output << "oob = \"" << tag << "_OOB\"\n";
	output << "if = {\n";
	output << "\tlimit = { has_dlc = \"Man the Guns\" }\n";
	output << "\t\tset_naval_oob = \"" << tag << "_1936_naval_mtg\"\n";
	output << "\telse = { \n";
	output << "\t\tset_naval_oob = \"" << tag << "_1936_naval_legacy\"\n";
	output << "\t}\n";
	output << "}\n";

	output << "\n";
}


void outputConvoys(std::ostream& output, const int& convoys)
{
	output << "set_convoys = " << convoys << '\n';
	output << "\n";
}


void outputEquipmentStockpile(std::ostream& output,
	 const std::map<std::string, unsigned int>& equipmentStockpile,
	 const std::string& tag)
{
	for (const auto& eqp: equipmentStockpile)
	{
		output << "add_equipment_to_stockpile = ";
		output << "{ type = " << eqp.first << " amount = " << eqp.second << " producer = " << tag << " }\n";
	}
	output << "\n";
}


void outputPuppets(std::ostream& output,
	 const std::string& tag,
	 const std::string& governmentIdeology,
	 const std::set<std::string>& puppets,
	 const std::string& puppetMaster,
	 const std::map<std::string, double>& spherelings)
{
	if (!puppets.empty() || !spherelings.empty())
	{
		output << "# DIPLOMACY\n";
		output << "if = {\n";
		output << "\tlimit = {\n";
		output << "\t\tOR = {\n";
		output << "\t\t\thas_dlc = \"Together for Victory\"\n";
		output << "\t\t\thas_dlc = \"Man the Guns\"\n";
		output << "\t\t}\n";
		output << "\t}\n";
		if (!puppets.empty())
		{
			for (const auto& puppet: puppets)
			{
				if (governmentIdeology == "fascism")
				{
					output << "    set_autonomy = {\n";
					output << "        target = " << puppet << "\n";
					output << "        autonomous_state = autonomy_integrated_puppet\n";
					output << "    }\n";
				}
				else
				{
					output << "    set_autonomy = {\n";
					output << "        target = " << puppet << "\n";
					output << "        autonomous_state = autonomy_dominion\n";
					output << "        freedom_level = 0.4\n";
					output << "    }\n";
				}
			}
		}

		if (!spherelings.empty())
		{
			for (auto& sphereling: spherelings)
			{
				if (!puppets.count(sphereling.first))
				{
					output << "\tset_autonomy = {\n";
					output << "\t\ttarget = " << sphereling.first << "\n";
					output << "\t\tautonomous_state = autonomy_sphereling\n";
					output << "\t\tfreedom_level = " << sphereling.second << "\n";
					output << "\t}\n";
				}
			}
		}

		output << "    else = {\n";
		for (const auto& puppet: puppets)
		{
			if (governmentIdeology == "fascism")
			{
				output << "        set_autonomy = {\n";
				output << "            target = " << puppet << "\n";
				output << "            autonomous_state = autonomy_puppet\n";
				output << "        }\n";
			}
			else
			{
				output << "        puppet = " << puppet << "\n";
			}
		}
		output << "    }\n";
		output << "}\n";
		output << "\n";

		output << "if = {\n";
		output << "    limit = {has_dlc = \"Together for Victory\" }\n";
		output << "\n";
		output << "    add_to_tech_sharing_group = " << tag << "_research\n";
		output << "}\n\n";
	}

	if (!puppetMaster.empty())
	{
		output << "if = {\n";
		output << "    limit = {has_dlc = \"Together for Victory\" }\n";
		output << "\n";
		output << "    add_to_tech_sharing_group = " << puppetMaster << "_research\n";
		output << "}\n\n";
	}
}


void outputPolitics(std::ostream& output,
	 const std::string& governmentIdeology,
	 const date& lastElection,
	 const bool& areElectionsAllowed,
	 const std::map<std::string, int>& ideologySupport)
{
	output << "set_politics = {\n";
	output << "    ruling_party = " << governmentIdeology << "\n";
	output << "    last_election = \"" << lastElection << "\"\n";
	output << "    election_frequency = 48\n";
	if (areElectionsAllowed)
	{
		output << "    elections_allowed = yes\n";
	}
	else
	{
		output << "    elections_allowed = no\n";
	}
	output << "}\n";
	output << "\n";

	output << "set_popularities = {\n";
	for (const auto& ideology: ideologySupport)
	{
		output << "\t" << ideology.first << " = " << ideology.second << "\n";
	}
	output << "}\n";
	output << "\n";
}

void outputRelations(std::ostream& output,
	 const std::string& tag,
	 const std::map<std::string, HoI4::Relations>& relations)
{
	for (auto& relation: relations)
	{
		if (relation.first != tag)
		{
			if (relation.second.getRelations() == 0)
			{
				continue;
			}
			output << "add_opinion_modifier = { target = " << relation.first << " modifier = ";
			const auto relationsValue = relation.second.getRelations();
			if (relationsValue < 0)
			{
				output << "negative_";
			}
			else
			{
				output << "positive_";
			}
			output << abs(relationsValue) << " }\n";
		}
	}
	output << "\n";
}


void outputFactions(std::ostream& output,
	 const std::string& tag,
	 const std::optional<HoI4::Faction>& faction,
	 const std::optional<std::string>& possibleLeaderName)
{
	if (faction && (faction->getLeader()->getTag() == tag))
	{
		std::string allianceName;
		if (possibleLeaderName)
		{
			allianceName = "Alliance of " + *possibleLeaderName;
		}
		else
		{
			Log(LogLevel::Warning) << "Could not name alliance";
			allianceName = "faction";
		}
		output << "create_faction = \"" + allianceName + "\"\n";
		for (const auto& factionMember: faction->getMembers())
		{
			output << "add_to_faction = " + factionMember->getTag() + "\n";
		}
	}

	output << '\n';
}

void outputGuaranteedSpherelings(std::ostream& output, const std::vector<std::string>& guaranteed)
{
	for (const auto& guaranteedTag: guaranteed)
	{
		output << "give_guarantee = " + guaranteedTag + "\n";
	}
	output << "\n";
}


void outputIdeas(std::ostream& output,
	 const bool& greatPower,
	 const bool& civilized,
	 const std::set<std::string>& ideas,
	 const std::string& mobilizationLaw,
	 const std::string& economicLaw,
	 const std::string& tradeLaw,
	 const std::string& primaryCulture)
{
	if (greatPower)
	{
		output << "if = {\n";
		output << "\tlimit = { has_dlc = \"Man the Guns\" }\n";
		output << "\tadd_ideas = MTG_naval_treaty_adherent\n";
		output << "}\n";
	}
	output << "add_ideas = {\n";
	if (greatPower)
	{
		output << "\tgreat_power\n";
	}
	if (!civilized)
	{
		output << "\tuncivilized\n";
	}

	for (const auto& idea: ideas)
	{
		output << "\t" << idea << "\n";
	}

	output << "\t" << mobilizationLaw << "\n";
	output << "\t" << economicLaw << "\n";
	output << "\t" << tradeLaw << "\n";
	output << "\tculture_" << primaryCulture << "\n";

	output << "}\n";
}


void outputStability(std::ostream& output, const int& stability)
{
	output << "set_stability = 0." << stability << "\n";
}


void outputWarSupport(std::ostream& output, const int& warSupport)
{
	output << "set_war_support = 0." << warSupport << "\n";
}


void outputCountryLeader(std::ostream& output,
	 const std::string& leaderPortrait,
	 const std::string& leaderName,
	 const std::string& leaderSurname,
	 const std::string& leaderIdeology,
	 const std::string& tag)
{
	if (!leaderName.empty() && !leaderSurname.empty() && !leaderPortrait.empty())
	{
		auto upperFirstName = leaderName;
		std::transform(upperFirstName.begin(), upperFirstName.end(), upperFirstName.begin(), toupper);
		auto upperSurname = leaderSurname;
		std::transform(upperSurname.begin(), upperSurname.end(), upperSurname.begin(), toupper);
		output << "create_country_leader = {\n";
		output << "    name = \"" << leaderName << " " << leaderSurname << "\"\n";
		output << "    desc = \"POLITICS_" << upperFirstName << "_" << upperSurname << "_DESC\"\n";
		output << "    picture = \"" << leaderPortrait << "\"\n";
		output << "    expire = \"1965.1.1\"\n";
		output << "    ideology = " << leaderIdeology << "\n";
		output << "    traits = {\n";
		output << "    }\n";
		output << "}\n";
	}
}


void outputCommanders(std::ostream& output,
	 const std::vector<HoI4::General>& generals,
	 const std::vector<HoI4::Admiral>& admirals)
{
	for (const auto& general: generals)
	{
		output << general;
		output << "\n";
	}
	for (const auto& admiral: admirals)
	{
		output << admiral;
		output << "\n";
	}
}


void outputOOB(const std::vector<HoI4::DivisionTemplateType>& divisionTemplates,
	 const HoI4::Country& theCountry,
	 const Configuration& theConfiguration)
{
	const auto& tag = theCountry.getTag();

	std::ofstream output("output/" + theConfiguration.getOutputName() + "/history/units/" + tag + "_OOB.txt");
	if (!output.is_open())
	{
		throw std::runtime_error(
			 "Could not open output/" + theConfiguration.getOutputName() + "/history/units/" + tag + "_OOB.txt");
	}
	output << "\xEF\xBB\xBF"; // add the BOM to make HoI4 happy

	output << "start_equipment_factor = 0\n";
	for (auto& divisionTemplate: divisionTemplates)
	{
		output << divisionTemplate;
		output << "\n";
	}

	const auto& technologies = theCountry.getTechnologies();
	if (technologies)
	{
		output << "### No BHU air forces ###\n";
		output << "instant_effect = {\n";
		if (technologies->hasTechnology("infantry_weapons1"))
		{
			output << "\tadd_equipment_production = {\n";
			output << "\t\tequipment = {\n";
			output << "\t\t\ttype = infantry_equipment_1\n";
			output << "\t\t\tcreator = \"" << tag << "\"\n";
			output << "\t\t}\n";
			output << "\t\trequested_factories = 5\n";
			output << "\t\tprogress = 0.88\n";
			output << "\t\tefficiency = 100\n";
			output << "\t}\n";
		}
		else
		{
			output << "\tadd_equipment_production = {\n";
			output << "\t\tequipment = {\n";
			output << "\t\t\ttype = infantry_equipment_0\n";
			output << "\t\t\tcreator = \"" << tag << "\"\n";
			output << "\t\t}\n";
			output << "\t\trequested_factories = 5\n";
			output << "\t\tprogress = 0.88\n";
			output << "\t\tefficiency = 100\n";
			output << "\t}\n";
		}
		if (technologies->hasTechnology("gw_artillery"))
		{
			output << "\tadd_equipment_production = {\n";
			output << "\t\tequipment = {\n";
			output << "\t\t\ttype = artillery_equipment_1\n";
			output << "\t\t\tcreator = \"" << tag << "\"\n";
			output << "\t\t}\n";
			output << "\t\trequested_factories = 2\n";
			output << "\t\tprogress = 0.88\n";
			output << "\t\tefficiency = 100\n";
			output << "\t}\n";
		}
		if (technologies->hasTechnology("fighter1"))
		{
			output << "\tadd_equipment_production = {\n";
			output << "\t\tequipment = {\n";
			output << "\t\t\ttype = fighter_equipment_1\n";
			output << "\t\t\tcreator = \"" << tag << "\"\n";
			output << "\t\t}\n";
			output << "\t\trequested_factories = 5\n";
			output << "\t\tprogress = 0.88\n";
			output << "\t\tefficiency = 100\n";
			output << "\t}\n";
		}
		else if (technologies->hasTechnology("early_fighter"))
		{
			output << "\tadd_equipment_production = {\n";
			output << "\t\tequipment = {\n";
			output << "\t\t\ttype = fighter_equipment_0\n";
			output << "\t\t\tcreator = \"" << tag << "\"\n";
			output << "\t\t}\n";
			output << "\t\trequested_factories = 5\n";
			output << "\t\tprogress = 0.88\n";
			output << "\t\tefficiency = 100\n";
			output << "\t}\n";
		}
	}
	output << "\tadd_equipment_production = {\n";
	output << "\t\tequipment = {\n";
	output << "\t\t\ttype = convoy_1\n";
	output << "\t\t\tcreator = \"" << tag << "\"\n";
	output << "\t\t}\n";
	output << "\t\trequested_factories = 10\n";
	output << "\t\tprogress = 0.50\n";
	output << "\t\tamount = 100\n";
	output << "\t}\n";
	output << "}\n";
	output << "units = {\n";
	output << theCountry.getArmy();
	output << "}\n";
	if (auto& planes = theCountry.getPlanes(); !planes.empty())
	{
		output << "air_wings = {\n";
		output << "\t" << *theCountry.getCapitalState() << " = {\n";
		for (auto& plane: planes)
		{
			output << plane;
		}
		output << "\t}\n";
		output << "}\n";
	}
	output.close();

	auto& navies = theCountry.getNavies();
	std::ofstream legacyNavy(
		 "output/" + theConfiguration.getOutputName() + "/history/units/" + tag + "_1936_naval_legacy.txt");
	outputLegacyNavies(navies, *technologies, tag, legacyNavy);

	std::ofstream mtgNavy(
		 "output/" + theConfiguration.getOutputName() + "/history/units/" + tag + "_1936_naval_mtg.txt");
	outputMtgNavies(navies, *technologies, tag, mtgNavy);
}


void outputCommonCountryFile(const HoI4::Country& theCountry, const Configuration& theConfiguration)
{
	const auto& commonCountryFile = theCountry.getCommonCountryFile();
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/common/countries/" +
								Utils::normalizeUTF8Path(commonCountryFile));
	if (!output.is_open())
	{
		throw std::runtime_error("Could not open output/" + theConfiguration.getOutputName() + "/common/countries/" +
										 Utils::normalizeUTF8Path(commonCountryFile));
	}

	auto& graphicalCulture = theCountry.getGraphicalCulture();
	auto& graphicalCulture2d = theCountry.getGraphicalCulture2d();
	if (!graphicalCulture.empty() && !graphicalCulture2d.empty())
	{
		output << "graphical_culture = " << graphicalCulture << "\n";
		output << "graphical_culture_2d = " << graphicalCulture2d << "\n";
	}
	output << "color = { " << theCountry.getColor() << " }\n";

	output.close();
}


void outputAdvisorIdeas(const std::string& tag,
	 const std::set<HoI4::Advisor>& ideologicalAdvisors,
	 const Configuration& theConfiguration)
{
	std::ofstream ideasFile("output/" + theConfiguration.getOutputName() + "/common/ideas/" + tag + ".txt");
	if (!ideasFile.is_open())
	{
		throw std::runtime_error(
			 "Could not open output/" + theConfiguration.getOutputName() + "/common/ideas/" + tag + ".txt");
	}

	ideasFile << "ideas = {\n";
	ideasFile << "\tpolitical_advisor = {\n";
	for (auto& ideologicalAdvisor: ideologicalAdvisors)
	{
		outputAdvisor(ideasFile, tag, ideologicalAdvisor);
	}
	ideasFile << "\t}\n";

	ideasFile << "\ttank_manufacturer = { \n";
	ideasFile << "\t\tdesigner = yes\n";
	ideasFile << "\t\t\n";
	ideasFile << "\t\t" << tag << "_tank_manufacturer = {\n";
	ideasFile << "\t\t\tpicture = generic_tank_manufacturer_1\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\tarmor = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { tank_manufacturer }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t}\n";
	ideasFile << "\tnaval_manufacturer = { \n";
	ideasFile << "\t\tdesigner = yes\n";
	ideasFile << "\t\t" << tag << "_naval_manufacturer = {\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tpicture = generic_naval_manufacturer_1\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\tnaval_equipment = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { naval_manufacturer }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t}\n";
	ideasFile << "\taircraft_manufacturer = { \n";
	ideasFile << "\t\tdesigner = yes\n";
	ideasFile << "\t\t" << tag << "_light_aircraft_manufacturer = {\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tpicture = generic_air_manufacturer_1\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\tair_equipment = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { light_aircraft_manufacturer }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t\t\n";
	ideasFile << "\t\t" << tag << "_medium_aircraft_manufacturer = {\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tpicture = generic_air_manufacturer_3\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\tair_equipment = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { medium_aircraft_manufacturer }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t\t\n";
	ideasFile << "\t\t" << tag << "_heavy_aircraft_manufacturer = {\n";
	ideasFile << "\t\t\tpicture = generic_air_manufacturer_2\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\tair_equipment = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { heavy_aircraft_manufacturer }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t\t\n";
	ideasFile << "\t\t" << tag << "_naval_aircraft_manufacturer = {\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tpicture = generic_naval_manufacturer_2\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\tair_equipment = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { naval_aircraft_manufacturer }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t}\n";
	ideasFile << "\tindustrial_concern = {\n";
	ideasFile << "\t\t" << tag << "_industrial_concern = {\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tpicture = generic_industrial_concern_1\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\tindustry = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { industrial_concern }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t\t" << tag << "_electronics_concern = {\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tpicture = generic_electronics_concern_1\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\telectronics = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { electronics_concern }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t}\n";
	ideasFile << "\tmateriel_manufacturer = {\n";
	ideasFile << "\t\tdesigner = yes\n";
	ideasFile << "\t\t" << tag << "_motorized_equipment_manufacturer = {\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tpicture = generic_motorized_equipment_manufacturer_3\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\tmotorized_equipment = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { motorized_equipment_manufacturer }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t\t\n";
	ideasFile << "\t\t" << tag << "_infantry_equipment_manufacturer = {\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tpicture = generic_infantry_equipment_manufacturer_2\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\tinfantry_weapons = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { infantry_equipment_manufacturer }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t\t\n";
	ideasFile << "\t\t" << tag << "_artillery_manufacturer = {\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tpicture = generic_artillery_manufacturer_2\n";
	ideasFile << "\t\t\tallowed = {\n";
	ideasFile << "\t\t\t\toriginal_tag = " << tag << "\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tcost = 150\n";
	ideasFile << "\t\t\tremoval_cost = 10\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\tresearch_bonus = {\n";
	ideasFile << "\t\t\t\tartillery = 0.10\n";
	ideasFile << "\t\t\t}\n";
	ideasFile << "\t\t\t\n";
	ideasFile << "\t\t\ttraits = { artillery_manufacturer }\n";
	ideasFile << "\t\t}\n";
	ideasFile << "\t}\n";

	ideasFile << "}\n";
}


void HoI4::reportIndustry(std::ostream& out, const Country& theCountry)
{
	if (theCountry.hasProvinces())
	{
		out << theCountry.getTag() << ',';
		out << theCountry.getMilitaryFactories() << ',';
		out << theCountry.getCivilianFactories() << ',';
		out << theCountry.getDockyards() << ',';
		out << theCountry.getMilitaryFactories() + theCountry.getCivilianFactories() + theCountry.getDockyards() << '\n';
	}
}