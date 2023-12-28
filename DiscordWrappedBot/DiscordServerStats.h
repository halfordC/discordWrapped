#pragma once
#include "DiscordMessage.h"

class DiscordServerStats
{
public:
	DiscordServerStats();
	void filterYear(int inYear);
	void findTopTenPosters();
	void findtop5HCMusicPosters();
	void findtop5MusicPosters();
	void findpromoterWithMostFlyers();
	void findtop3ReactedMessages();
	void findtop3MemeLords();
	void findtop3Reactions();

	void printResults();
	void addMessage(std::string inLine); //we'll do all of our line parsing in here. 
	void changeChannel(std::string inChannel);

private:
	std::vector<DiscordMessage*> allMessages;
	std::vector<DiscordMessage*> allCurrentYear;
	std::vector<std::string> top10Posters;
	std::vector<std::string> top5HCMusicPosters;
	std::vector<std::string> top5MusicPosters;
	std::vector<std::string> promoterWithMostFlyers; //promoters have a specific color name
	std::vector<DiscordMessage*> top3ReactedMessages;
	std::vector<std::string> top3MemeLords;
	std::vector<std::string> top3Reactions;

	std::string currentChannel;

	bool newLineFlag;
};

