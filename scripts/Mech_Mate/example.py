

class Monkey:
    def __init__(self) -> None:
        self.bananas = {
            "Too" : 1, 
            "Many": 2,
            "Bananas" : 3
        }

    def getBananas(self) -> list:
        return self.bananas
    

    def printBananas(self):
        print(self.bananas)



class Humans:
    def __init__(self, bananaList: dict) -> None:
        self.stolenCultures = bananaList
        self.stolenCultures[1] = "Indian"
        self.stolenCultures[2] = "Japanese"


    def printStolenCultures(self):
        print(self.stolenCultures)



def main():
    animal = Monkey()
    animal.printBananas()

    asshole = Humans(animal.getBananas())
    asshole.printStolenCultures()

    animal.printBananas()





main()