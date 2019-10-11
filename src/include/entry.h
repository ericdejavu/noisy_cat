#ifndef ENTRY_H
#define ENTRY_H

class Http {
private:
	BaseParser *pParser;

public:
	Http(BaseParser *pParser);
	~Http();
};


#endif
