import os
from struct import pack, unpack
from dataclasses import dataclass

@dataclass
class NpcTblEntry:
    bits: int = 0
    life: int = 0
    surf: int = 0
    hit_voice: int = 0
    destroy_voice: int = 0
    size: int = 0
    exp: int = 0
    damage: int = 0 
    hit: tuple = (0,0,0,0)
    view: tuple = (0,0,0,0)

class NpcTbl:
    def __init__(self, fileName = ""):
        self.data = []
        if fileName:
            self.loadFromFile(fileName)
    
    def size(self):
        return len(self.data)
    
    def loadFromFile(self, fileName):
        self.data = [NpcTblEntry() for i in range(os.path.getsize(fileName) // 24)]
        
        with open(fileName, 'rb') as f:
            for npc in self.data:
                npc.bits = unpack("<H", f.read(2))[0]
            for npc in self.data:
                npc.life = unpack("<H", f.read(2))[0]
            for npc in self.data:
                npc.surf = f.read(1)[0]
            for npc in self.data:
                npc.destroy_voice = f.read(1)[0]
            for npc in self.data:
                npc.hit_voice = f.read(1)[0]
            for npc in self.data:
                npc.size = f.read(1)[0]
            for npc in self.data:
                npc.exp = unpack("<i", f.read(4))[0]
            for npc in self.data:
                npc.damage = unpack("<i", f.read(4))[0]
            for npc in self.data:
                npc.hit = unpack("BBBB", f.read(4))
            for npc in self.data:
                npc.view = unpack("BBBB", f.read(4))
    
    def writeToFile(self, fileName):
        with open(fileName, 'wb') as f:
            for npc in self.data:
                f.write(pack("<H", npc.bits))
            for npc in self.data:
                f.write(pack("<H", npc.life))
            for npc in self.data:
                f.write(pack("B", npc.surf))
            for npc in self.data:
                f.write(pack("B", npc.destroy_voice))
            for npc in self.data:
                f.write(pack("B", npc.hit_voice))
            for npc in self.data:
                f.write(pack("B", npc.size))
            for npc in self.data:
                f.write(pack("<i", npc.exp))
            for npc in self.data:
                f.write(pack("<i", npc.damage))
            for npc in self.data:
                f.write(pack("BBBB", *npc.hit))
            for npc in self.data:
                f.write(pack("BBBB", *npc.view))

if __name__ == "__main__":
    print("Enter the path to npc.tbl #1 (the one that you are adding NPCs to).")
    print("Or drag and drop it here and press Enter:")
    tbl1Path = input(">> ").strip('"')
    
    print("\nEnter the path to npc.tbl #2 (the one containing the new NPCs to be added).")
    print("Or drag and drop it here and press Enter:")
    tbl2Path = input(">> ").strip('"')
    
    npctbl1 = NpcTbl(tbl1Path)
    print("\nLoaded {} NPCs from npc.tbl #1".format(npctbl1.size()))
    
    npctbl2 = NpcTbl(tbl2Path)
    print("Loaded {} NPCs from npc.tbl #2".format(npctbl2.size()))
    
    option = 1
    if npctbl2.size() > npctbl1.size():
        print("\nHow would you like to combine these two npc.tbls?")
        print("1: Add all {} NPCs from npc.tbl #2 (npc.tbl #1 will have {} NPCs after)".
            format(npctbl2.size(), npctbl1.size() + npctbl2.size()))
        print("2: Add only NPCs {}-{} from npc.tbl #2 to npc.tbl #1".format(npctbl1.size(), npctbl2.size()-1))
        
        while True:
            try:
                option = int(input(">> "))
                if option == 1 or option == 2:
                    break
                else:
                    raise ValueError("not a valid option")
            except ValueError:
                print("Please enter a valid option number")
    else:
        print("\nGoing to add {} NPCs from npc.tbl #2 to npc.tbl #1".format(npctbl2.size()))
    
    print("\nEnter the file name that you would like to save the new npc.tbl to:")
    outputFileName = input(">> ")
    
    if option == 1:
        npctbl1.data.extend(npctbl2.data)
    elif option == 2:
        npctbl1.data.extend(npctbl2.data[npctbl1.size():])
    npctbl1.writeToFile(outputFileName)
    
    print("Wrote {} NPCs to {}".format(npctbl1.size(), os.path.realpath(outputFileName)))
    print("All done! (Press Enter to exit)")
    input()
