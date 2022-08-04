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
    std::string peopleXid;

public:
    forJson(/* args */)
    {

        srand((unsigned)time(NULL));
    }
    ~forJson() {}

    std::string createJson()
    {
        std::string jsonstr;
        Json::Value root;
        std::cout << "请输入账号" << std::endl;
        std::cin >> uid;
        std::cout << "请输入昵称" << std::endl;
        std::cin >> name;
        std::cout << "请输入密码" << std::endl;
        std::cin >> passwd;
        std::cout << "请输入邮箱" << std::endl;
        std::cin >> email;

        peopleXid = std::to_string(rand() % 1000);
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
        root.append(peopleXid);
        Json::FastWriter f;

        jsonstr = f.write(root);
        return jsonstr;
    }
    std::string foraddfriend()
    {
        std::string frienduid;
        std::cout << "要加好友的uid" << std::endl;
        std::cin >> frienduid;

        Json::Value root;
        std::string jsonstr;
        root.append(uid);
        root.append(frienduid);

        std::cout << " uid" << uid << std::endl;
        std::cout << " uid" << frienduid << std::endl;
        Json::FastWriter f;
        jsonstr = f.write(root);
        return jsonstr;
    }
    std::string forlogin()
    {
        std::string logid;
        std::string logname;
        std::string logpasswd;
        std::cout << "请输入账号" << std::endl;
        std::cin >> logid;
        std::cout << "请输入昵称" << std::endl;
        std::cin >> logname;
        std::cout << "请输入密码" << std::endl;
        std::cin >> logpasswd;

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
        peopleXid = root[i++].asString();
        Storstr += peopleXid;

        return Storstr;
    }
};
