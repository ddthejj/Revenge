#include "defines.h"
#include "SaveManager.h"
#include "FileReader.h"

int SaveManager::whichSave;
FlagBits SaveManager::mainQuestProgress, SaveManager::mainQuestDecisions, SaveManager::sideQuestProgress, SaveManager::sideQuestDecisions, SaveManager::itemsCollected, SaveManager::conversationChoices, SaveManager::defeatedEnemies, SaveManager::misc;

void SaveManager::SetMainQuestProgress(FLAGS_MAIN_QUEST_PROGRESS flag, bool value)
{
	mainQuestProgress = ((unsigned long long)flag * value ? 0 : 1) | mainQuestProgress;
}

void SaveManager::SetMainQuestDecisions(FLAGS_MAIN_QUEST_DECISIONS flag, bool value)
{
	mainQuestDecisions = ((unsigned long long)flag * value ? 0 : 1) | mainQuestDecisions;
}

void SaveManager::SetSideQuestProgress(FLAGS_SIDE_QUEST_PROGRESS flag, bool value)
{
	sideQuestProgress = ((unsigned long long)flag * value ? 0 : 1) | sideQuestProgress;
}

void SaveManager::SetSideQuestDecisions(FLAGS_SIDE_QUEST_DECISIONS flag, bool value)
{
	sideQuestDecisions = ((unsigned long long)flag * value ? 0 : 1) | sideQuestDecisions;
}

void SaveManager::SetItemsCollected(FLAGS_ITEMS_COLLECTED flag, bool value)
{
	itemsCollected = ((unsigned long long)flag * value ? 0 : 1) | itemsCollected;
}

void SaveManager::SetConversationChoices(FLAGS_CONVERSATION_CHOICES flag, bool value)
{
	conversationChoices = ((unsigned long long)flag * value ? 0 : 1) | conversationChoices;
}

void SaveManager::SetDefeatedEnemies(FLAGS_DEFEATED_ENEMIES flag, bool value)
{
	defeatedEnemies = ((unsigned long long)flag * value ? 0 : 1) | defeatedEnemies;
}

void SaveManager::SetMisc(FLAGS_MISC flag, bool value)
{
	misc = ((unsigned long long)flag * value ? 0 : 1) | misc;
}

bool SaveManager::GetMainQuestProgress(FLAGS_MAIN_QUEST_PROGRESS flag)
{
	return (unsigned long long)flag & mainQuestProgress;
}

bool SaveManager::GetMainQuestDecisions(FLAGS_MAIN_QUEST_DECISIONS flag)
{
	return (unsigned long long)flag & mainQuestDecisions;
}

bool SaveManager::GetSideQuestProgress(FLAGS_SIDE_QUEST_PROGRESS flag)
{
	return (unsigned long long)flag & sideQuestProgress;
}

bool SaveManager::GetSideQuestDecisions(FLAGS_SIDE_QUEST_DECISIONS flag)
{
	return (unsigned long long)flag & sideQuestDecisions;
}

bool SaveManager::GetItemsCollected(FLAGS_ITEMS_COLLECTED flag)
{
	return (unsigned long long)flag & itemsCollected;
}

bool SaveManager::GetConversationChoices(FLAGS_CONVERSATION_CHOICES flag)
{
	return (unsigned long long)flag & conversationChoices;
}

bool SaveManager::GetDefeatedEnemies(FLAGS_DEFEATED_ENEMIES flag)
{
	return (unsigned long long)flag & defeatedEnemies;
}

bool SaveManager::GetMisc(FLAGS_MISC flag)
{
	return (unsigned long long)flag & misc;
}

void SaveManager::OpenSave(int save)
{
	SaveReader saveReader;

	std::string savePath = "../Save/save";
	char buffer[8];
	_itoa_s(save, buffer, 10);
	buffer[7] = '\0';
	savePath.append(buffer);
	savePath.append(".txt");

	saveReader.Open(savePath.c_str());

	std::vector<unsigned long long> values = saveReader.GetFlags();

	mainQuestProgress = values[0];
	mainQuestDecisions = values[1];
	sideQuestProgress = values[2];
	sideQuestDecisions = values[3];
	itemsCollected = values[4];
	conversationChoices = values[5];
	defeatedEnemies = values[6];
	misc = values[7];
}

void SaveManager::Save()
{
	SaveWriter saveWriter;

	saveWriter.WriteSave("../Save/save0.txt");
}

void SaveManager::CloseSave()
{
	Save();
	// reset save
	whichSave = 0;
	// reset flags
	mainQuestProgress = 0;
	mainQuestDecisions = 0;
	sideQuestProgress = 0;
	sideQuestDecisions = 0;
	itemsCollected = 0;
	conversationChoices = 0;
	defeatedEnemies = 0;
	misc = 0;
}
