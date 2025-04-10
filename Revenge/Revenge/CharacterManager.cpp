#include "defines.h"
#include "CharacterManager.h"
#include "Character.h"
#include "Weapon.h"

std::vector<CharacterData*> CharacterManager::characterDataList = std::vector<CharacterData*>();
std::vector<WeaponData*> CharacterManager::weaponDataList = std::vector<WeaponData*>();


void CharacterManager::Init()
{
	// weapons
	weaponDataList.push_back(new WeaponData("Retzva", 1, 1, 1, MAGIC_TYPE::FIRE, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Tekith", 1, 1, 1, MAGIC_TYPE::WATER, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Cailra", 1, 1, 1, MAGIC_TYPE::MAGNET, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Galrice", 1, 1, 1, MAGIC_TYPE::MAGNET, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Imprints", 1, 1, 1, MAGIC_TYPE::ALL, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Kohara-Atua", 1, 1, 1, (MAGIC_TYPE)((unsigned int)MAGIC_TYPE::WATER | (unsigned int)MAGIC_TYPE::WARD), std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Fit-Rana", 1, 1, 1, (MAGIC_TYPE)((unsigned int)MAGIC_TYPE::LIFE | (unsigned int)MAGIC_TYPE::WARD), std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Talchul", 1, 1, 1, MAGIC_TYPE::EARTH, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Kamee", 1, 1, 1, MAGIC_TYPE::MIND, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Freamorth", 1, 1, 1, MAGIC_TYPE::VENOM, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Antaga Intemer", 1, 1, 1, MAGIC_TYPE::LIFE, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("Andamaul", 1, 1, 1, MAGIC_TYPE::DEATH, std::vector<Ability*>()));
	// alpha squad																																	
	weaponDataList.push_back(new WeaponData("PH_DELLSEOC", 1, 1, 1, MAGIC_TYPE::DEATH, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_KAYA", 1, 1, 1, MAGIC_TYPE::LIGHT, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_AARON", 1, 1, 1, (MAGIC_TYPE)((unsigned int)MAGIC_TYPE::WATER | (unsigned int)MAGIC_TYPE::LIGHT), std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_GEIR", 1, 1, 1, MAGIC_TYPE::EARTH, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_ISANA", 1, 1, 1, MAGIC_TYPE::FIRE, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_URUN", 1, 1, 1, MAGIC_TYPE::LIFE, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_GUNHILD", 1, 1, 1, MAGIC_TYPE::WARD, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_JI_SU", 1, 1, 1, MAGIC_TYPE::VENOM, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_RAHUL", 1, 1, 1, MAGIC_TYPE::EARTH, std::vector<Ability*>()));
	// bear claw																																	
	weaponDataList.push_back(new WeaponData("PH_VIKT", 1, 1, 1, MAGIC_TYPE::EARTH, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_WYSTAN", 1, 1, 1, MAGIC_TYPE::MIND, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_EADWIN", 1, 1, 1, MAGIC_TYPE::LIGHT, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_ALLARA", 1, 1, 1, MAGIC_TYPE::VENOM, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_LEAFYTH", 1, 1, 1, MAGIC_TYPE::WATER, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_ESTHER", 1, 1, 1, MAGIC_TYPE::DEATH, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_KORAE", 1, 1, 1, MAGIC_TYPE::FIRE, std::vector<Ability*>()));
	// kings																																		
	weaponDataList.push_back(new WeaponData("PH_ATHELRIC", 1, 1, 1, MAGIC_TYPE::WARD, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_STANAS", 1, 1, 1, MAGIC_TYPE::MAGNET, std::vector<Ability*>()));
	//other 																																		
	weaponDataList.push_back(new WeaponData("PH_SHAULTAN", 1, 1, 1, MAGIC_TYPE::EARTH, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_MAURA", 1, 1, 1, MAGIC_TYPE::MAGNET, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_KAYON", 1, 1, 1, MAGIC_TYPE::VENOM, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_ZEPHYR", 1, 1, 1, MAGIC_TYPE::DEATH, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("PH_BYRESS", 1, 1, 1, MAGIC_TYPE::WATER, std::vector<Ability*>()));
	weaponDataList.push_back(new WeaponData("RPH_NIKOLIetzva", 1, 1, 1, MAGIC_TYPE::DEATH, std::vector<Ability*>()));

	// characters
	// main party
	characterDataList.push_back(new CharacterData("Archer", "Alwestow", 67, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MAGNET, GetWeaponData(WEAPON_LIST::RETZVA), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Clara", "Lurar", 7, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::LIGHT, GetWeaponData(WEAPON_LIST::TEKITH), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Sevan", "Lukvat", 41, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::LIFE, GetWeaponData(WEAPON_LIST::CAILRA), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Kipii", "Phanx", 94, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::VENOM, GetWeaponData(WEAPON_LIST::GALRICE), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Valeriya", "", 48, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::EARTH, GetWeaponData(WEAPON_LIST::IMPRINTS), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Awha", "Bann", 14, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::FIRE, GetWeaponData(WEAPON_LIST::KOHARA_ATUA), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Abori", "Uuru", 87, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MIND, GetWeaponData(WEAPON_LIST::FIT_RANA), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Mu", "Jeong", 72, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MIND, GetWeaponData(WEAPON_LIST::TALCHUL), std::vector<Ability*>(), "", true));
	characterDataList.push_back(new CharacterData("Jyotsana", "Istara", 53, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::DEATH, GetWeaponData(WEAPON_LIST::KAMEE), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Relia", "Bewerigend", 60, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::FIRE, GetWeaponData(WEAPON_LIST::FREAMORTH), std::vector<Ability*>(), "Lia"));
	characterDataList.push_back(new CharacterData("Skyten", "Alwestow", 23, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::WATER, GetWeaponData(WEAPON_LIST::ANTAGA_INTEMER), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Reinold", "Forbrasa", 35, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::WARD, GetWeaponData(WEAPON_LIST::ANDAMAUL), std::vector<Ability*>()));
	// alpha squad
	characterDataList.push_back(new CharacterData("Dellseoc", "Bewerigend", 41, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::EARTH, GetWeaponData(WEAPON_LIST::PH_DELLSEOC), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Kaya", "Ayverto", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MIND, GetWeaponData(WEAPON_LIST::PH_KAYA), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Aaron", "Irull", 100, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::VENOM, GetWeaponData(WEAPON_LIST::PH_AARON), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Geir", "Svar", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::LIFE, GetWeaponData(WEAPON_LIST::PH_GEIR), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Ragna", "Skryty", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::DEATH, GetWeaponData(WEAPON_LIST::IMPRINTS), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Isana", "Ehwa", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::WARD, GetWeaponData(WEAPON_LIST::PH_ISANA), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Urun", "Ehwa", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MAGNET, GetWeaponData(WEAPON_LIST::PH_URUN), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Gunhild", "Djuprod", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::LIGHT, GetWeaponData(WEAPON_LIST::PH_GUNHILD), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Chi", "Ji-Su", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::WATER, GetWeaponData(WEAPON_LIST::PH_JI_SU), std::vector<Ability*>(), "", true));
	characterDataList.push_back(new CharacterData("Rahul", "Chaah", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MAGNET, GetWeaponData(WEAPON_LIST::PH_RAHUL), std::vector<Ability*>()));
	// bear claws
	characterDataList.push_back(new CharacterData("Vikt", "Grabjorn", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::WARD, GetWeaponData(WEAPON_LIST::PH_VIKT), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Wystan", "Purfreo", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::WATER, GetWeaponData(WEAPON_LIST::PH_WYSTAN), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Eadwin", "Raedend", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MAGNET, GetWeaponData(WEAPON_LIST::PH_EADWIN), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Lukasha", "Konets", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MIND, GetWeaponData(WEAPON_LIST::IMPRINTS), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Allara", "Ysafa", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::LIFE, GetWeaponData(WEAPON_LIST::PH_ALLARA), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Leafyth", "Awedan", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::VENOM, GetWeaponData(WEAPON_LIST::PH_LEAFYTH), std::vector<Ability*>(), "Leaf"));
	characterDataList.push_back(new CharacterData("Esther", "Weman", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::FIRE, GetWeaponData(WEAPON_LIST::PH_ESTHER), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Rodya", "Vpered", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::LIGHT, GetWeaponData(WEAPON_LIST::IMPRINTS), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Korae", "Faegryre", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::DEATH, GetWeaponData(WEAPON_LIST::PH_KORAE), std::vector<Ability*>()));
	// kings
	characterDataList.push_back(new CharacterData("Athelric", "Byrthodn", 90, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::EARTH, GetWeaponData(WEAPON_LIST::PH_ATHELRIC), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Stanas", "Bewerigend", 10, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::FIRE, GetWeaponData(WEAPON_LIST::PH_STANAS), std::vector<Ability*>()));
	// envoys
	characterDataList.push_back(new CharacterData("Fire", "Envoy", 0, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::FIRE, nullptr, std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Water", "Envoy", 0, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::WATER, nullptr, std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Ward", "Envoy", 0, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::WARD, nullptr, std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Venom", "Envoy", 0, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::VENOM, nullptr, std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Life", "Envoy", 0, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::LIFE, nullptr, std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Light", "Envoy", 0, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::LIGHT, nullptr, std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Earth", "Envoy", 0, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::EARTH, nullptr, std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Magnet", "Envoy", 0, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MAGNET, nullptr, std::vector<Ability*>()));
	// other bosses
	characterDataList.push_back(new CharacterData("Katushki", "", 100, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::VENOM, nullptr, std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Mind Virus", "", 100, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MIND, nullptr, std::vector<Ability*>()));
	// other 
	characterDataList.push_back(new CharacterData("Shaultan", "", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MIND, GetWeaponData(WEAPON_LIST::PH_SHAULTAN), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Maura", "", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MIND, GetWeaponData(WEAPON_LIST::PH_MAURA), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Kayon", "Black", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::EARTH, GetWeaponData(WEAPON_LIST::PH_KAYON), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Zephyr", "Bann", -1, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::WATER, GetWeaponData(WEAPON_LIST::PH_ZEPHYR), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Byress", "Alwestow", 66, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::MAGNET, GetWeaponData(WEAPON_LIST::PH_BYRESS), std::vector<Ability*>()));
	characterDataList.push_back(new CharacterData("Nikoli", "", 0, 1, 1, 1, 1, 1, 1, 1, MAGIC_TYPE::DEATH, GetWeaponData(WEAPON_LIST::PH_NIKOLI), std::vector<Ability*>()));
}

void CharacterManager::Clean()
{
	for (auto it = characterDataList.begin(); it != characterDataList.end(); it++)
	{
		SafeDelete(*it);
	}
	for (auto it = weaponDataList.begin(); it != weaponDataList.end(); it++)
	{
		SafeDelete(*it);
	}
}

CharacterData* CharacterManager::GetCharacterData(CHARACTER_LIST index)
{
	return characterDataList[index];
}

WeaponData* CharacterManager::GetWeaponData(WEAPON_LIST index)
{
	return nullptr;
}
