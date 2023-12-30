#pragma once
#include <string>
#include <vector>
#include <iostream>

typedef enum {
	text,
	link,
	image,
}postType_t;


class DiscordMessage
{
public:
	DiscordMessage();
	void addReactions(std::string inReaction);
	std::string userName;
	std::string dateTime;
	std::string postContent;
	std::string channelPosted;
	postType_t postType;
	std::vector<std::string> postReactions;

private:

	
};

