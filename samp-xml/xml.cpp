#include "xml.h"

/*
	SA:MP XML Plugin by Stewie`
	2013
*/

/*
 * XML files are loaded by the XMLFile class
 * but you can't really do a thing with them
 * without creating "Pointers", or Operation Handlers.
 * I'm sorry if this was a terrible idea for
 * genericizing OO methods into Pawn.
 * At least you can access the same file
 * multiple times at once.
 */

XMLHANDLE XMLHandler::XMLFile::lastFileHandle = 0;
XMLPOINTER XMLHandler::XMLPointer::lastOperationHandle = 0;

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

// Pointers Class Constructor
XMLHandler::XMLPointer::XMLPointer(XMLHANDLE filehandle) {
	this->file = filehandle;
	this->handle = ++XMLPointer::lastOperationHandle;
}

XMLHandler::XMLPointer::XMLPointer(XMLPointer* copy) {
	*this = *copy;
	this->handle = ++XMLPointer::lastOperationHandle;
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
	return NULL;
}

// getByHandle - Gets pointer to object from XMLHANDLE uniqueid.
XMLHandler::XMLPointer* XMLHandler::getByPointer(XMLPOINTER pointer) {
	for(int i = 0; i != this->pointers.size(); ++i) {
		if(this->pointers[i]->handle == pointer) {
			return this->pointers[i];
	   }
	}
	return NULL;
}

// Data Functions:
bool XMLHandler::jumpToStart(XMLPOINTER _pointer) {
	XMLPointer* pointer = this->getByPointer(_pointer);
	if(pointer != NULL) {
		pointer->currentMode = XMLHandler::XMLPointer::XMLMode::XMLNODE_DOC;
		pointer->element = NULL;
		return true;
	}
	return false;
}

bool XMLHandler::jumpFromStart(XMLPOINTER _pointer, std::string node) {
	XMLPointer* pointer = this->getByPointer(_pointer);
	if(pointer != NULL) {
		XMLFile* file = getByHandle(pointer->file);
		rapidxml::xml_node<>* n = file->doc->first_node(node.c_str());

		pointer->element = n;
		pointer->currentMode = XMLHandler::XMLPointer::XMLMODE_NODE;
		return true;
	}
	return false;
}

bool XMLHandler::jumpToChild(XMLPOINTER _pointer, std::string node) {
	XMLPointer* pointer = this->getByPointer(_pointer);
	if(pointer != NULL) {
		rapidxml::xml_node<>* n = (rapidxml::xml_node<>*) (pointer->element);
		n = n->first_node(node.c_str());

		pointer->element = n;
		pointer->currentMode = XMLHandler::XMLPointer::XMLMODE_NODE;
		return true;
	}
	return false;
}

bool XMLHandler::jumpToNext(XMLPOINTER _pointer, std::string node) {
	XMLPointer* pointer = this->getByPointer(_pointer);
	if(pointer != NULL) {
		rapidxml::xml_node<>* n = (rapidxml::xml_node<>*) (pointer->element);
		n = n->first_node(node.c_str());

		pointer->element = n;
		pointer->currentMode = XMLHandler::XMLPointer::XMLMODE_NODE;
		return true;
	}
	return false;
}


