#ifndef ENTRY_H
#define ENTRY_H

class Transceiver {
private:
	BaseParser *pParser;
};

class Http : public Transceiver {
public:
	Http(BaseParser *pParser);
	~Http();
};

class Serial : public Transceiver {
public:
	Serial();
	~Serial();
};


#endif
