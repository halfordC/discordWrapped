#include "DiscordServerStats.h"

DiscordServerStats::DiscordServerStats()
{
	newLineFlag = false;
}

void DiscordServerStats::filterYear(int inYear)
{
}

void DiscordServerStats::findTopTenPosters()
{
}

void DiscordServerStats::findtop5HCMusicPosters()
{
}

void DiscordServerStats::findtop5MusicPosters()
{
}

void DiscordServerStats::findpromoterWithMostFlyers()
{
}

void DiscordServerStats::findtop3ReactedMessages()
{
}

void DiscordServerStats::findtop3MemeLords()
{
}

void DiscordServerStats::findtop3Reactions()
{
}

void DiscordServerStats::printResults()
{
}

void DiscordServerStats::addMessage(std::string inLine)
{
	int usernameLocationTag = inLine.find("<span class=chatlog__author");
	if (usernameLocationTag == -1) 
	{
		std::cout << "Not a discord message" << std::endl;
		return;
	}

	int userNameLocationStart = inLine.find(">", usernameLocationTag) +1;
	int userNameLocationEnd = inLine.find("<", userNameLocationStart);
	std::string userName = inLine.substr(userNameLocationStart, userNameLocationEnd - userNameLocationStart);

	int dateTimeTag = inLine.find("<span class=chatlog__timestamp");
	int dateTimeStart = inLine.find("title=\"", dateTimeTag)+7;
	int dateTimeEnd = inLine.find("\">", dateTimeStart);
	std::string dateTime = inLine.substr(dateTimeStart, dateTimeEnd - dateTimeStart);


	int postContentTag = inLine.find("<span class=chatlog__markdown-preserve");
	int postContentStart = inLine.find(">", postContentTag) + 1;
	int postContentEnd = inLine.find("</span>", postContentStart);

	if (postContentEnd == -1) 
	{
		//this means that someone put an /n character in their message, and the HTML will reflect that. 
		//we need to have a flag that goes back to our while loop that deals with this. 
	}

	std::string postContent = inLine.substr(postContentStart, postContentEnd - postContentStart);

	//int channelPostedTag; //this has to be done on a file by file basis
	int postReactionTag;

	//then we need to decide post type based on post Content
}
