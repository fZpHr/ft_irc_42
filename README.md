## This project involved collaboration, and my teammates were https://github.com/eyJvcy and https://github.com/BenjaminBerkrouber


## FT_IRC

IRC (Internet Relay Chat) is a 42 project where you create your own IRC server. The goal is to build a server that can handle multiple clients, allow them to communicate in real-time, and manage various chat functionalities. This project helps reinforce your understanding of network programming, socket handling, and concurrent programming concepts.

*For more information, please refer to the subject in the git.*


## Installation

To install the IRC server, clone the project:

```bash
git clone https://github.com/fZpHr/ft_irc_42.git
```
Then compile the executable using make
```bash
make
```
Finally, launch the server with:
```
./ircserv <port> <password>
```

## USAGE

Connect to the server using any IRC client, such as Netcat:
```
/nc localhost <port> <password>
```
Then authentifcate yourself in the server:

```
PASS <password>
NICK <nick>
USER <user> * * :<realname>
```
Once you are authenticated, you can use the following commands:

Join a channel:
```
JOIN #<channel_name>
````
Leave a channel:
```
PART #<channel_name>
```
Send a private message to user:
```
PRIVMSG <nick> <message>
```

Send message to a channel:
```
PRIVMSG #<channel_name> <message>
```

Kick a client from the channel:
```
KICK #<channel_name> <nick> [<comment>]
```
Invite a client to the channel:
```
INVITE <nick> #<channel_name>
```
Modify or display the channel topic:
```
TOPIC #<channel_name> <topic>
```

Change the channel mode (i for invite, t for topic restricted, k for password, i for operator privilege, l for user limit in the channel:
```
MODE #<channel_name> [+|-]<mode> [<mode_params>]
```

## Bonus (bot client)
Bot client with a tic tac toe game, playing against a IA
## Installation


Compile bot
```bash
make bot
```
Launcht it
```
./bot localhost <port>
```

## USAGE

To communicate with the bot, send it a private message using the following command format, it will then send you instructions to get started:
```
PRIVMSG botnick <message>
```
