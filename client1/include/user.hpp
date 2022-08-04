// {
//     "uid" : "605325871",
//             "name" : "guo tian yu",
//                      "passwd" : "147258369",
//                                 "Email" : "605325871@qq.com"
// }

#include <iostream>
#include "json/json.h"
#include <fstream>

#include <cstdlib>
#include <time.h>

class forJson
{
public:
    std::string uid;
    std::string name;
    std::string passwd;
    std::string email;

public:
    forJson(/* args */)
    {

        srand((unsigned)time(NULL));
    }
    ~forJson() {}

    std::string createJson(std::string &_uid, std::string &_name, std::string &_passwd, std::string &_emial)
    {
        std::string jsonstr;
        Json::Value root;

        uid = _uid;

        name = _name;

        passwd = _passwd;

        email = _passwd;

        /* root["uid"] = us.uid;
         root["name"] = us.name;
         root["passwd"] = us.passwd;
         root["email"] = us.email;
         root["PeopleXid"] = us.peopleXid;*/
        // root.append()
        /*std::unique_ptr<Json::StreamWriter> jsonswrite(writerbuilder.newStreamWriter());

        jsonswrite->write(root, &os);
        jsonstr = os.str();*/
        // std::cout << jsonstr << std::endl;

        root.append(uid);
        root.append(passwd);
        root.append(name);
        root.append(email);

        Json::FastWriter f;

        jsonstr = f.write(root);
        return jsonstr;
    }
    std::string foraddfriend(std::string &_logind, std::string &_friendid)
    {
        std::string frienduid = _friendid;
        uid = _logind;
        Json::Value root;
        std::string jsonstr;
        root.append(uid);
        root.append(frienduid);

        Json::FastWriter f;
        jsonstr = f.write(root);
        return jsonstr;
    }
    std::string foraddgroup(std::string &_userid, std::string &_groupid)
    {
        std::string groupid = _groupid;
        uid = _userid;
        Json::Value root;
        std::string jsonstr;
        root.append(uid);
        root.append(groupid);

        Json::FastWriter f;
        jsonstr = f.write(root);
        return jsonstr;
    }

    std::string sendid(std::string &_sendid)
    {
        Json::Value root;
        std::string jsonstr;
        std::string uid = _sendid;
        root.append(uid);
        Json::FastWriter f;
        jsonstr = f.write(root);
        return jsonstr;
    }
    std::string forlogin(std::string &_logid, std::string &_logname, std::string &_logpasswd)
    {
        std::string logid = _logid;
        std::string logname = _logname;
        std::string logpasswd = _logpasswd;

        Json::Value root;
        std::string jsonstr;
        root.append(logid);
        root.append(logpasswd);
        root.append(logname);
        Json::FastWriter f;

        jsonstr = f.write(root);
        return jsonstr;
    }

    std::string parseJson(const std::string &info)
    {

        bool res;
        JSONCPP_STRING errs;
        Json::Value root;
        std::string Storstr;
        // Json::CharReaderBuilder readerBuilder;
        Json::Reader r;

        // std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
        // res = jsonReader->parse(info.c_str(), info.c_str() + info.length(), &root, &errs);

        r.parse(info, root);
        int i = 0;
        uid = root[i++].asString();
        Storstr += uid;
        passwd = root[i++].asString();
        Storstr += passwd;
        name = root[i++].asString();
        Storstr += name;
        email = root[i++].asString();
        Storstr += email;

        return Storstr;
    }
};
