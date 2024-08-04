#pragma once

#define CHANNELMODES "itkol"

// The server sends Replies 001 to 004 to a user upon successful registration.
#define RPL_WELCOME(nick, host) (":" + host + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\r\n")
#define RPL_YOURHOST(host, nick) (":" + host + " 002 " + nick + " :Your host is " + host + ", running version 1.0.0" + "\r\n")
#define RPL_CREATED(host, nick) (":" + host + " 003 " + nick + " :This server was created on 10/07/2024. \r\n")
#define RPL_MYINFO(host, nick) (":" + host +  " 004 " + nick + " :Server " + host + " Version 1.0.0 - Channel modes: " + CHANNELMODES + "\r\n")

#define ERR_PASSWDMISMATCH(host) (":" + host + " 464 " + host + " : Password incorrect \r\n")
#define ERR_NEEDMOREPARAMS(command, host) (":" + host + " 461 " + command + " : Not enough parameters \r\n")
#define ERR_ALREADYREGISTERED(host) (":" + host + " 462 " + host + " : You may not reregister \r\n")

#define ERR_NOTREGISTERED(host) (":" + host + " 451 " + host + " :You have not registered \r\n")
#define ERR_NONICKNAMEGIVEN(host) (":" + host + " 431 " + host + " :No nickname given \r\n")
#define ERR_ERRONEUSNICKNAME(host, nick) (":" + host + " 432 " + nick + " :Erroneus nickname \r\n")
#define ERR_NICKNAMEINUSE(host, nick) (":" + host + " 433 * " + nick + " :Nickname is already in use \r\n")
#define	NICKNAMECHANGE(nick, oldnick) (":" + oldnick + " NICK " + nick + "\r\n")
#define ERR_UNKNOWNCOMMAND(command, host) (":" + host + " 421 " + command + " :Unknown command \r\n")

//private messages
#define ERR_TOOMANYTARGETS(extraTarget, host) (":" + host + " 407 " + extraTarget + " :More than one recipient, aborted message" + "\r\n")
#define ERR_NORECIPIENT(command, host) (":" + host + " 411 " + command + " :No recipient given" + "\r\n")
#define ERR_NOTEXTTOSEND(host) (":" + host + " 412 :No text to send\r\n")
#define ERR_NOSUCHNICK(nick, host) (":" + host + " 401 " + nick + " :No such nick/channel" + "\r\n")
#define ERR_CANNOTSENDTOCHAN(channel) ("404 " + channel + " :Cannot send to channel" + "\r\n")
#define PRIVMSG(prefix, target, message) (":" + prefix + " PRIVMSG " + target + " :" + message + "\r\n")

// channel
#define RPL_TOPIC(prefix, nick, channel, topic) (":" + prefix + " 332 " + nick + " " + channel + " " + topic + "\r\n")
#define RPL_NOTOPIC(prefix, nick, channel) (":" + prefix + " 331 " + nick + " " + channel + " :No Topic Set \r\n")
#define RPL_INVITING(channel, nick, prefix) (":" + prefix + " 341 INVITE " + nick + " " + channel + "\r\n")
#define RPL_NAMREPLY(prefix, nick, channel, users) (":" + prefix + " 353 " + nick + " = " + channel + " :" + users + "\r\n")
#define	RPL_ENDOFNAMES(prefix, nick, channel) (":" + prefix + " 366 " + nick + " " + channel + " :End of NAMES list\r\n")
#define RPL_KICK(nick, prefix, channel, comment) (":" + prefix + " KICK " + channel + " " + nick + " " + comment + "\r\n")
#define RPL_PART(prefix, channel, comment) (":" + prefix + " PART " + channel + " " + comment + "\r\n")
#define RPL_JOIN(prefix, channel) (":" + prefix + " JOIN " + channel + "\r\n")
#define RPL_MODE(mode, nick, channel) (":" + nick + " MODE " + channel + " " + mode + " \r\n")
#define ERR_NOSUCHCHANNEL(channel, host) (":" + host + " 403 " + channel + " :No such channel, ")
#define ERR_USERNOTINCHANNEL(nick, channel, host) (":" + host + " 441 " + channel + " " + nick + " :They aren't on that channel \r\n")
#define ERR_NOTONCHANNEL(channel, host) (":" + host + " 442 " + channel + " :You're not on that channel \r\n")
#define ERR_USERONCHANNEL(user, channel, host) (":" + host + " 443 " + channel + " " + user + " is already on channel \r\n")
#define ERR_KEYSET(channel) ("467 " + channel + "  :Channel key already set \r\n")
#define ERR_CHANNELISFULL(channel) ("471 " + channel + " :Cannot join channel (+l) \r\n")
#define ERR_INVITEONLYCHAN(channel) ("473 " + channel + " :Cannot join channel (+i) \r\n")
#define ERR_BADCHANNELKEY(channel) ("475 " + channel + " :Cannot join channel (+k) \r\n")
#define ERR_NOPRIVILEGES(channel) ("481 " + channel + " :Permission Denied- You are not an IRC operator \r\n")
#define ERR_CHANOPRIVSNEEDED(channel, host) (":" + host + " 482 " + channel + " :You're not channel operator \r\n")
#define ERR_CHANOP(channel, host) (":" + host + " ERROR " + channel + " :You can't kick an operator \r\n")
#define ERR_WRONGKEY(channel, host) (":" + host + " ERROR " + channel + " :Wrong Channel key \r\n")
#define ERR_UNKNOWNMODE(channel) ("472 : Unknown mode char for " + channel + "\r\n")
#define RPL_QUIT(prefix, message) (":" + prefix + " QUIT " + message + "\r\n")


// Error codes can be found on https://www.irchelp.org/protocol/rfc/chapter6.html
// this website has the errors and describes the situations to use each of them
