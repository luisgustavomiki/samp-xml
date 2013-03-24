#pragma once
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
	try {
		this->path = file;
		this->file = new rapidxml::file<> (file.c_str());
		this->handle = ++XMLFile::lastFileHandle;
		this->doc = new rapidxml::xml_document<> ();
		this->doc->parse<0>(this->file->data());
	} catch(std::runtime_error a) {
		//logprintf((char*)a.what());
		Debug::write(a.what());
		this->handle = 0;
	}

}

XMLHandler::XMLFile::~XMLFile() {
	if(this->handle) {
		delete this->file;
		delete this->doc;
	}
}

// Pointers Class Constructor
XMLHandler::XMLPointer::XMLPointer(XMLHANDLE filehandle) {
	this->file = filehandle;
	this->handle = ++XMLPointer::lastOperationHandle;
}

/*
XMLHandler::XMLPointer::XMLPointer(XMLPointer* copy) {
	*this = *copy;
	this->handle = ++XMLPointer::lastOperationHandle;
}
*/

// loadFile - File Class Constructor Interface
XMLHANDLE XMLHandler::loadFile(std::string path) {
	XMLHandler::XMLFile* f = new XMLHandler::XMLFile(path);
	if(f->OK) {
		this->files.push_back(f);
		return this->files.back()->handle;
	} else {
		delete f;
		return 0;
	}
}

void XMLHandler::unloadFile(XMLHANDLE file) {
	if(this->isValidFile(file))
		delete this->getByHandle(file);
}

bool XMLHandler::isValidFile(XMLHANDLE file) {
	return this->getByHandle(file) != NULL;
}

// createPointer - Pointer Class Constructor Interface
XMLPOINTER XMLHandler::createPointer(XMLHANDLE file) {
	if(!this->isValidFile(file))
		return 0;

	XMLHandler::XMLPointer* p = (new XMLHandler::XMLPointer(file));
	this->pointers.push_back(p);

	p->currentMode = XMLHandler::XMLPointer::XMLMode::XMLMODE_DOC;
	p->element = this->getByHandle(file)->doc;

	return this->files.back()->handle;
}

void XMLHandler::destroyPointer(XMLPOINTER pointer) {
	if(this->isValidPointer(pointer))
		delete this->getByPointer(pointer);
}

bool XMLHandler::isValidPointer(XMLHANDLE file) {
	return this->getByPointer(file) != NULL;
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
		/*char buffer[80];
		sprintf(buffer, "getByPointer pointer(%d - %d) 0x%x", pointer, this->pointers[i]->handle, this->pointers[i]);
		Debug::write(buffer);*/

		if(this->pointers[i]->handle == pointer) {
			return this->pointers[i];
	   }
	}
	return NULL;
}

// Data Functions:
bool XMLHandler::name(XMLPOINTER _pointer, std::string* output) {
	XMLPointer* pointer = this->getByPointer(_pointer);

	/*char buffer[80];
	sprintf(buffer, "pointer(%d) 0x%x", _pointer, pointer);
	Debug::write(buffer);*/

	// If pointer exists
	if(pointer != NULL) {
		if(pointer->element != NULL) {
			rapidxml::xml_base<>* n = (rapidxml::xml_base<>*) (pointer->element);
			/*sprintf(buffer, "getting name: %s", n->name());
			Debug::write(buffer);*/
			*output = n->name();
			return true;
		}
	}
	return false;
}

bool XMLHandler::value(XMLPOINTER _pointer, std::string* output) {
	XMLPointer* pointer = this->getByPointer(_pointer);
	// If pointer exists
	if(pointer != NULL) {
		if(pointer->element != NULL) {
			rapidxml::xml_base<>* n = (rapidxml::xml_base<>*) (pointer->element);
			*output = n->value();
			return true;
		}
	}
	return false;
}

bool XMLHandler::jumpToDocument(XMLPOINTER _pointer) {
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

bool XMLHandler::jumpToChildNode(XMLPOINTER _pointer, std::string node) {
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

bool XMLHandler::jumpToNextNode(XMLPOINTER _pointer, std::string node) {
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

bool XMLHandler::jumpToParentNode(XMLPOINTER _pointer) {
	// get current data
	XMLPointer* pointer = this->getByPointer(_pointer);
	// is valid pointer
	if(pointer != NULL) {
		if(pointer->currentMode == XMLHandler::XMLPointer::XMLMode::XMLMODE_ATTR) {
			rapidxml::xml_base<>* n = (rapidxml::xml_base<>*) (pointer->element);
			pointer->currentMode = XMLHandler::XMLPointer::XMLMode::XMLMODE_NODE;
			pointer->element = pointer->nodetemp;
			return true;
		}
		if(pointer->currentMode == XMLHandler::XMLPointer::XMLMode::XMLMODE_NODE) {
			rapidxml::xml_base<>* n = (rapidxml::xml_base<>*) (pointer->element);
			if(n->parent() != NULL) {
				pointer->currentMode = XMLHandler::XMLPointer::XMLMode::XMLMODE_NODE;
				pointer->element = n;
				return true;
			}
		}
	}
	return false;
}

bool XMLHandler::jumpToAttr(XMLPOINTER _pointer, std::string attr) {
	// Getting current data
	XMLPointer* pointer = this->getByPointer(_pointer);
	// If current
	if(pointer != NULL) {
		// if current position would be suitable to have a child attribute
		if(pointer->currentMode == XMLHandler::XMLPointer::XMLMode::XMLMODE_NODE) {
			rapidxml::xml_node<>* n = (rapidxml::xml_node<>*) (pointer->element);
			rapidxml::xml_attribute<>* a;
			// get first child attr
			if(attr.length() > 0) {
				a = n->first_attribute(attr.c_str());
			} else {
				a = n->first_attribute();
			}
			// is there a child node
			if(a != NULL) {
				// complete operation
				pointer->nodetemp = pointer->element;
				pointer->element = a;
				pointer->currentMode = XMLHandler::XMLPointer::XMLMODE_ATTR;
				return true;
			}
		}
	}
	return false;
}

