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
	for (auto & i : this->files) {
		delete i;
	}
	for (auto & j : this->pointers) {
		delete j;
	}
}

// Files Class Constructor
XMLHandler::XMLFile::XMLFile(std::string file) {
	this->path = file;
	this->file = new rapidxml::file<> (file.c_str());
	this->handle = ++XMLFile::lastFileHandle;
	this->doc = new rapidxml::xml_document<> ();
	this->doc->parse<0>(this->file->data());
}

XMLHandler::XMLFile::~XMLFile() {
	delete this->file;
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
void XMLHandler::name(XMLPOINTER _pointer, std::string* output) {
	XMLPointer* pointer = this->getByPointer(_pointer);
	// If pointer exists
	if(pointer != NULL) {
		if(pointer->element != NULL) {
			rapidxml::xml_base<>* n = (rapidxml::xml_base<>*) (pointer->element);
			*output = n->name();
		}
	}
}

void XMLHandler::value(XMLPOINTER _pointer, std::string* output) {
	XMLPointer* pointer = this->getByPointer(_pointer);
	// If pointer exists
	if(pointer != NULL) {
		if(pointer->element != NULL) {
			rapidxml::xml_base<>* n = (rapidxml::xml_base<>*) (pointer->element);
			*output = n->value();
		}
	}
}

bool XMLHandler::jumpToStart(XMLPOINTER _pointer) {
	XMLPointer* pointer = this->getByPointer(_pointer);
	// If pointer exists
	if(pointer != NULL) {
		// Set current node to document pointer. It has the same effect
		pointer->currentMode = XMLHandler::XMLPointer::XMLMode::XMLMODE_DOC;
		pointer->element = this->getByHandle(pointer->file)->doc;
		return true;
	}
	return false;
}

bool XMLHandler::jumpFromStart(XMLPOINTER _pointer, std::string node) {
	XMLPointer* pointer = this->getByPointer(_pointer);
	// If pointer exists
	if(pointer != NULL) {
		// Get file pointer
		XMLFile* file = getByHandle(pointer->file);
		// Get first node from document
		rapidxml::xml_node<>* n;
		if(node.length() > 0) {
			n = file->doc->first_node(node.c_str());
		} else {
			n = file->doc->first_node();
		}
		// If there's a node matching with std::string node
		if(n != NULL) {
			pointer->element = n;
			pointer->currentMode = XMLHandler::XMLPointer::XMLMODE_NODE;
			return true;
		}
	}
	return false;
}

bool XMLHandler::jumpToChild(XMLPOINTER _pointer, std::string node) {
	// Getting current data
	XMLPointer* pointer = this->getByPointer(_pointer);
	// If current
	if(pointer != NULL) {
		// if current position would be suitable to have a child node
		if(pointer->currentMode == XMLHandler::XMLPointer::XMLMode::XMLMODE_DOC ||
			pointer->currentMode == XMLHandler::XMLPointer::XMLMode::XMLMODE_NODE) {
			// get node poiner
			rapidxml::xml_node<>* n = (rapidxml::xml_node<>*) (pointer->element);
			// get first child node
			if(node.length() > 0) {
				n = n->first_node(node.c_str());
			} else {
				n = n->first_node();
			}
			// is there a child node
			if(n != NULL) {
				// complete operation
				pointer->element = n;
				pointer->currentMode = XMLHandler::XMLPointer::XMLMODE_NODE;
				return true;
			}
		}
	}
	return false;
}

bool XMLHandler::jumpToNext(XMLPOINTER _pointer, std::string node) {
	// get current data
	XMLPointer* pointer = this->getByPointer(_pointer);
	// is valid pointer
	if(pointer != NULL) {
		// if current position would be suitable to have a sibling
		if(pointer->currentMode == XMLHandler::XMLPointer::XMLMode::XMLMODE_DOC ||
			pointer->currentMode == XMLHandler::XMLPointer::XMLMode::XMLMODE_NODE) {
			// get node from pointer
			rapidxml::xml_node<>* n = (rapidxml::xml_node<>*) (pointer->element);
			// get sibling
			if(node.length() > 0) {
				n = n->next_sibling(node.c_str());
			} else {
				n = n->next_sibling();
			}
			if(n != NULL) {
				pointer->element = n;
				pointer->currentMode = XMLHandler::XMLPointer::XMLMODE_NODE;
			}
			return true;
		}
	}
	return false;
}


