#include <iostream>
#include <unordered_map>
#include <string>
#include <regex>
#include <unordered_set>

class Group
{
public:
    Group() : _group_id(-1) {}

    Group(int id): _group_id(id) {};

    ~Group() = default;

    int get_group_id() const
    {
        return _group_id;
    }

    std::unordered_set<int>& get_user_list()
    {
        return _users;
    }

    void deleteUser(int userId)
    {
        auto it = _users.find(userId);
        if (it != _users.end())
            _users.erase(it);
    }

    bool operator==(const Group& rhs) const
    {
        return this->get_group_id() == rhs.get_group_id();
    }
private:
    std::unordered_set<int> _users;
    int _group_id;
};

class User
{
public:
    User(int user_id, std::string name, int group_id = -1): _user_id(user_id), _name(name), _group_id(group_id) {};

    ~User() = default;

    int get_user_id() const
    {
        return _user_id;
    }

    std::string get_user_name() const
    {
        return _name;
    }

    int get_group_id() const
    {
        return _group_id;
    }

    void set_group_id(int id)
    {
        _group_id = id;
    }

    bool operator==(const User& rhs) const
    {
        return this->get_user_id() == rhs.get_user_id();
    }

private:
    int _user_id;
    int _group_id;
    std::string _name;
};

std::string command(std::string enter)
{
    std::string res;
    for (size_t i = 0; i < enter.size(); i++)
    {
        if(enter[i] == ' ')
            return res;
        res += enter[i];
    }
    return res;
}

void createUser(const std::string& enter, std::unordered_map<int, User>& users,
                std::unordered_map<int, Group>& groups)
{
    std::regex re(R"(createUser\s+\{(\d+)\}\s+\{([^{}]+)\}(?:\s+\{(-?\d+)\})?)");
    std::smatch match;
    if (std::regex_match(enter, match, re))
    {
        int userId = std::stoi(match[1]);
        if (userId <= 0)
        {
            std::cerr << "User's id must be more than 0\n";
            return;
        }
        std::string username = match[2];
        int groupId = -1;
        if (match.size() > 3 && match[3].matched)
            groupId = std::stoi(match[3]);

        if (users.find(userId) != users.end())
        {
            std::cerr << "User with this id already exists\n";
            return;
        }

        if (groupId != -1)
        {
            if (groups.find(groupId) == groups.end())
            {
                std::cerr << "Group with id " << groupId << " doesn't exist\n";
                return;
            }
        }

        User newUser(userId, username, groupId);
        users.insert({userId, newUser});

        if (groupId != -1)
            groups[groupId].get_user_list().insert(userId);

        std::cout << "User added successfully\n";
    }
    else
        std::cerr << "Wrong command's format. Expected: createUser {userId} {username} [{groupId}]\n";
}

void deleteUser(const std::string& enter, std::unordered_map<int, User>& users,
    std::unordered_map<int, Group>& groups)
{
    std::regex re(R"(deleteUser\s+\{(\d+)\})");
    std::smatch match;
    if (std::regex_match(enter, match, re))
    {
        int userId = std::stoi(match[1]);

        auto it = users.find(userId);
        if (it == users.end())
        {
            std::cerr << "User with ID " << userId << " doesn't exist\n";
            return;
        }

        const User& user = it->second;
        int groupId = user.get_group_id();

        if (groupId != -1)
        {
            auto g_it = groups.find(groupId);
            if (g_it != groups.end())
                g_it->second.deleteUser(userId);
        }

        users.erase(it);
        std::cout << "User " << userId << " deleted successfully\n";
    }
    else
        std::cerr << "Wrong command format. Expected: deleteUser {userId}\n";
}

void out_user_information(User& user)
{
    int userId = user.get_user_id();
    std::string name = user.get_user_name();
    int groupId = user.get_group_id();
    std::cout << "Information about this user:\n";
    std::cout << "id: <" << userId << "> name: <" << name << ">";
    if (groupId != -1)
        std::cout << " group: <" << groupId << ">";
    std::cout << "\n";
}

void allUsers(std::unordered_map<int, User>& users)
{
    if (users.size() == 0)
        std::cout << "No any users exist\n";
    else
    {
        std::cout << "Full list of users:\n";
        for (auto& pair : users)
        {
            User& user = pair.second;
            out_user_information(user);
        }
    }
}

void getUser(std::string& enter, std::unordered_map<int, User>& users)
{
    std::regex re(R"(getUser\s+\{(\d+)\})");
    std::smatch match;
    if (std::regex_match(enter, match, re))
    {
        int userId = std::stoi(match[1]);

        auto it = users.find(userId);
        if(it == users.end())
        {
            std::cerr << "User with this id doesn't exist\n";
            return;
        }

        User& user = it->second;
        out_user_information(user);
    }
    else
        std::cerr << "Wrong command's format\n";
}

void createGroup(const std::string& enter, std::unordered_map<int, Group>& groups)
{
    std::regex re(R"(createGroup\s+\{(\d+)\})");
    std::smatch match;
    if (std::regex_match(enter, match, re))
    {
        int groupId = std::stoi(match[1]);

        if (groupId <= 0)
        {
            std::cerr << "Group's id must be more than 0\n";
            return;
        }
        if (groups.find(groupId) != groups.end())
        {
            std::cerr << "Group with this id already exists\n";
            return;
        }

        groups.insert({groupId, Group(groupId)});
        std::cout << "Group added successfully\n";
    }
    else
        std::cerr << "Wrong command's format\n";
}

void deleteGroup(const std::string& enter, std::unordered_map<int, User>& users,
    std::unordered_map<int, Group>& groups)
{
    std::regex re(R"(deleteGroup\s+\{(\d+)\})");
    std::smatch match;
    if (std::regex_match(enter, match, re))
    {
        int groupId = std::stoi(match[1]);

        auto it = groups.find(groupId);
        if (it == groups.end())
        {
            std::cerr << "Group with ID " << groupId << " doesn't exist\n";
            return;
        }

        Group& group = it->second;
        auto& user_list = group.get_user_list();
        for (auto v: user_list)
        {
            auto uit = users.find(v);
            if (uit != users.end())
                uit->second.set_group_id(-1);
        }

        groups.erase(it);
        std::cout << "Group " << groupId << " deleted successfully\n";
    }
    else
        std::cerr << "Wrong command format. Expected: deleteGroup {groupId}\n";
}

void allGroups(std::unordered_map<int, User>& users, std::unordered_map<int, Group>& groups)
{
    if (groups.size() == 0)
        std::cout << "No any groups exist\n";
    else
    {
        std::cout << "Full list of groups:\n\n";
        for (auto& pair : groups)
        {
            auto& user_list = pair.second.get_user_list();
            if (user_list.size() == 0)
            {
                std::cout << "Group <" << pair.second.get_group_id() << "> doesn't have users:\n";
                continue;
            }
            std::cout << "Group <" << pair.second.get_group_id() << "> has users:\n";
            for(auto &u: user_list)
            {
                auto it = users.find(u);
                if (it != users.end())
                {
                    User& user = it->second;
                    out_user_information(user);
                }
            }
            std::cout << '\n';
        }
    }
}

void getGroup(std::string& enter, std::unordered_map<int, User>& users,
    std::unordered_map<int, Group>& groups)
{
    std::regex re(R"(getGroup\s+\{(\d+)\})");
    std::smatch match;
    if (std::regex_match(enter, match, re))
    {
        int groupId = std::stoi(match[1]);

        auto it = groups.find(groupId);
        if (it == groups.end())
        {
            std::cerr << "Group with this id doesn't exist\n";
            return;
        }

        Group& group = it->second;
        auto& user_list = group.get_user_list();
        if (user_list.size() == 0)
        {
            std::cout << "Group <" << group.get_group_id() << "> doesn't have users:\n";
            return;
        }
        std::cout << "Group <" << group.get_group_id() << "> has users:\n";
        for(auto &u: user_list)
        {
            auto uit = users.find(u);
            if (uit != users.end())
            {
                User& user = uit->second;
                out_user_information(user);
            }
        }
    }
    else
        std::cerr << "Wrong command's format\n";
}

int main()
{
    std::string enter;
    std::getline(std::cin, enter);
    std::unordered_map<int, User> users;
    std::unordered_map<int, Group> groups;
    while(enter != "EXIT")
    {
        std::string cmmand = command(enter);
        if (cmmand == "createUser")
            createUser(enter, users, groups);
        else if (cmmand == "deleteUser")
            deleteUser(enter, users, groups);
        else if (cmmand == "allUsers")
            allUsers(users);
        else if (cmmand == "getUser")
            getUser(enter, users);
        else if (cmmand == "createGroup")
            createGroup(enter, groups);
        else if (cmmand == "deleteGroup")
            deleteGroup(enter, users, groups);
        else if (cmmand == "allGroups")
            allGroups(users, groups);
        else if (cmmand == "getGroup")
            getGroup(enter, users, groups);
        else
            std::cout << "Command not found\n";

        std::getline(std::cin, enter);
    }
}