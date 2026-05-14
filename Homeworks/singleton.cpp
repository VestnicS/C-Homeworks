#include <iostream>
#include <string>
#include <deque>
#include <ctime>

const int LOG_NORMAL = 0;
const int LOG_WARNING = 1;
const int LOG_ERROR = 2;

struct Event 
{
    std::string timeStr;
    int level;
    std::string text;
};

class Log 
{
private:
    std::deque<Event> events;
    static const int MAX_EVENTS = 10;

    Log() {}
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

public:
    static Log& getInstance() 
    {
        static Log instance;
        return instance;
    }

    void message(int level, const std::string& msg) 
    {
        std::time_t t = std::time(nullptr);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        std::string timeStr(buf);

        Event ev{timeStr, level, msg};
        if (events.size() >= MAX_EVENTS)
            events.pop_front();
        events.push_back(ev);
    }

    void print() 
    {
        std::cout << "===== Last " << events.size() << " events =====" << std::endl;
        for (const auto& ev : events) 
        {
            std::string levelStr;
            if (ev.level == LOG_NORMAL) 
                levelStr = "NORMAL";
            else if (ev.level == LOG_WARNING) 
                levelStr = "WARNING";
            else 
                levelStr = "ERROR";

            std::cout << ev.timeStr << " [" << levelStr << "] " << ev.text << std::endl;
        }
        std::cout << "===============================" << std::endl;
    }
};

int main() 
{
    Log& log = Log::getInstance();

    log.message(LOG_NORMAL, "program loaded");
    log.message(LOG_WARNING, "configuration file not found, using defaults");
    log.message(LOG_ERROR, "error happens! help me!");
    log.message(LOG_NORMAL, "shutting down");

    for (int i = 0; i < 8; ++i)
    {
        log.message(LOG_NORMAL, "dummy " + std::to_string(i));
    }

    log.print();

    Log& another = Log::getInstance();
    std::cout << "Same instance? " << (&log == &another) << std::endl;

    return 0;
}