#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath> 

class Checkpoint 
{
public:
    Checkpoint(const std::string& name, double lat, double lon)
        : name_(name), latitude_(lat), longitude_(lon) {}
    virtual ~Checkpoint() = default;

    std::string getName() const { return name_; }
    double getLatitude() const { return latitude_; }
    double getLongitude() const { return longitude_; }
    virtual bool isMandatory() const = 0;
    virtual double getPenalty() const { return 0.0; }

private:
    std::string name_;
    double latitude_;
    double longitude_;
};

class MandatoryCheckpoint : public Checkpoint 
{
public:
    MandatoryCheckpoint(const std::string& name, double lat, double lon)
        : Checkpoint(name, lat, lon) {}
    bool isMandatory() const override { return true; }
};

class OptionalCheckpoint : public Checkpoint 
{
public:
    OptionalCheckpoint(const std::string& name, double lat, double lon, double penalty)
        : Checkpoint(name, lat, lon), penalty_(penalty) {}
    bool isMandatory() const override { return false; }
    double getPenalty() const override { return penalty_; }
private:
    double penalty_;
};

class CheckpointBuilder 
{
public:
    virtual ~CheckpointBuilder() = default;
    virtual void startProcessing() = 0;
    virtual void processMandatory(int index, const MandatoryCheckpoint& cp) = 0;
    virtual void processOptional(int index, const OptionalCheckpoint& cp) = 0;
    virtual void finishProcessing() = 0;
};

class TextOutputBuilder : public CheckpointBuilder 
{
private:
    std::string output;
public:
    void startProcessing() override 
    {
        output = "===== Список контрольных пунктов =====\n";
    }

    void processMandatory(int index, const MandatoryCheckpoint& cp) override 
    {
        output += std::to_string(index) + ". " + cp.getName() + " | "
               + std::to_string(cp.getLatitude()) + ", " + std::to_string(cp.getLongitude())
               + " | незачёт СУ\n";
    }

    void processOptional(int index, const OptionalCheckpoint& cp) override 
    {
        output += std::to_string(index) + ". " + cp.getName() + " | "
               + std::to_string(cp.getLatitude()) + ", " + std::to_string(cp.getLongitude())
               + " | штраф: " + std::to_string(cp.getPenalty()) + " ч\n";
    }

    void finishProcessing() override 
    {
        output += "=====================================\n";
        std::cout << output;
    }

    std::string getResult() const { return output; }
};

class PenaltyCounterBuilder : public CheckpointBuilder 
{
private:
    double totalPenalty;

public:
    void startProcessing() override 
    {
        totalPenalty = 0.0;
    }

    void processMandatory(int, const MandatoryCheckpoint&) override {}

    void processOptional(int, const OptionalCheckpoint& cp) override 
    {
        totalPenalty += cp.getPenalty();
    }

    void finishProcessing() override 
    {
        std::cout << "Суммарный штраф по необязательным КП: " << totalPenalty << " ч." << std::endl;
    }

    double getTotalPenalty() const { return totalPenalty; }
};

void processCheckpoints(const std::vector<std::unique_ptr<Checkpoint>>& checkpoints, CheckpointBuilder& builder) {
    builder.startProcessing();
    int index = 1;
    for (const auto& cp : checkpoints) 
    {
        if (cp->isMandatory()) 
        {
            const MandatoryCheckpoint* m = dynamic_cast<const MandatoryCheckpoint*>(cp.get());
            if (m) 
                builder.processMandatory(index, *m);
        } 
        else 
        {
            const OptionalCheckpoint* o = dynamic_cast<const OptionalCheckpoint*>(cp.get());
            if (o) 
                builder.processOptional(index, *o);
        }
        ++index;
    }
    builder.finishProcessing();
}

int main() 
{
    std::vector<std::unique_ptr<Checkpoint>> checkpoints;
    checkpoints.push_back(std::make_unique<MandatoryCheckpoint>("Старт", 55.751244, 37.618423));
    checkpoints.push_back(std::make_unique<OptionalCheckpoint>("Лесной участок", 55.755, 37.62, 0.5));
    checkpoints.push_back(std::make_unique<MandatoryCheckpoint>("Река", 55.760, 37.63));
    checkpoints.push_back(std::make_unique<OptionalCheckpoint>("Гора", 55.770, 37.65, 1.2));
    checkpoints.push_back(std::make_unique<MandatoryCheckpoint>("Финиш", 55.780, 37.67));

    TextOutputBuilder textBuilder;
    processCheckpoints(checkpoints, textBuilder);

    PenaltyCounterBuilder penaltyBuilder;
    processCheckpoints(checkpoints, penaltyBuilder);
}