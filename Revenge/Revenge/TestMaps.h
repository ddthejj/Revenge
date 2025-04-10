#pragma once
#include "Map.h"
#include "Room.h"
#include "Character.h"


class TestMap0 : public Map
{
public:
	TestMap0() : Map("TestMap0") { CreateRooms(); }
protected:
	virtual void CreateRooms() override;
};

class TestRoom0 : public Room
{
public:
	TestRoom0() : Room("TestRoom0") { InterpretRoomData(CreateRoomData()); }
protected:
	virtual RoomData CreateRoomData() override;
};

class TestRoom1 : public Room
{
public:
	TestRoom1() : Room("TestRoom1") { InterpretRoomData(CreateRoomData()); }
protected:
	virtual RoomData CreateRoomData() override;
};