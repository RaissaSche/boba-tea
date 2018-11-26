#pragma once
class UserData
{
public:
	UserData();
	~UserData();

	int getCont();
	char getAuxiliarBorda();
	int getIsIgual();
	int getId();

	void setCont(int cont);
	void setAuxiliarBorda(char auxiliarBorda);
	void addIsIgual();
	void setId(int id);

private:

	int cont;
	char auxiliarBorda;
	int isIgual;
	int id;
};