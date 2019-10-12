#ifndef ENTRY_H
#define ENTRY_H

class Server {
private:
	BaseParser *pParser;
};

class Http : public Server {
public:
	Http(BaseParser *pParser);
	~Http();
};

class Serial : public Server {
public:
	Serial();
	~Serial();
};


#endif
