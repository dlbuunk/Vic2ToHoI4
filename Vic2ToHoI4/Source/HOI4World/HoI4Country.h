#ifndef HOI4_COUNTRY_H_
#define HOI4_COUNTRY_H_



#include "../Color.h"
#include "../V2World/Party.h"
#include "Date.h"
#include "Diplomacy/Faction.h"
#include "Diplomacy/HoI4Relations.h"
#include "Diplomacy/HoI4War.h"
#include "HoI4FocusTree.h"
#include "Ideologies/Ideology.h"
#include "Leaders/Admiral.h"
#include "Leaders/General.h"
#include "Military/Airplane.h"
#include "Military/HoI4Army.h"
#include "Navies/Navies.h"
#include "Navies/NavyNames.h"
#include "ShipTypes/ShipVariants.h"
#include "Technologies.h"
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>



namespace Vic2
{
class Country;
class World;
class Localisations;
} // namespace Vic2


namespace HoI4
{
class Ideologies;
class CoastalProvinces;
class MtgUnitMappings;
class ProvinceDefinitions;
class Names;
class ShipVariant;
class State;
class UnitMappings;
} // namespace HoI4


namespace mappers
{
class FlagsToIdeasMapper;
class ProvinceMapper;
class techMapper;
} // namespace mappers


class CountryMapper;
class governmentMapper;
class graphicsMapper;
class Date;



namespace HoI4
{

class Country
{
  public:
	explicit Country(std::string tag,
		 const Vic2::Country* srcCountry,
		 Names& names,
		 graphicsMapper& theGraphics,
		 const CountryMapper& countryMap,
		 const mappers::FlagsToIdeasMapper& flagsToIdeasMapper,
		 Localisation& hoi4Localisations);
	Country() = delete;
	Country(const Country&) = delete;
	Country& operator=(const Country&) = delete;
	Country(Country&&) = delete;
	Country& operator=(Country&&) = delete;
	~Country() = default;

	void determineCapitalFromVic2(const mappers::ProvinceMapper& theProvinceMapper,
		 const std::map<int, int>& provinceToStateIDMap,
		 const std::map<int, State>& allStates);
	void setGovernmentToExistingIdeology(const std::set<std::string>& majorIdeologies,
		 const Ideologies& ideologies,
		 const governmentMapper& governmentMap,
		 bool debug);
	void addLeader(Names& names, graphicsMapper& theGraphics);
	void convertGovernment(const Vic2::World& sourceWorld,
		 const governmentMapper& governmentMap,
		 const Vic2::Localisations& vic2Localisations,
		 Localisation& hoi4Localisations,
		 bool debug);
	void convertParties(const std::set<std::string>& majorIdeologies,
		 const governmentMapper& governmentMap,
		 const Vic2::Localisations& vic2Localisations,
		 Localisation& hoi4Localisations);
	void convertIdeologySupport(const std::set<std::string>& majorIdeologies, const governmentMapper& governmentMap);
	void determineShipVariants(const PossibleShipVariants& possibleVariants);
	void convertNavies(const UnitMappings& unitMap,
		 const MtgUnitMappings& mtgUnitMap,
		 const std::map<int, int>& provinceToStateIDMap,
		 const std::map<int, State>& allStates,
		 const ProvinceDefinitions& provinceDefinitions,
		 const mappers::ProvinceMapper& provinceMapper);
	void convertConvoys(const UnitMappings& unitMap);
	void convertAirForce(const UnitMappings& unitMap);
	void convertArmies(const militaryMappings& theMilitaryMappings,
		 const HoI4::States& theStates,
		 const mappers::ProvinceMapper& provinceMapper,
		 const Configuration& theConfiguration);
	void convertTechnology(const mappers::techMapper& theTechMapper);
	void addState(const State& state);
	void calculateIndustry(const std::map<int, State>& allStates);
	void addGenericFocusTree(const std::set<std::string>& majorIdeologies);
	void adjustResearchFocuses() const;

	void setSphereLeader(const std::string& SphereLeader) { sphereLeader = SphereLeader; }
	void setFaction(const std::shared_ptr<const Faction>& newFaction) { faction = newFaction; }
	void giveNationalFocus(std::unique_ptr<HoI4FocusTree>& NF) { nationalFocus = std::move(NF); }
	void setGreatPower() { greatPower = true; }
	void setPuppetMaster(const std::string& _master) { puppetMaster = _master; }
	void addPuppet(const std::string& countryTag) { puppets.insert(countryTag); }

	[[nodiscard]] std::optional<HoI4::Relations> getRelations(const std::string& withWhom) const;
	[[nodiscard]] double getStrengthOverTime(const double& years) const;
	static double getMilitaryStrength();
	[[nodiscard]] float getNavalStrength() const;
	[[nodiscard]] double getEconomicStrength(const double& years) const;
	[[nodiscard]] bool areElectionsAllowed() const;
	[[nodiscard]] std::optional<Faction> getFaction() const;
	[[nodiscard]] std::optional<HoI4FocusTree> getNationalFocus() const;

	[[nodiscard]] const std::string& getTag() const { return tag; }
	[[nodiscard]] const Vic2::Country& getSourceCountry() const { return sourceCountry; }
	[[nodiscard]] const std::string& getFilename() const { return filename; }
	[[nodiscard]] const std::string& getCommonCountryFile() const { return commonCountryFile; }
	[[nodiscard]] bool isHuman() const { return human; }

	[[nodiscard]] const ConverterColor::Color& getColor() const { return color; }
	[[nodiscard]] const std::string& getGraphicalCulture() const { return graphicalCulture; }
	[[nodiscard]] const std::string& getGraphicalCulture2d() const { return graphicalCulture2d; }
	[[nodiscard]] const auto& getCommunistAdvisorPortrait() const { return communistAdvisorPortrait; }
	[[nodiscard]] const auto& getDemocraticAdvisorPortrait() const { return democraticAdvisorPortrait; }
	[[nodiscard]] const auto& getNeutralityAdvisorPortrait() const { return neutralityAdvisorPortrait; }
	[[nodiscard]] const auto& getAbsolutistAdvisorPortrait() const { return absolutistAdvisorPortrait; }
	[[nodiscard]] const auto& getRadicalAdvisorPortrait() const { return radicalAdvisorPortrait; }
	[[nodiscard]] const auto& getFascistAdvisorPortrait() const { return fascistAdvisorPortrait; }

	[[nodiscard]] bool hasProvinces() const { return !provinces.empty(); }
	[[nodiscard]] const std::set<int>& getProvinces() const { return provinces; }
	[[nodiscard]] const std::set<int>& getStates() const { return states; }
	[[nodiscard]] std::optional<int> getCapitalState() const { return capitalState; }
	[[nodiscard]] std::optional<int> getCapitalProvince() const { return capitalProvince; }

	[[nodiscard]] const std::string& getGovernmentIdeology() const { return governmentIdeology; }
	[[nodiscard]] const std::string& getLeaderIdeology() const { return leaderIdeology; }
	[[nodiscard]] const Vic2::Party& getRulingParty() const { return rulingParty; }
	[[nodiscard]] const std::set<Vic2::Party>& getParties() const { return parties; }
	[[nodiscard]] const std::map<std::string, int>& getIdeologySupport() const { return ideologySupport; }
	[[nodiscard]] const date& getLastElection() const { return lastElection; }
	[[nodiscard]] int getStability() const { return stability; }
	[[nodiscard]] int getWarSupport() const { return warSupport; }
	[[nodiscard]] const std::string& getMobilizationLaw() const { return mobilizationLaw; }
	[[nodiscard]] const std::string& getEconomicLaw() const { return economicLaw; }
	[[nodiscard]] const std::string& getTradeLaw() const { return tradeLaw; }
	[[nodiscard]] const auto& getLeaderPortrait() const { return leaderPortrait; }
	[[nodiscard]] const auto& getLeaderName() const { return leaderName; }
	[[nodiscard]] const auto& getLeaderSurname() const { return leaderSurname; }

	[[nodiscard]] auto getTechnologyCount() const
	{
		if (theTechnologies)
		{
			return theTechnologies->getTechnologyCount();
		}
		else
		{
			return static_cast<size_t>(0);
		}
	}
	[[nodiscard]] const std::optional<technologies>& getTechnologies() const { return theTechnologies; }
	[[nodiscard]] const std::set<std::string>& getIdeas() const { return ideas; }

	[[nodiscard]] double getMilitaryFactories() const { return militaryFactories; }
	[[nodiscard]] double getCivilianFactories() const { return civilianFactories; }
	[[nodiscard]] double getDockyards() const { return dockyards; }

	[[nodiscard]] const Army& getArmy() const { return theArmy; }
	[[nodiscard]] const ShipVariants& getTheShipVariants() const { return *theShipVariants; }
	[[nodiscard]] const Navies& getNavies() const { return *theNavies; }
	[[nodiscard]] const auto& getNavyNames() const { return navyNames; }
	[[nodiscard]] int getConvoys() const { return convoys; }
	[[nodiscard]] const std::vector<Airplane>& getPlanes() const { return planes; }
	[[nodiscard]] const std::map<std::string, unsigned int>& getEquipmentStockpile() const { return equipmentStockpile; }
	[[nodiscard]] const std::vector<General>& getGenerals() const { return generals; }
	[[nodiscard]] const std::vector<Admiral>& getAdmirals() const { return admirals; }

	[[nodiscard]] const std::map<std::string, HoI4::Relations>& getRelations() const { return relations; }
	[[nodiscard]] const std::vector<War>& getWars() const { return wars; }
	[[nodiscard]] double getThreat() const { return threat; }
	[[nodiscard]] bool isInFaction() const { return faction.operator bool(); }
	[[nodiscard]] const std::string& getSphereLeader() const { return sphereLeader; }
	[[nodiscard]] const std::set<std::string>& getAllies() const { return allies; }
	[[nodiscard]] const std::set<std::string>& getPuppets() const { return puppets; }
	[[nodiscard]] const std::string& getPuppetMaster() const { return puppetMaster; }
	[[nodiscard]] bool isGreatPower() const { return greatPower; }
	[[nodiscard]] bool isCivilized() const { return civilized; }

	std::set<std::string>& editAllies() { return allies; }

	void addGPInfluence(std::string GPTag, int influenceValue) { GPInfluences.insert({GPTag, influenceValue}); }
	[[nodiscard]] const std::map<std::string, double>& getSpherelings() const { return spherelings; }
	void addSphereling(std::string sphereling) { spherelings.insert(make_pair(sphereling, 0.5)); }
	void setSpherelingAutonomy(std::string sphereling, double autonomy)
	{
		spherelings.find(sphereling)->second = autonomy;
	}
	double calculateInfluenceFactor();
	const auto& getGuaranteed() const { return guaranteed; }
	void addGuaranteed(std::string guaranteedTag) { guaranteed.push_back(guaranteedTag); }

  private:
	void determineFilename();
	void initIdeas(Names& names, Localisation& hoi4Localisations) const;
	void convertLaws();
	void convertLeaders(const graphicsMapper& theGraphics);
	void convertRelations(const CountryMapper& countryMap);
	void convertWars(const Vic2::Country& sourceCountry, const CountryMapper& countryMap);

	bool attemptToPutCapitalInPreferredNonWastelandOwned(const mappers::ProvinceMapper& theProvinceMapper,
		 const std::map<int, int>& provinceToStateIDMap,
		 const std::map<int, State>& allStates);
	bool attemptToPutCapitalInNonWastelandOwned(const std::map<int, State>& allStates);
	bool attemptToPutCapitalInPreferredWastelandOwned(const mappers::ProvinceMapper& theProvinceMapper,
		 const std::map<int, int>& provinceToStateIDMap,
		 const std::map<int, State>& allStates);
	bool attemptToPutCapitalInAnyOwned(const std::map<int, State>& allStates);
	bool attemptToPutCapitalInPreferredNonWastelandCored(const mappers::ProvinceMapper& theProvinceMapper,
		 const std::map<int, int>& provinceToStateIDMap,
		 const std::map<int, State>& allStates);
	bool attemptToPutCapitalInAnyNonWastelandCored(const std::map<int, State>& allStates);
	bool attemptToPutCapitalInPreferredWastelandCored(const mappers::ProvinceMapper& theProvinceMapper,
		 const std::map<int, int>& provinceToStateIDMap,
		 const std::map<int, State>& allStates);
	bool attemptToPutCapitalInAnyCored(const std::map<int, State>& allStates);

	std::string tag;
	const Vic2::Country& sourceCountry;
	std::string filename;
	std::string commonCountryFile;
	bool human = false;

	ConverterColor::Color color;
	std::string graphicalCulture = "western_european_gfx";
	std::string graphicalCulture2d = "western_european_2d";
	std::string communistAdvisorPortrait;
	std::string democraticAdvisorPortrait;
	std::string neutralityAdvisorPortrait;
	std::string absolutistAdvisorPortrait;
	std::string radicalAdvisorPortrait;
	std::string fascistAdvisorPortrait;

	std::set<int> provinces;
	std::set<int> states;
	std::optional<int> capitalState;
	std::optional<int> capitalProvince;

	std::string governmentIdeology = "neutrality";
	std::string leaderIdeology = "conservatism_neutral";
	Vic2::Party rulingParty;
	std::set<Vic2::Party> parties;
	std::map<std::string, int> ideologySupport{std::make_pair("neutrality", 100)};
	date lastElection;
	int stability = 50;
	int warSupport = 50;
	std::string mobilizationLaw = "volunteer_only";
	std::string economicLaw = "civilian_economy";
	std::string tradeLaw = "export_focus";

	std::string leaderPortrait;
	std::string leaderName;
	std::string leaderSurname;

	std::optional<technologies> theTechnologies;
	std::set<std::string> ideas;
	std::unique_ptr<HoI4FocusTree> nationalFocus;

	double militaryFactories = 0.0;
	double civilianFactories = 0.0;
	double dockyards = 0.0;

	Army theArmy;
	std::unique_ptr<ShipVariants> theShipVariants;
	std::unique_ptr<Navies> theNavies;
	NavyNames navyNames;
	int convoys = 0;
	std::vector<Airplane> planes;
	std::map<std::string, unsigned int> equipmentStockpile;
	std::vector<General> generals;
	std::vector<Admiral> admirals;

	std::map<std::string, HoI4::Relations> relations;
	std::vector<War> wars;
	double threat = 0.0;
	std::shared_ptr<const Faction> faction;
	std::string sphereLeader;
	std::set<std::string> allies;
	std::set<std::string> puppets;
	std::string puppetMaster;
	bool greatPower = false;
	bool civilized = false;

	std::map<std::string, int> GPInfluences;
	std::map<std::string, double> spherelings;
	std::vector<std::string> guaranteed;
};

} // namespace HoI4



#endif // HOI4_COUNTRY_H_