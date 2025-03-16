from abc import ABC, abstractmethod
from enum import Enum
from typing import Dict, List

class VehicleType(Enum):
    TWO_WHEELER = "two_wheeler"
    FOUR_WHEELER = "four_wheeler"
    
class ParkingSpot(ABC):
    
    @abstractmethod
    def get_spot_id(self):
        pass
    
    @abstractmethod    
    def occupy_spot(self):
        pass
        
    @abstractmethod    
    def vacate_spot(self):
        pass
    
    @abstractmethod
    def get_vehicle_type(self):
        pass
    
class TwoWheelerParkingSpot(ParkingSpot):
    def __init__(self, spot_id: str):
        self.spot_id = spot_id
        self.is_occupied = False
        
    def get_spot_id(self):
        return self.spot_id
        
    def occupy_spot(self):
        self.is_occupied = True
        
    def vacate_spot(self):
        self.is_occupied = False
        
    def get_vehicle_type(self):
        return VehicleType.TWO_WHEELER.value
        
class FourWheelerParkingSpot(ParkingSpot):
    def __init__(self, spot_id: str):
        self.spot_id = spot_id
        self.is_occupied = False
        
    def get_spot_id(self):
        return self.spot_id
        
    def occupy_spot(self):
        self.is_occupied = True
        
    def vacate_spot(self):
        self.is_occupied = False
        
    def get_vehicle_type(self):
        return VehicleType.FOUR_WHEELER.value

class SpotManager(ABC):
    
    @abstractmethod
    def add_spot(self, spot: ParkingSpot):
        pass
    
    @abstractmethod
    def remove_spot(self, spot: ParkingSpot):
        pass
    
    @abstractmethod
    def find_spot(self):
        pass
    
    @abstractmethod
    def free_spot(self, spot_id: str):
        pass
    

class TwoWheelerSpotManager(SpotManager):
    def __init__(self):
        self.spots: List[ParkingSpot] = []
        
    def add_spot(self, spot: ParkingSpot):
        if isinstance(spot, TwoWheelerParkingSpot):
            self.spots.append(spot)
        else:
            raise TypeError("This manager only handles two-wheeler spots")
        
    def remove_spot(self, spot: ParkingSpot):
        if spot in self.spots:
            self.spots.remove(spot)
        else:
            print(f"Spot {spot.get_spot_id()} not found in the manager.")
        
    def find_spot(self):
        for spot in self.spots:
            if not spot.is_occupied:
                return spot
        return None
        
    def free_spot(self, spot_id: str):
        for spot in self.spots:
            if spot.get_spot_id() == spot_id:
                spot.vacate_spot()
                return
    
        
class FourWheelerSpotManager(SpotManager):
    def __init__(self):
        self.spots: List[ParkingSpot] = []
        
    def add_spot(self, spot: ParkingSpot):
        if isinstance(spot, FourWheelerParkingSpot):
            self.spots.append(spot)
        else:
            raise TypeError("This manager only handles four-wheeler spots")
        
    def remove_spot(self, spot: ParkingSpot):
        if spot in self.spots:
            self.spots.remove(spot)
        else:
            print(f"Spot {spot.get_spot_id()} not found in the manager.")
        
    def find_spot(self):
        for spot in self.spots:
            if not spot.is_occupied:
                return spot
        return None
        
    def free_spot(self, spot_id: str):
        for spot in self.spots:
            if spot.get_spot_id() == spot_id:
                spot.vacate_spot()
                return
        
class CostCalculatorStrategy(ABC):
    
    @abstractmethod
    def calculate_cost(self, duration: int):
        pass
    
class TwoWheelerCostCalculator(CostCalculatorStrategy):
    
    def calculate_cost(self, duration: int):
        return (duration // 60) * 20
        
class FourWheelerCostCalculator(CostCalculatorStrategy):
    
    def calculate_cost(self, duration: int):
        return (duration // 60) * 30

class Ticket:
    def __init__(self, ticket_id: str, entry_time: int, spot_id: str):
        self.ticket_id = ticket_id
        self.entry_time = entry_time
        self.exit_time = entry_time
        self.spot_id = spot_id
        
    def get_duration(self):
        return self.exit_time - self.entry_time
        
    def set_exit_time(self, time: int):
        self.exit_time = time
        
    
class ParkingLot:
    def __init__(self, spot_manager: SpotManager, cost_calc: CostCalculatorStrategy):
        self.spot_manager = spot_manager
        self.cost_calc = cost_calc
        self.active_tickets: Dict[str, Ticket] = {}
    
    def park(self, vehicle_type: VehicleType, entry_time: int):
        parking_spot = self.spot_manager.find_spot()
        if parking_spot:
            spot_id = parking_spot.get_spot_id()
            ticket_id = f"ticket_{spot_id}"
            ticket = Ticket(ticket_id, entry_time, spot_id)
            parking_spot.occupy_spot()
            self.active_tickets[ticket_id] = ticket
            return ticket_id
        else:
            print(f"No parking spot available for {vehicle_type}")

    def un_park(self, ticket_id: str, exit_time: int):
        if ticket_id in self.active_tickets:
            ticket: Ticket = self.active_tickets[ticket_id]
            ticket.set_exit_time(exit_time)
            spot_id = ticket.spot_id
            self.spot_manager.free_spot(spot_id)
            duration = ticket.get_duration()
            del self.active_tickets[ticket_id]
            return self.cost_calc.calculate_cost(duration)
        else:
            print("Invalid ticket")
            
class Entry:
    def process_entry(self, vehicle_type: VehicleType, entry_time: int, parking_lot: ParkingLot):
        return parking_lot.park(vehicle_type, entry_time)
        
class Exit:
    def process_exit(self, ticket_id: str, exit_time: int, parking_lot: ParkingLot):
        return parking_lot.un_park(ticket_id, exit_time)
        

if __name__ == "__main__":
    two_wheeler_manager = TwoWheelerSpotManager()
    two_wheeler_manager.add_spot(TwoWheelerParkingSpot("TW-01"))
    cost_calc = TwoWheelerCostCalculator()
    parking_lot = ParkingLot(two_wheeler_manager, cost_calc)
    
    entry = Entry()
    ticket_id = entry.process_entry(VehicleType.TWO_WHEELER, 100, parking_lot)
    print(f"Ticket issued: {ticket_id}")

    exit_handler = Exit()
    cost = exit_handler.process_exit(ticket_id, 160, parking_lot)
    print(f"Parking cost: {cost}")
