#include "DiscordMessage.h"

DiscordMessage::DiscordMessage(std::string inUsername, std::string inDateTime, std::string inPostContent, std::string inChannelPosted, postType_t inPostType)
{
	userName = inUsername;
	dateTime = inDateTime;
	postContent = inPostContent;
	channelPosted = inChannelPosted;
	postType = inPostType;
}

void DiscordMessage::addReactions(std::string inReaction)
{
	postReactions.push_back(inReaction);
}
