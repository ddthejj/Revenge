#pragma once

class CharacterData;
class WeaponData;

enum CHARACTER_LIST
{
	// main party
	ARCHER,
	CLARA,
	SEVAN,
	KIPII,
	VALERIYA,
	AWHA,
	ABORI,
	JEONG,
	JYOTSANA,
	RELIA,
	SKYTEN,
	REINOLD,
	// alpha squad
	DELLSEOC,
	KAYA,
	AARON,
	GEIR,
	RAGNA,
	ISANA,
	URUN,
	GUNHILD,
	JI_SU,
	RAHUL,
	// bear claws
	VIKT,
	WYSTAN,
	EADWIN,
	LUKASHA,
	ALLARA,
	LEAFYTH,
	ESTHER,
	RODYA,
	KORAE,
	// kings
	ATHELRIC,
	STANAS,
	// envoys
	FIRE_ENVOY,
	WATER_ENVOY,
	WARD_ENVOY,
	VENOM_ENVOY,
	LIFE_ENVOY,
	LIGHT_ENVOY,
	EARTH_ENVOY,
	MAGNET_ENVOY,
	// other bosses
	KATUSHKI,
	MIND_VIRUS,
	// other
	SHAULTAN,
	MAURA,
	KAYON,
	ZEPHYR,
	BYRESS,
	NIKOLI,
};

enum WEAPON_LIST
{
	// main party
	RETZVA,
	TEKITH,
	CAILRA,
	GALRICE,
	IMPRINTS, // special value for imprinters
	KOHARA_ATUA,
	FIT_RANA,
	TALCHUL,
	KAMEE,
	FREAMORTH,
	ANTAGA_INTEMER,
	ANDAMAUL,
	// alpha squad
	PH_DELLSEOC,
	PH_KAYA,
	PH_AARON,
	PH_GEIR,
	PH_ISANA,
	PH_URUN,
	PH_GUNHILD,
	PH_JI_SU,
	PH_RAHUL,
	// bear claws
	PH_VIKT,
	PH_WYSTAN,
	PH_EADWIN,
	PH_ALLARA,
	PH_LEAFYTH,
	PH_ESTHER,
	PH_KORAE,
	// kings
	PH_ATHELRIC,
	PH_STANAS,
	// other
	PH_SHAULTAN,
	PH_MAURA,
	PH_KAYON,
	PH_ZEPHYR,
	PH_BYRESS,
	PH_NIKOLI,
};

class CharacterManager
{
	static std::vector<CharacterData*> characterDataList;
	static std::vector<WeaponData*> weaponDataList;

public:

	static void Init();
	static void Clean();

	static CharacterData* GetCharacterData(CHARACTER_LIST index);
	static WeaponData* GetWeaponData(WEAPON_LIST index);
};

