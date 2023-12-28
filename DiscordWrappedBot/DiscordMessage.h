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
	DiscordMessage( std::string inUsername, std::string inDateTime, std::string inPostContent, std::string inChannelPosted, postType_t inPostType);
	void addReactions(std::string inReaction);

private:
	std::string userName;
	std::string dateTime;
	std::string postContent;
	std::string channelPosted;
	postType_t postType;
	std::vector<std::string> postReactions;
};

