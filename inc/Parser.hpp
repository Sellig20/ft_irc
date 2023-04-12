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

        class WrongPasswordException : public std::exception
        {
            public :
                const char *what() const throw()
                {
                   return ("Wrong password... Try again ?");
                }
        };
        class ChannelDoesNotExistException : public std::exception
        {
            public :
                const char *what() const throw()
                {
                   return ("No such channel");
                }
        };
        class UserDoesNotExistException : public std::exception
        {
            public :
                const char *what() const throw()
                {
                   return ("No such user");
                }
        };
        class CannotBeKickedException : public std::exception
        {
            public :
                const char *what() const throw()
                {
                   return ("No such user");
                }
        };

        void    check_for_cmd();
        void    tokenizer();
        void    execute()
        {
            if (_cmd.compare("PRIVMSG") == 0)
                privmsg();
            else if (_cmd.compare("JOIN") == 0)
                join();
            else if (_cmd.compare("PASS") == 0)
                pass();
            else if (_cmd.compare("NICK") == 0)
                nick();
            else if (_cmd.compare("USER") == 0)
                user();
            else if (_cmd.compare("PING") == 0)
                ping();
            else if (_cmd.compare("PONG") == 0)
                pong();
            else if (_cmd.compare("OPER") == 0)
                oper();
            else if (_cmd.compare("QUIT") == 0)
                quit();
            else if (_cmd.compare("ERROR") == 0)
                error();
        }

        //CONNECTION OPERATIO
        void    pass()
        {
            if (!_cmd.compare(_pass))
            {
                //they can go on with authentication
            }
            else
                throw WrongPasswordException();
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

        void    ping(){}
        void    pong(){}
        void    oper(){}
        void    quit()
        {
            _user->erase_me_from_allchannel();
            _tree->erase_user(_user);
        }

        void    error(std::string  _error)
        {
            //send the _error to the client
        }

        //CHANNEL OPERATION
        void    part()
        {}

        void    topic()
        {
            std::map<std::string, Channel>::iterator it = _tree->get_channel().find(_param[0]);

            if (it != _tree->get_channel().find(_param[0]))
            {
                it->second.get_topic() = _param[1];
                //send a topic message to alll the members of the channel
            }
            else
                //error channel doont exist

        }

        void    invite()
        {
            std::map<std::string, Channel>::iterator it = _tree->get_channel().find(_param[0]);
            if (it != _tree->get_channel().end())
            {
                if (it->second.size() < 1)
                    _user->_fd >> "INVITE: error: empty channel\n";
                else if (it->second.is_member(_param[0]))
                    _user->_fd >> "INVITE: error: you are already in the channel !\n";
                else
                {
                    it->second.add_member(_tree->find_usr_by_nickname(_param[0]));
                    //send a success msg to the user
                }
            }
            else
                //channel does not exist error
        }

        void    kick()
        {
            std::string channelName;
            std::string userToKick;
            std::map<std::string, Channel>::iterator it = _tree->get_channel().find(_param[0]);
            if (_param[0] == "" || _param[1] == "")
            {
                //throw exception mmissing pe arg for kick
            }
            else if (it == _tree->get_channel().end())
                throw ChannelDoesNotExistException();//change with the fd
            else if (it != _tree->get_channel().end())
            {
                if (it->second.is_member(_param[1]))
                    it->second.erase_user(_tree->find_usr_by_nickname(_param[1]));
                else if (it->second.is_oper(_param[1]))
                    _user->_fd >> "User cannot be kicked because he is an administrator !";//to write in the wbuffer / fd of the client who asked to kick
                else
                    throw UserDoesNotExistException(); //change with the fd
            }
        }
        //in the commands you will mostly use find, erase, insert functions of map to execute
        void    join()
        {
            std::map<std::string, Channel>::iterator it = _tree->get_channel().find(_param[0]);
            if (it != _tree->get_channel().end())
            {
                if (!it->second.is_ban(_user->_nickname))
                {
                    //discuss what we need to do with the keys
                    it->second.add_member(*_user);//??ask aguillar
                    //1.send a JOIN message to the client
                    if (it->second.get_topic() != "")
                    {
                        //send a message to the client fd with the topic of the channel he joined
                    }
                    //3.call the name command : name();
                }
                else
                    //user is banned exception
            }
            else
                throw ChannelDoesNotExistException();

        }
        void    privmsg()
        {
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
                //error for NO chan and NO user
                //exception
            }
        }
        void    notice();


    private:
        std::string     _cmd;
        std::string     _param[3];
        User            *_user;
        Tree            *_tree;
        std::string     _pass;//discuss later




};
#endif
