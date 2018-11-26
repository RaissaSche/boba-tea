#include "UserData.h"

UserData::UserData()
{
	isIgual = 0;
}

UserData::~UserData(){}

int UserData::getCont()
{
	return cont;
}

char UserData::getAuxiliarBorda()
{
	return auxiliarBorda;
}

int UserData::getIsIgual()
{
	return isIgual;
}

int UserData::getId()
{
	return id;
}

void UserData::setCont(int cont)
{
	this->cont = cont;
}

void UserData::setAuxiliarBorda(char auxiliarBorda)
{
	this->auxiliarBorda = auxiliarBorda;
}

void UserData::addIsIgual()
{
	isIgual++;
}

void UserData::setId(int id)
{
	this->id = id;
}
