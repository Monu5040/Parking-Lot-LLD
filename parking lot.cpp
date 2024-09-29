#include<bits/stdc++.h>
using namespace std;

class Vehicle {
private:
	string vehicle_type;
	string registration_num;
	string color;
public:
	Vehicle(string v_type, string r_num, string color) :
		vehicle_type(v_type), registration_num(r_num), color(color) {}

	string getVehicleRegistrationNum() {
		return registration_num;
	}
	string getVehicleColor() {
		return color;
	}
	string getVehicleType() {
		return vehicle_type;
	}
};

class Slot {
private:
	int slot_id;
	string slot_type;
	bool is_occupied;
	Vehicle* vehicle;
public:
	Slot(int slot_id, string slot_type, bool is_occupied) :
		slot_id(slot_id), slot_type(slot_type), is_occupied(is_occupied) {}

	void park(Vehicle* vehicle) {
		this->is_occupied = true;
		this->vehicle = vehicle;
	}

	void unPark() {
		is_occupied = false;
	}

	bool isOccupied() {
		return is_occupied;
	}
	int getSlotId() {
		return slot_id;
	}
	string getSlotType() {
		return slot_type;
	}
};

class Floor {
private:
	int floor_id;
	map<int,set<Slot*>>total_slots;
public:
	Floor(int floor_id) : floor_id(floor_id) {}

	void addSlot(Slot* slot) {
		total_slots[floor_id].insert(slot);
	}

	// void removeSlot(Slot* slot) {
	// 	if(total_slots.find(slot) != total_slots.end()) {
	// 		total_slots.erase(slot);
	// 	}
	// }

	set<Slot*> getAvailableSlot(){
		set<Slot*> available_slot;
		for(auto& slot : total_slots[floor_id]) {
			if(slot->isOccupied() == false) {
				available_slot.insert(slot);
			}
		}
		return available_slot;
	}

	set<Slot*> getOccupiedSlot() {
		set<Slot*> occupied_slot;
		for(auto& slot: total_slots[floor_id]) {
			if(slot->isOccupied() == true) {
				occupied_slot.insert(slot);
			}
		}
		return occupied_slot;
	}

	int getFloorId() {
		return floor_id;
	}
	map<int,set<Slot*>> getTotalSlots() {
		return total_slots;
	}
};

class Ticket {
private:
	string parking_slot_id;
	int floor_id;
	int slot_id;
public:
	Ticket(string parking_slot_id,int floor_id, int slot_id): 
		parking_slot_id(parking_slot_id), floor_id(floor_id), slot_id(slot_id) {}

	string getTicketId() {
		string ticket_id = parking_slot_id + "_" + to_string(floor_id) + "_" + to_string(slot_id);
		return ticket_id;
	}

};

class ParkingLot {
private:
	string parking_slot_id;
	map<int,Floor*> floors;
public:
	ParkingLot(string parking_slot_id) : parking_slot_id(parking_slot_id) {}

	void addFloor(Floor* floor) {
		floors[floor->getFloorId()] = floor;
	}


	pair<Slot*,int> findAvailableSlot() {
		pair<Slot*,int>parking_spot;
		for(auto& floor : floors) {
			set<Slot*> slots = floor.second->getAvailableSlot();
			if(slots.size() > 0) {
				auto itr = slots.begin();
				return {*itr, floor.first};
			}
		}
		
		return {nullptr, -1};
	}
	Ticket* parkVehicle(Vehicle* vehicle){
		pair<Slot*,int> slot = findAvailableSlot();
		if(slot.first) {
			slot.first->park(vehicle);
			Ticket* ticket = new Ticket(parking_slot_id, slot.second,slot.first->getSlotId());
			return ticket;
		}
		return nullptr;
		

	}

	void unParkVehicle(int floor_id, int slot_id) {
		if(floors.find(floor_id) != floors.end()) {
			map<int,set<Slot*>> total_slots = floors[floor_id]->getTotalSlots();
			for(auto& slot : total_slots[floor_id]) {
				if(slot->getSlotId() == slot_id) {
					slot->unPark();
					break;
				}
			}
		}
	}

	void displayAvailableSlot(string vehicle_type) {
		cout<<"Vehicle Type: "<<vehicle_type<<endl;
		for(auto& floor : floors) {
			set<Slot*> slots = floor.second->getAvailableSlot();
			cout<<"{floor: " << floor.first<<", Available slots : [ ";
			for(auto& slot : slots) {
				if(slot->getSlotType() == vehicle_type) {
					cout<<slot->getSlotId()<<", ";
				}
			}
			cout<<"]}"<<endl;
		}
		cout<<endl;
	}

	void displayOccupiedSlot(string vehicle_type) {
		cout<<"Vehicle Type: "<<vehicle_type<<endl;
		for(auto& floor : floors) {
			set<Slot*> slots = floor.second->getOccupiedSlot();
			cout<<"{floor: " << floor.first<<", Occupied slots : [ ";
			for(auto& slot : slots) {
				if(slot->getSlotType() == vehicle_type) {
					cout<<slot->getSlotId()<<", ";
				}
			}
			cout<<"]}"<<endl;
		}
		cout<<endl;
	}
};

int main() {
	ParkingLot parking_lot("PA1234");
	Floor* floor1 = new Floor(1);
	floor1->addSlot(new Slot(1,"Truck",false));
	floor1->addSlot(new Slot(2,"Bike",false));
	floor1->addSlot(new Slot(3,"Bike",false));
	floor1->addSlot(new Slot(4,"Car",false));
	floor1->addSlot(new Slot(5,"Car",false));
	floor1->addSlot(new Slot(6,"Car",false));

	parking_lot.addFloor(floor1);
	Floor* floor2 = new Floor(2);
	floor2->addSlot(new Slot(1,"Truck",false));
	floor2->addSlot(new Slot(2,"Bike",false));
	floor2->addSlot(new Slot(3,"Bike",false));
	floor2->addSlot(new Slot(4,"Car",false));
	floor2->addSlot(new Slot(5,"Car",false));
	parking_lot.addFloor(floor2);
	Floor* floor3 = new Floor(3);
	floor3->addSlot(new Slot(1,"Truck",false));
	floor3->addSlot(new Slot(2,"Bike",false));
	floor3->addSlot(new Slot(3,"Bike",false));
	floor3->addSlot(new Slot(4,"Car",false));
	parking_lot.addFloor(floor3);
	parking_lot.displayAvailableSlot("Truck");
	Vehicle* vehicle = new Vehicle("Bike","DL-10-AC-8605","Blue");
	Ticket* ticket = parking_lot.parkVehicle(vehicle);
	cout<<"ticket: "<<ticket->getTicketId()<<endl;
	parking_lot.displayOccupiedSlot("Bike");
	parking_lot.displayAvailableSlot("Bike");
	// parking_lot.unParkVehicle(1,3);
	// parking_lot.displayOccupiedSlot("Bike");
	// parking_lot.displayAvailableSlot("Bike");
}