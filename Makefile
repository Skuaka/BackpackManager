flag = -std=c++17

objects = Backpack.o BackpackEvent.o Item.o ItemBar.o StorageRoom.o main.o

game : ${objects}
	g++ -o game ${objects} ${flag}


main.o : ItemExample.h main.cpp BackpackEvent.h
	g++ -c main.cpp ${flag}

BackpackEvent.o : BackpackEvent.h BackpackEvent.cpp Backpack.o ItemBar.o
	g++ -c BackpackEvent.cpp ${flag}

Backpack.o : Backpack.h Backpack.cpp StorageRoom.o
	g++ -c Backpack.cpp ${flag}

ItemBar.o :ItemBar.h ItemBar.cpp StorageRoom.o
	g++ -c ItemBar.cpp ${flag}

StorageRoom.o : StorageRoom.h StorageRoom.cpp Item.h
	g++ -c StorageRoom.cpp ${flag}

Item.o : Item.h Item.cpp
	g++ -c Item.cpp ${flag}


.PHONY : clean
clean:
	rm game ${objects} *.o.tmp
