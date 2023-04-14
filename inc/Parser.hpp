#ifndef _PARSER_HPP
#define _PARSER_HPP

#include "ft_irc.hpp"

class Parser
{
    public:
        Parser(Tree &tree)
        {
            _tree = &tree;
            _cmd = "";
            _param = NULL;
            _user = NULL;
        }

        ~Parser();

        void    check_for_cmd();
        void    tokenizer();
        void    execute()
        {
            if (_cmd.compare("CAP") == 0)
                cap();
            else if (_cmd.compare("PASS") == 0)
                pass();
            else if (_cmd.compare("NICK") == 0)
                nick();
            else if (_cmd.compare("PRIVMSG") == 0)
                user();
            else if (_cmd.compare("PING") == 0)
                ping();
            else if (_cmd.compare("PONG") == 0)
                pong();
            else if (_cmd.compare("USER") == 0)
                user();
            else if (_cmd.compare("QUIT") == 0)
                quit();
            else if (_cmd.compare("PART") == 0)
                part();
            else if (_cmd.compare("TOPIC") == 0)
                topic();
            else if (_cmd.compare("INVITE") == 0)
                invite();
            else if (_cmd.compare("KICK") == 0)
                kick();
            else if (_cmd.compare("JOIN") == 0)
                join();
            else if (_cmd.compare("PRIVMSG") == 0)
                privmsg();
            else if (_cmd.compare("NOTICE") == 0)
                notice();
        }

        //CONNECTION OPERATION
        void    cap()
        {
            return ;
        }

        void    pass()
        {
            if (!_cmd.compare(_pass))
            {
                //they can go on with authentication
            }
            else
                _user->_fd << "PASS: error: wrong password, try again !\n";
        }
        void    nick()
        {
            _user->_nickname = _param[0];
        }

        void    user()
        {
            _user->_username = _param[0];
            _user->_realname = _param[3];
        }

        void    ping()
        {
            if (_param[0] == "")
            {
                _user->_fd << "PING: error: wrong parameter\n";
            }
            else
                _user->_fd << "PING\n";
        }

        void    pong()
        {
            if (_param[0] == "")
            {
                _user->_fd << "PONG: error: wrong parameter\n";
            }
            else
                _user->_fd << "PING\n";
        }

        void    oper()
        {

        }

        void    quit()
        {
            _user->erase_me_from_allchannel();
            _tree->erase_user(_user);
        }

        //CHANNEL OPERATION
        void    part()
        {
            if (_param[0] == "" || _param[1] == "")
            {
                _user->_fd << "KICK: error: wrong parameters\n";
            }
        }

        void    topic()
        {
            std::map<std::string, Channel>::iterator it = _tree->get_channel().find(_param[0]);
            if (_param[0] == "")
            {
                _user->_fd << "KICK: error: wrong parameter\n";
            }
            if (it != _tree->get_channel().find(_param[0]))
            {
                it->second.get_topic() = _param[1];
                //send a topic message to alll the members of the channel
                for (it; it != _user->_nickname.find().end())
                {
                    _user->_fd << "topic of the channe is " << _tree->get_channel();
                }
            }
            else
                _user->_fd << "TOPIC: error: channel does not exist\n";

        }

        void    invite()
        {
            std::map<std::string, Channel>::iterator it = _tree->get_channel().find(_param[0]);
            if (_param[0] == "" || _param[1] == "")
            {
                _user->_fd << "KICK: error: wrong parameters\n";
            }
            if (it != _tree->get_channel().end())
            {
                if (it->second.size() < 1)
                    (*_user)._wbuff.append("INVITE: error: empty channel\n");
                else if (it->second.is_member(_param[0]))
                    _user->_fd << "INVITE: error: you are already in the channel !\n";
                else
                {
                    it->second.add_member(_tree->find_usr_by_nickname(_param[0]));
                    _user->_fd << _user->_nickname << "'ve been successfully added to" << _tree->get_channel();
                }
            }
            else
                _user->_fd << "INVITE: error: channel does not exist\n"
        }

        void    kick()
        {
            std::string channelName;
            std::string userToKick;
            std::map<std::string, Channel>::iterator it = _tree->get_channel().find(_param[0]);
            if (_param[0] == "" || _param[1] == "")
            {
                _user->_fd << "KICK: error: wrong parameters\n";
            }
            else if (it == _tree->get_channel().end())
                _user->_fd << "KICK: error: channel does not exist\n";
            else if (it != _tree->get_channel().end())
            {
                if (it->second.is_member(_param[1]))
                    it->second.erase_user(_tree->find_usr_by_nickname(_param[1]));
                else if (it->second.is_oper(_param[1]))
                    _user->_fd << "User cannot be kicked because he is an administrator !";
                else
                    _user->_fd << "KICK: error: user does not exist\n"
            }
        }
        void    join()
        {
            std::map<std::string, Channel>::iterator it = _tree->get_channel().find(_param[0]);
            if (_param[0] == "" || _param[1] == "")
            {
                _user->_fd << "KICK: error: wrong parameters\n";
            }
            if (it != _tree->get_channel().end())
            {
                if (!it->second.is_ban(_user->_nickname))
                {
                    //discuss what we need to do with the keys
                    it->second.add_member(*_user);//??ask aguillar
                    _user->_fd << "you've joined the channel " << _tree->get_channel();
                    if (it->second.get_topic() != "")
                    {
                        _user->_fd << "topic of the channel is : " << _tree->get_channel();
                    }
                    //3.call the name command : name();
                }privmsg
                else
                    _user->_fd << "INVITE: error: channel does not exist\n"
            }
            else
                _user->_fd << "INVITE: error: "<< _user->_nickname << "is banned\n";

        }

        void    privmsg()
        {
            if (_param[0] == "" || _param[1] == "")
            {
                _user->_fd << "KICK: error: wrong parameters\n";
            }
            if (_tree->get_channel().find(_param[0]) != _tree->get_channel().end())
            {
                //send msg to channel //check if the sender/user has permission to write in THAT channel, if not ERROR CANNOT SENDTO CHAN
            }
            else if (_param[0].compare(_user->_nickname) == 0)
            {
                //send msg to the user
            }
            else
            {
                _user->_fd << "PRIVMSG: error: impossible to send your message\n";
            }
        }
        void    notice()
        {
            if (_param[0] == "" || _param[1] == "")
            {
                _user->_fd << "KICK: error: wrong parameters\n";
            }
            if (_tree->get_channel().find(_param[0]) != _tree->get_channel().end())
            {
                //send msg to channel //check if the sender/user has permission to write in THAT channel, if not ERROR CANNOT SENDTO CHAN
            }
            else if (_param[0].compare(_user->_nickname) == 0)
            {
                //send msg to the user
            }
            else
            {
                _user->_fd << "PRIVMSG: error: impossible to send your message\n";
            }
        }


    private:
        std::string     _cmd;
        std::string     _param[3];
        User            *_user;
        Tree            *_tree;
        std::string     _pass;//discuss late

};
#endif

/*

1. check for \r\n
2. take evrything before and extract if, memmove, if empty return
3. split by ws
4. fill in the fields (cmd + args)
5. do reg status check at 3
6. if cmd doest exist error
7. switch commands
    1. Pass, if no arg error, if wrong arg error, if right arg reg status +1
    2. nick, if no arg error, if non compliant error, if already a nick or channel name, error, else 1. put im _nickname 2. map. modify nickname 3. regstat+1
    3. user, idem, idem, else 1. update fields 2. regstat+1
    4. privmsg, if no target error, if non existing target error, else send to user / channel
    5. join, check if channel exist, check if not ban,

    ...
    x. quit, send quit msg to all member channel, remove user from tree, return

*/
