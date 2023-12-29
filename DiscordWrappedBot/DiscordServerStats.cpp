#include "DiscordServerStats.h"

DiscordServerStats::DiscordServerStats()
{
	currentChannel = "no Channel";
}

void DiscordServerStats::filterYear(int inYear)
{
}

void DiscordServerStats::printResults()
{
}

void DiscordServerStats::addMessage(std::string inLine)
{

	/*Changes to make:
	* We need to take in each HTML tag / line, and parse acordingly. 
	* basically, we need to make a destructive parser that returns the current element. 
	* We can have a big div counter, that counts our div scope. 
	* and as we come across all tags, we can mark what they are for. 
	* we can create a post according to what we find in the tags. 
	*/

	DiscordMessage* insertMessage = new DiscordMessage();
	postType_t currentPost = text;
	std::string messageUserName;
	std::string messageDateTime;
	std::string messageContent = "";
	std::string currentReaction;
	int currentReactionCount;

	while (inLine.size() > 0)
	{
		size_t stringSize = inLine.size();
		std::string currentTag = getHtmlTag(inLine);
		size_t tagSize = currentTag.size();
		inLine = inLine.substr(tagSize, inLine.size() - tagSize);

		if (currentTag == " ") 
		{
			currentTag = getHtmlTag(inLine);
			tagSize = currentTag.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
		}


		tagType currentTagType = processTag(currentTag);


		int j;
		switch (currentTagType) 
		{
		case usernameTag:
			messageUserName = getHtmlTag(inLine);
			tagSize = messageUserName.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
			break;

		case dateTimeTag:
		{
			std::string skipTag = getHtmlTag(inLine); //we expect this to be an <a> tag that we do not care about
			tagSize = skipTag.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
			messageDateTime = getHtmlTag(inLine);
			tagSize = messageDateTime.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
		}
			break;

		case messageTextTag:
		{
			std::string catString = getHtmlTag(inLine);
			if (catString == "</a>") 
			{
				//if we see an Atag, there may be a link in it that is part of a message. 
				processTag(catString);
				break;
			}
			else if (catString.find("<span") != -1)
			{
				//This is a mention in a message. 
				tagType possibleMention = processTag(catString);
				if (possibleMention == mentionTag) 
				{
					tagSize = catString.size();
					inLine = inLine.substr(tagSize, inLine.size() - tagSize);
					catString = getHtmlTag(inLine);
				}

			}
			messageContent = messageContent + " " + catString;
			tagSize = catString.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
		}
			break;

			 
		case messageImageTag:{
			if (currentTag.find("<video") != -1)
			{
				std::string sourceString = getHtmlTag(inLine);
				tagSize = sourceString.size();
				size_t urlStart = sourceString.find("src=");
				size_t urlEnd = sourceString.find("alt");
				std::string catString = sourceString.substr(urlStart+4, urlEnd - (urlStart+4));
				messageContent = messageContent + " " + catString;
				inLine = inLine.substr(tagSize, inLine.size() - tagSize);
			}
			else 
			{
				std::string catString;
				size_t urlStart = currentTag.find("src=");
				size_t urlEnd = currentTag.find("alt", urlStart);
				catString = currentTag.substr(urlStart + 4, urlEnd - (urlStart + 4));
				messageContent = messageContent + " " + catString;
			}


		}
			break;

		case messageReactionTag:
		{
			size_t reactionNameStart = currentTag.find("title=");
			size_t reactionNameEnd = currentTag.find(">");
			currentReaction = currentTag.substr(reactionNameStart+6, reactionNameEnd - (reactionNameStart+6));
		}			
			break;

		case messageReactionCountTag:
		{
			std::string value = getHtmlTag(inLine);
			currentReactionCount = stoi(value);
			tagSize = value.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);

			//Then do our insert loop
			for (int i = 0; i<currentReactionCount; i++) 
			{
				insertMessage->addReactions(currentReaction);
			}
		}
			break;

		case mentionTag:
			{
				std::string catString = getHtmlTag(inLine);
				messageContent = messageContent + " " + catString;
				tagSize = catString.size();
				inLine = inLine.substr(tagSize, inLine.size() - tagSize);
			}
			break;

		case unknownTag:
			if (tagStack.back() == "<span class=chatlog__markdown-preserve>")
			{
				messageContent = messageContent + " " + currentTag;
				break;
				//this is the rest of a message after a mention. Hacky, but should work. 
			}
			std::cout << "unknown tag encountered: ";
			std::cout << currentTag << std::endl;
			std::cout << messageDateTime << std::endl;
			return;
			break;

		case ignoreTag:
			break;

		case ignoreNextTag:
		{ //the element after the edit tag must be skipped
			std::string skipTag = getHtmlTag(inLine); //we expect this to be an <a> tag that we do not care about
			tagSize = skipTag.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
		}
			break;

		default:
			break;

		}

	}

	insertMessage->userName = messageUserName;
	insertMessage->dateTime = messageDateTime;
	insertMessage->postContent = messageContent;
	insertMessage->postType = currentPost;

	allMessages.push_back(insertMessage);


}

void DiscordServerStats::changeChannel(std::string inChannel)
{
	currentChannel = inChannel;
}

std::string DiscordServerStats::getHtmlTag(std::string inHTMLBlock)
{
	std::string returnString;

	size_t firstTagIndex = inHTMLBlock.find("<");
	if (firstTagIndex != 0)
	{
		//this is where we have a text element in our HTML Tag
		returnString = inHTMLBlock.substr(0, firstTagIndex);
	}
	else 
	{
		size_t lastTagIndex = inHTMLBlock.find(">");
		returnString = inHTMLBlock.substr(firstTagIndex, lastTagIndex - firstTagIndex + 1);
	}

	return returnString;
}

tagType DiscordServerStats::processTag(std::string inHTMLTag)
{
	/*Tags that we should stack monitor:
	* Div
	* A
	* Span
	* 
	* Tags that do not need to be monitored:
	* img
	* 
	*/
	tagType returnTagType = unknownTag;
	std::string className;
	//Div section handeled. 
	if (inHTMLTag.find("<div") != -1) 
	{
		returnTagType = ignoreTag;
		tagStack.push_back(inHTMLTag);

		//now we need to find the class, and base what we do off of that. 

		size_t classStart = inHTMLTag.find("class=");
		size_t classEnd = inHTMLTag.find(" ", classStart);
		size_t classEnd2 = inHTMLTag.find(">", classStart);
		
		if (classEnd != -1) 
		{
			className = inHTMLTag.substr(classStart + 6, classEnd - (classStart + 6));
		}
		else if (classEnd2 != -1) 
		{
			className = inHTMLTag.substr(classStart + 6, classEnd2 - (classStart + 6));
		}
		else 
		{
			std::cout << "div with no class, seems wrong" << std::endl;
			return returnTagType;
		}

		if (className == "chatlog__reaction") 
		{
			returnTagType = messageReactionTag;
			return returnTagType;
		}
		else if (className == "chatlog__reply-author")
		{
			returnTagType = ignoreNextTag;
			return returnTagType;
		}
		else if (className == "chatlog__short-timestamp")
		{
			returnTagType = ignoreNextTag;
			return returnTagType;
		}
		else if (className == "chatlog__message-group" || 
			className == "chatlog__message-container" || className == "chatlog__message" ||
			className == "chatlog__message-aside" || className == "chatlog__reply-symbol" ||
			className == "chatlog__message-primary" || className == "chatlog__reply" ||
			className == "chatlog__reply-content" || className == "chatlog__header" ||
			className == "\"chatlog__content" || className == "chatlog__attachment" ||
			className == "chatlog__reactions" || className == "chatlog__embed" || 
			className == "chatlog__reply-unknown" || className == "\"chatlog__message-container" ||
			className == "\"chatlog__embed-color-pill" || className == "chatlog__embed-content-container" ||
			className == "chatlog__embed-content" || className == "chatlog__embed-text" ||
			className == "chatlog__embed-title" || className == "chatlog__embed-color-pill" ||
			className == "chatlog__embed-author-container" || className == "chatlog__embed-spotify-container" ||
			className == "chatlog__embed-footer" || className == "chatlog__markdown-quote-content")
		{
			return returnTagType;
		}
		else 
		{
			std::cout << "Unrecognized Div Class: ";
			std::cout << inHTMLTag << std::endl;
			//there are a lot of div classes, and we can probably ignore most of them. 
		}

	}

	if (inHTMLTag.find("</div>") != -1 && inHTMLTag.size() == 6) 
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size()-1).find("<div") != -1)
		{
			tagStack.pop_back(); //if we are currently on a div tag, we can pop that tag off of the stack. 
		}
	}


	//I do feel like I'm repeating myself a but here, but we do need to seperate out our tags. Might consolidate later during optimization, if I do that. 
	if (inHTMLTag.find("<span") != -1) 
	{
		returnTagType = ignoreTag;
		tagStack.push_back(inHTMLTag);

		size_t classStart = inHTMLTag.find("class=");
		size_t classEnd = inHTMLTag.find(" ", classStart);
		size_t classEnd2 = inHTMLTag.find(">", classStart);

		if (classEnd != -1)
		{
			className = inHTMLTag.substr(classStart + 6, classEnd - (classStart + 6));
		}
		else if (classEnd2 != -1)
		{
			className = inHTMLTag.substr(classStart + 6, classEnd2 - (classStart + 6));
		}
		else
		{
			//std::cout << "span with no class, seems wrong" << std::endl;
			return returnTagType;
		}

		if (className == "chatlog__markdown-preserve")
		{
			returnTagType = messageTextTag;
			return returnTagType;
		}
		else if (className == "chatlog__author") 
		{
			returnTagType = usernameTag;
			return returnTagType;
		}
		else if (className == "chatlog__timestamp") 
		{
			returnTagType = dateTimeTag;
			return returnTagType;
		}
		else if (className == "chatlog__reaction-count")
		{
			returnTagType = messageReactionCountTag;
			return returnTagType;
		}
		else if (className == "chatlog__edited-timestamp")
		{
			returnTagType = ignoreNextTag;
			return returnTagType;
		}
		else if (className == "chatlog__reply-link")
		{
			returnTagType = ignoreNextTag;
			return returnTagType;
		}
		else if (className == "chatlog__reply-edited-timestamp") 
		{
			returnTagType = ignoreNextTag;
			return returnTagType;
		}
		else if (className == "\"chatlog__markdown-mention\"" || className == "chatlog__markdown-mention")
		{
			returnTagType = mentionTag;
			return returnTagType;
		}
		else if (className == "\"chatlog__markdown-spoiler")
		{
			returnTagType = messageTextTag;
			return returnTagType;
		}
		else 
		{
			std::cout << "unrecognized Span class" << std::endl;

		}

	}

	if (inHTMLTag.find("</span>") != -1 && inHTMLTag.size() == 7)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<span") != -1)
		{
			tagStack.pop_back(); //if we are currently on a span tag, we can pop that tag off of the stack. 
		}
	}

	if (inHTMLTag.find("<a") != -1) 
	{
		returnTagType = messageTextTag;
		//Atags do not have a class
		tagStack.push_back(inHTMLTag);
		return returnTagType;
	}
	if (inHTMLTag.find("</a>") != -1 && inHTMLTag.size() == 4)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<a") != -1)
		{
			tagStack.pop_back(); //if we are currently on an a tag, we can pop that tag off of the stack. 
		}
		return returnTagType;
	}

	if (inHTMLTag.find("<img") != -1)
	{
		//this is not one we have to put on our stack
		size_t classStart = inHTMLTag.find("class=");
		size_t classEnd = inHTMLTag.find(" ", classStart);
		size_t classEnd2 = inHTMLTag.find(">", classStart);

		if (classEnd != -1)
		{
			className = inHTMLTag.substr(classStart + 6, classEnd - (classStart + 6));
		}
		else if (classEnd2 != -1)
		{
			className = inHTMLTag.substr(classStart + 6, classEnd2 - (classStart + 6));
		}
		else
		{
			std::cout << "img with no class, seems wrong" << std::endl;
			return returnTagType;
		}

		if (className == "chatlog__attachment-media")
		{
			returnTagType = messageImageTag;
			return returnTagType;
		}
		else if (className == "chatlog__avatar") 
		{
			returnTagType = ignoreTag;
			return returnTagType;
		}
		else if (className == "chatlog__reply-avatar")
		{
			returnTagType = ignoreTag;
			return returnTagType;
		}
		else if (className == "chatlog__emoji chatlog__emoji--small") 
		{
			returnTagType = ignoreTag;
			return returnTagType;
		}
		else if (className == "chatlog__emoji" || className == "\"chatlog__emoji")
		{
			returnTagType = ignoreTag;
			return returnTagType;
		}
		else if (className == "chatlog__embed-generic-image")
		{
			returnTagType = messageImageTag;
			return returnTagType;
		}
		else 
		{
			std::cout << "unrecognized Image class" << std::endl;
		}


	}

	if (inHTMLTag.find("<video") != -1)
	{
		tagStack.push_back(inHTMLTag);
		size_t classStart = inHTMLTag.find("class=");
		size_t classEnd = inHTMLTag.find(" ", classStart);
		size_t classEnd2 = inHTMLTag.find(">", classStart);

		if (classEnd != -1)
		{
			className = inHTMLTag.substr(classStart + 6, classEnd - (classStart + 6));
		}
		else if (classEnd2 != -1)
		{
			className = inHTMLTag.substr(classStart + 6, classEnd2 - (classStart + 6));
		}
		else
		{
			std::cout << "img with no class, seems wrong" << std::endl;
			return returnTagType;
		}

		if (className == "chatlog__attachment-media")
		{
			returnTagType = messageImageTag;
			return returnTagType;
		}
		else if (className == "chatlog__embed-generic-video")
		{
			returnTagType = messageImageTag;
			return returnTagType;

		}
		else if (className == "chatlog__embed-generic-gifv") 
		{
			returnTagType = messageImageTag;
			return returnTagType;
		}


	}

	if (inHTMLTag.find("</video") != -1)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<video") != -1)
		{
			tagStack.pop_back(); //if we are currently on an a tag, we can pop that tag off of the stack. 
		}
		return returnTagType;

	}

	if (inHTMLTag.find("<em") != -1) 
	{
		tagStack.push_back(inHTMLTag);
		returnTagType = messageTextTag;
		return returnTagType;
	}

	if (inHTMLTag.find("</em") != -1) 
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<em") != -1)
		{
			tagStack.pop_back(); //if we are currently on an a tag, we can pop that tag off of the stack. 
		}
		return returnTagType;
	}

	if (inHTMLTag.find("<iframe") != -1)
	{
		tagStack.push_back(inHTMLTag);
		returnTagType = ignoreTag;
		return returnTagType;
	}

	if (inHTMLTag.find("</iframe") != -1)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<iframe") != -1)
		{
			tagStack.pop_back(); //if we are currently on an a tag, we can pop that tag off of the stack. 
		}
		return returnTagType;

	}



	return returnTagType;
}
