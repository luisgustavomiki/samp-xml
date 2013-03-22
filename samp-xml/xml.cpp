#include "xml.h"


XMLHandler::XMLHandler(void) {
}


XMLHandler::~XMLHandler(void) {
}

// Files Class Constructor
XMLHandler::XMLFile::XMLFile(std::string file) {
	this->path = file;
	this->file = new rapidxml::file<> (file.c_str());
	this->handle = ++XMLFile::lastFileHandle;
	this->doc = new rapidxml::xml_document<> ();
	this->doc->parse<0>(this->file->data());
}

// loadFile - File Class Constructor Interface
XMLHANDLE XMLHandler::loadFile(std::string path) {
	this->files.push_back(new XMLHandler::XMLFile(path));
	return this->files.back()->handle;
}

// createPointer - Pointer Class Constructor Interface
XMLPOINTER XMLHandler::createPointer(XMLHANDLE file) {
	this->pointers.push_back(new XMLHandler::XMLPointer(file));
	return this->files.back()->handle;
}

// getByHandle - Gets pointer to object from XMLHANDLE uniqueid.
XMLHandler::XMLFile* XMLHandler::getByHandle(XMLHANDLE handle) {
	for(int i = 0; i != this->files.size(); ++i) {
		if(this->files[i]->handle == handle) {
			return this->files[i];
	   }
	}
	return nullptr;
}

