#include "DiscordMessage.h"

DiscordMessage::DiscordMessage()
{
	userName = "";
	dateTime = "";
	postContent = "";
	channelPosted = "";
	postType = text;
}

void DiscordMessage::addReactions(std::string inReaction)
{
	postReactions.push_back(inReaction);
}
