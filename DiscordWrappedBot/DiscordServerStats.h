#pragma once
#include "DiscordMessage.h"

typedef enum 
{
	usernameTag,
	dateTimeTag,
	messageTextTag,
	messageImageTag,
	messageReactionTag,
	messageReactionCountTag,
	unknownTag,
	ignoreTag,
}tagType;

class DiscordServerStats
{
public:
	DiscordServerStats();
	void filterYear(int inYear);

	void printResults();
	void addMessage(std::string inLine); //we'll do all of our line parsing in here. 
	void changeChannel(std::string inChannel);
	std::string getHtmlTag(std::string inHTMLBlock);
	tagType processTag(std::string inHTMLTag);

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
	std::vector<std::string> tagStack;
};

