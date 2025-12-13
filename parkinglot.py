from abc import ABC, abstractmethod

class Vehicle:
    def __init__(self,number, v_type):
        self.number = number
        self.v_type = v_type
        
class ParkingSpot:
    def __init__(self,id,type):
        self.id = id
        self.is_occupied = False
        self.vehicle = None
        self.type = type
        
    def park(self,vehicle):
        self.vehicle = vehicle
        self.is_occupied = True
        
    def unpark(self):
        self.vehicle = None
        self.is_occupied = False
        
    def get_price(self):
        return 5
        
class TwoWheelerParkingSpot(ParkingSpot):
    def get_price(self):
        return 20
        
class FourWheelerParkingSpot(ParkingSpot):
    def get_price(self):
        return 50
        
class ParkingSpotManager:
    def __init__(self, parking_spots=None, parking_strategy=None,payment_strategy=None):
        self.parking_spots = parking_spots
        self.praking_strategy = parking_strateg if parking_strategy else DefaultStrategy()
        self.payment_strategy = payment_strategy
    
    def add_parking_spot(self, spot):
        self.parking_spots.append(spot)
        
    def remove_parking_spot(self,spot):
        self.parking_spots.remove(spot)
        
    def find_free_spot(self, v_type,gate_no):
        for spot in self.spots:
            if spot.type == v_type and not spot.is_occupied:
                return spot
        print(f"Free spot not available for {v_type}")     
        return None
    
    def park_vehicle(self,spot, vehicle):
        spot.park(vehicle)
        
    def unpark_vehicle(self):
        spot.unpark()
    

class TwoWheelerParkingSpotManager(ParkingSpotManager):
    def __init__(self):
        super().__init__(parking_spots=[],parking_strategy,payment_strategy)
        
class FourWheelerParkingSpotManager(ParkingSpotManager):
    def __init__(self):
        super().__init__(parking_spots=[],parking_strategy,payment_strategy)
        
class ParkingSpotManagerFactory:
    
    @staticmethod
    def get_parking_spot_manager(vehicle_type):
        if vehicle_type.lower() == 'two_wheeler':
            return TwoWheelerParkingSpotManager()
            
        elif vehicle_type.lower() == 'four_wheeler':
            return FourWheelerParkingSpotManager()
            
class Ticket:
    def __init__(self,id,spot,vehicle,entry_time):
        self.id = id
        self.spot = spot
        self.vehicle = vehicle
        self.entry_time = entry_time
        self.exit_time = 0
        
    
class ParkingStrategy(ABC):
    
    @abstractmethod
    def find_free_spot(self):
        pass

class DefaultStrategy(ParkingStrategy):
    
    def find_free_spot(self):
        print("Free spot allocated by default")

class NearToEntrance(ParkingStrategy):
    
    def find_free_spot(self):
        print("Free spot near to entrance")
        
class NearToEntranceAndElevator(ParkingStrategy):
    
    def find_free_spot(self):
        print("Free spot near to entrance and elevator")
        
class Entrance:
    def __init__(self, spot_manager_factory,vehicle_type,vehicle_number,entry_time):
        self.spot_manager_factory = spot_manager_factory
        self.vehicle = Vehicle(vehicle_number,vehicle_type)
        self.spot_manager = spot_manager_factory.get_parking_spot_manager(vehicle_type)
        self.entry_time = entry_time
        
    def find_parking_spot(self,v_type,gate_no):
        return self.spot_manager.find_free_spot(v_type, gate_no)
    
    def book_spot(self,vehicle,spot):
        self.spot_manager.park_vehicle(vehicle,spot)
    
    def generate_ticket(self,spot, vehicle):
        id = vehicle.number + spot.type
        return Ticket(id,spot,vehicle, self.entry_time)
    
    def entry(self):
        spot = self.find_parking_spot(v_type,gate_no)
        self.book_spot(vehicle, spot)
        ticket = self.generate_ticket(spot,vehicle)
        return ticket
        
class PricingStrategy(ABC):
    
    @abstractmethod
    def calculate_price(self,ticket):
        pass
    
class HourlyBasedPrice(PricingStrategy):
    
    def calculate_price(self,ticket):
        print(f"Hourly price")
        amount = ticket.spot.get_price() + ((ticket.exit_time - ticket.entry_time)//60)*0.5
        return amount
        

class MinuteBasedPrice(PricingStrategy):
    
    def calculate_price(self,ticket):
        print("Minute based price")
        amount = ticket.spot.get_price() + ((ticket.exit_time - ticket.entry_time))*0.5
        return amount
        
class PaymentStrategy(ABC):
    
    @abstractmethod
    def make_payment(self, amount):
        pass

class CreditCardPayment(PaymentStrategy):
    
    def pay(self, amount):
        print(f"Making payment of {amount} via credit card")
        
class DebitCardPayment(PaymentStrategy):
    
    def pay(self, amount):
        print(f"Making payment of {amount} via debit card")
  
class UPIPayment(PaymentStrategy):
    
    def pay(self, amount):
        print(f"Making payment of {amount} via upi")  

class Reciept:
    def __init__(self,id,amount,payment_status):
        self.id = id
        self.amount = amount
        self.payment_status = payment_status

class Exit:
    def __init__(self,spot_manager_factory, ticket,exit_time):
        self.spot_manager = spot_manager_factory.get_parking_spot_manager(ticket.vehicle.v_type)
        self.ticket = ticket
        self.exit_time = exit_time
        
    def calculate_price(self):
        return self.spot_manager.payment_strategy.calculate_price(self.ticket)
    
    def make_payment(self, amount,payment_strategy):
        payment_strategy.pay(amount)
    
    def generate_reciept(self):
        amount self.calculate_price()
        return Reciept()
    
    def free_spot(self):
        pass
    
        
