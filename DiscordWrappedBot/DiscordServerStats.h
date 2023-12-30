#pragma once
#include "DiscordMessage.h"
#include <unordered_map>
#include <map>

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
	ignoreNextTag,
	ignoreUntilPop,
	mentionTag,
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
	tagType processTag(std::string inHTMLTag, bool igUPopFlag);

	void parseTypes();
	void sortMap(std::vector<std::string> *users, std::vector<int> *values, std::unordered_map<std::string, int> inputMap);
private:
	std::vector<DiscordMessage*> allMessages;

	std::string currentChannel;
	std::vector<std::string> tagStack;
	std::string findClassName(std::string inHtmlTag);
};

