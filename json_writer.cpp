#include <string>
#include <fstream>
#include <stdexcept>

class JsonWriter
{
public:
    JsonWriter(const std::string& filename): file_(filename)
    {
        if (!file_.is_open())
            throw std::runtime_error("Cannot open file");
    }

    ~JsonWriter()
    {
        if (file_.is_open())
            file_.close();
    }

    JsonWriter(const JsonWriter&) = delete;
    JsonWriter& operator=(const JsonWriter&) = delete;

    JsonWriter(JsonWriter&& other)
        : file_(std::move(other.file_))
    {}

    JsonWriter& operator=(JsonWriter&& other) 
    {
        if (this != &other)
            file_ = std::move(other.file_);
        return *this;
    }

    void Add(const std::string& key, const std::string& value) 
    {
        file_ << "{\"" << key << "\":\"" << value << "\"}\n";
    }

private:
    std::ofstream file_;
};

class Entity
{
public:
    Entity(const std::string& filename) : writer_(filename) {}

    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;

    void Add(const std::string& key, const std::string& value)
    {
        writer_.Add(key, value);
    }

private:
    JsonWriter writer_;
};