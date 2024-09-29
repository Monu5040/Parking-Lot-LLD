#include <bits/stdc++.h>
using namespace std;

// Enum for Vehicle Type
enum class VehicleType {
    TWOWHEELER,
    FOURWHEELER
};

// Interface for Cost Calculation (DIP)
class CostCalculator {
public:
    virtual int calculateCost(int duration) const = 0;
    virtual ~CostCalculator() = default;
};

// CostCalculator for TwoWheeler (OCP)
class TwoWheelerCostCalculator : public CostCalculator {
public:
    int calculateCost(int duration) const override {
        return (duration / 60) * 10; // Rate: 10 currency units per hour
    }
};

// CostCalculator for FourWheeler (OCP)
class FourWheelerCostCalculator : public CostCalculator {
public:
    int calculateCost(int duration) const override {
        return (duration / 60) * 20; // Rate: 20 currency units per hour
    }
};

// Interface for Parking Spot Management (ISP)
class ParkingSpot {
public:
    virtual string getSpotId() const = 0;
    virtual bool isOccupied() const = 0;
    virtual void occupy() = 0;
    virtual void vacate() = 0;
    virtual VehicleType getVehicleType() const = 0;
    virtual ~ParkingSpot() = default;
};

// Implementation of Parking Spot for TwoWheelers
class TwoWheelerSpot : public ParkingSpot {
private:
    string spot_id;
    bool occupied;

public:
    TwoWheelerSpot(string id) : spot_id(id), occupied(false) {}

    string getSpotId() const override { return spot_id; }
    bool isOccupied() const override { return occupied; }
    void occupy() override { occupied = true; }
    void vacate() override { occupied = false; }
    VehicleType getVehicleType() const override { return VehicleType::TWOWHEELER; }
};

// Implementation of Parking Spot for FourWheelers
class FourWheelerSpot : public ParkingSpot {
private:
    string spot_id;
    bool occupied;

public:
    FourWheelerSpot(string id) : spot_id(id), occupied(false) {}

    string getSpotId() const override { return spot_id; }
    bool isOccupied() const override { return occupied; }
    void occupy() override { occupied = true; }
    void vacate() override { occupied = false; }
    VehicleType getVehicleType() const override { return VehicleType::FOURWHEELER; }
};

// Interface for Ticket Management (ISP)
class Ticket {
public:
    virtual string getTicketId() const = 0;
    virtual void setExitTime(int exit_time) = 0;
    virtual int getParkingDuration() const = 0;
    virtual string getSpotId() const = 0;
    virtual ~Ticket() = default;
};

// Ticket Implementation (SRP)
class ParkingTicket : public Ticket {
private:
    string ticket_id;
    int entry_time;
    int exit_time;
    string spot_id;

public:
    ParkingTicket(string ticket_id, int entry_time, string spot_id) 
        : ticket_id(ticket_id), entry_time(entry_time), spot_id(spot_id), exit_time(-1) {}

    string getTicketId() const override { return ticket_id; }
    void setExitTime(int exit_time) override { this->exit_time = exit_time; }
    int getParkingDuration() const override { return exit_time - entry_time; }
    string getSpotId() const override { return spot_id; }
};

// Class to Manage Parking Spots (SRP)
class SpotManager {
private:
    vector<ParkingSpot*> two_wheeler_spots;
    vector<ParkingSpot*> four_wheeler_spots;

public:
    SpotManager(vector<ParkingSpot*> two_wheeler_spots, vector<ParkingSpot*> four_wheeler_spots) 
        : two_wheeler_spots(two_wheeler_spots), four_wheeler_spots(four_wheeler_spots) {}

    ParkingSpot* findFreeSpot(VehicleType v_type) {
        if (v_type == VehicleType::TWOWHEELER) {
            for (auto& spot : two_wheeler_spots) {
                if (!spot->isOccupied()) {
                    return spot;
                }
            }
        } else if (v_type == VehicleType::FOURWHEELER) {
            for (auto& spot : four_wheeler_spots) {
                if (!spot->isOccupied()) {
                    return spot;
                }
            }
        }
        return nullptr;
    }

    void freeSpot(string spot_id) {
        for (auto& spot : two_wheeler_spots) {
            if (spot->getSpotId() == spot_id) {
                spot->vacate();
                return;
            }
        }
        for (auto& spot : four_wheeler_spots) {
            if (spot->getSpotId() == spot_id) {
                spot->vacate();
                return;
            }
        }
    }
};

// Class to Manage Parking Lot Operations (SRP, OCP)
class ParkingLot {
private:
    SpotManager* spot_manager;
    unordered_map<string, Ticket*> active_tickets;
    unordered_map<VehicleType, CostCalculator*> cost_calculators;

public:
    ParkingLot(SpotManager* spot_manager, unordered_map<VehicleType, CostCalculator*> calculators)
        : spot_manager(spot_manager), cost_calculators(calculators) {}

    Ticket* parkVehicle(VehicleType v_type, int entry_time) {
        ParkingSpot* spot = spot_manager->findFreeSpot(v_type);
        if (spot != nullptr) {
            string ticket_id = "ticket_" + spot->getSpotId();
            Ticket* ticket = new ParkingTicket(ticket_id, entry_time, spot->getSpotId());
            active_tickets[ticket_id] = ticket;
            spot->occupy();
            cout << "Vehicle parked. Ticket ID: " << ticket_id << endl;
            return ticket;
        } else {
            cout << "No available spots for this vehicle type." << endl;
            return nullptr;
        }
    }

    void unParkVehicle(string ticket_id, int exit_time) {
        if (active_tickets.find(ticket_id) != active_tickets.end()) {
            Ticket* ticket = active_tickets[ticket_id];
            ticket->setExitTime(exit_time);
            spot_manager->freeSpot(ticket->getSpotId());

            // Calculate cost
            int duration = ticket->getParkingDuration();
            VehicleType v_type = (ticket->getSpotId().find("4W") != string::npos) ? VehicleType::FOURWHEELER : VehicleType::TWOWHEELER;
            int cost = cost_calculators[v_type]->calculateCost(duration);

            cout << "Vehicle unparked. Ticket ID: " << ticket_id << " Parking cost: $" << cost << endl;
            active_tickets.erase(ticket_id);
        } else {
            cout << "Invalid ticket ID!" << endl;
        }
    }
};

// Entry Gate
class EntryGate {
public:
    void processEntry(VehicleType v_type, int entry_time, ParkingLot* parking_lot) {
        parking_lot->parkVehicle(v_type, entry_time);
    }
};

// Exit Gate
class ExitGate {
public:
    void processExit(string ticket_id, int exit_time, ParkingLot* parking_lot) {
        parking_lot->unParkVehicle(ticket_id, exit_time);
    }
};

int main() {
    // Create parking spots
    vector<ParkingSpot*> two_wheeler_spots = { new TwoWheelerSpot("p201"), new TwoWheelerSpot("p202"), new TwoWheelerSpot("p203") };
    vector<ParkingSpot*> four_wheeler_spots = { new FourWheelerSpot("p401"), new FourWheelerSpot("p402"), new FourWheelerSpot("p403") };

    // Spot manager
    SpotManager* spot_manager = new SpotManager(two_wheeler_spots, four_wheeler_spots);

    // Cost calculators
    unordered_map<VehicleType, CostCalculator*> cost_calculators = {
        { VehicleType::TWOWHEELER, new TwoWheelerCostCalculator() },
        { VehicleType::FOURWHEELER, new FourWheelerCostCalculator() }
    };

    // Parking lot
    ParkingLot* parking_lot = new ParkingLot(spot_manager, cost_calculators);

    // Entry and Exit Gates
    EntryGate entry_gate;
    ExitGate exit_gate;

    // Simulate parking and unparking
    entry_gate.processEntry(VehicleType::TWOWHEELER, 100, parking_lot);
    entry_gate.processEntry(VehicleType::FOURWHEELER, 200, parking_lot);

    exit_gate.processExit("ticket_p201", 180, parking_lot);  // Unpark TwoWheeler
    exit_gate.processExit("ticket_p401", 360, parking_lot);  // Unpark FourWheeler

    return 0;
}
