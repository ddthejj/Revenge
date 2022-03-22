#pragma once

typedef unsigned long long FlagBits;

#pragma region Flag Enums

enum class FLAGS_MAIN_QUEST_PROGRESS : FlagBits
{
	TEST1 = 1 << 0,
	TEST2 = 1 << 1,
	TEST3 = 1 << 2,
};

enum class FLAGS_MAIN_QUEST_DECISIONS : FlagBits
{
	TEST1 = 1 << 0,
	TEST2 = 1 << 1,
	TEST3 = 1 << 2,
};

enum class FLAGS_SIDE_QUEST_PROGRESS : FlagBits
{
	TEST1 = 1 << 0,
	TEST2 = 1 << 1,
	TEST3 = 1 << 2,
};

enum class FLAGS_SIDE_QUEST_DECISIONS : FlagBits
{
	TEST1 = 1 << 0,
	TEST2 = 1 << 1,
	TEST3 = 1 << 2,
};

enum class FLAGS_ITEMS_COLLECTED : FlagBits
{
	TEST1 = 1 << 0,
	TEST2 = 1 << 1,
	TEST3 = 1 << 2,
};

enum class FLAGS_CONVERSATION_CHOICES : FlagBits
{
	TEST1 = 1 << 0,
	TEST2 = 1 << 1,
	TEST3 = 1 << 2,
};

enum class FLAGS_DEFEATED_ENEMIES : FlagBits
{
	TEST1 = 1 << 0,
	TEST2 = 1 << 1,
	TEST3 = 1 << 2,
};

enum class FLAGS_MISC : FlagBits
{
	TEST1 = 1 << 0,
	TEST2 = 1 << 1,
	TEST3 = 1 << 2,
};

#pragma endregion

class SaveManager
{
	friend class SaveWriter; 

	static int whichSave;
	static FlagBits mainQuestProgress, mainQuestDecisions, sideQuestProgress, sideQuestDecisions, itemsCollected, conversationChoices, defeatedEnemies, misc;

public:

#pragma region Flag Set / Get

	static void SetMainQuestProgress(FLAGS_MAIN_QUEST_PROGRESS flag, bool value);
	static void SetMainQuestDecisions(FLAGS_MAIN_QUEST_DECISIONS flag, bool value);
	static void SetSideQuestProgress(FLAGS_SIDE_QUEST_PROGRESS flag, bool value);
	static void SetSideQuestDecisions(FLAGS_SIDE_QUEST_DECISIONS flag, bool value);
	static void SetItemsCollected(FLAGS_ITEMS_COLLECTED flag, bool value);
	static void SetConversationChoices(FLAGS_CONVERSATION_CHOICES flag, bool value);
	static void SetDefeatedEnemies(FLAGS_DEFEATED_ENEMIES flag, bool value);
	static void SetMisc(FLAGS_MISC flag, bool value);
	
	static bool GetMainQuestProgress(FLAGS_MAIN_QUEST_PROGRESS flag);
	static bool GetMainQuestDecisions(FLAGS_MAIN_QUEST_DECISIONS flag);
	static bool GetSideQuestProgress(FLAGS_SIDE_QUEST_PROGRESS flag);
	static bool GetSideQuestDecisions(FLAGS_SIDE_QUEST_DECISIONS flag);
	static bool GetItemsCollected(FLAGS_ITEMS_COLLECTED flag);
	static bool GetConversationChoices(FLAGS_CONVERSATION_CHOICES flag);
	static bool GetDefeatedEnemies(FLAGS_DEFEATED_ENEMIES flag);
	static bool GetMisc(FLAGS_MISC flag);

#pragma endregion

	static void OpenSave(int save);
	static void Save();
	static void CloseSave();

};

