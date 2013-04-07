#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_utils.hpp"
#include "rapidxml\rapidxml_print.hpp"

#include "Debug.h"

typedef int XMLHANDLE;
typedef int XMLPOINTER;

class XMLHandler {
public:
	XMLHandler(void);
	~XMLHandler(void);
	XMLHANDLE loadFile(std::string path);
	void unloadFile(XMLHANDLE file);
	bool isValidFile(XMLHANDLE file);
	bool saveFile(XMLHANDLE file);
	XMLHANDLE getFile(XMLPOINTER pointer);

	XMLPOINTER createPointer(XMLHANDLE file);
	void destroyPointer(XMLPOINTER pointer);
	XMLPOINTER clonePointer(XMLPOINTER pointer);
	bool isValidPointer(XMLHANDLE file);
	/*
	 *	Data functions
	 */
	bool name(XMLPOINTER _pointer, std::string* output);
	bool value(XMLPOINTER _pointer, std::string* output);

	bool jumpToDocument(XMLPOINTER _pointer);

	bool jumpToChildNode(XMLPOINTER _pointer, std::string node);
	bool jumpToNextNode(XMLPOINTER _pointer, std::string node);
	bool jumpToParentNode(XMLPOINTER _pointer);

	bool jumpToAttr(XMLPOINTER _pointer, std::string attr);
	bool jumpToNextAttr(XMLPOINTER _pointer, std::string node);

	bool prependnode(XMLPOINTER _pointer, std::string _name, std::string _value);
	bool appendnode(XMLPOINTER _pointer, std::string _name, std::string _value);
	bool remove(XMLPOINTER _pointer);
	/*
		Sets 
			*/


	struct XMLFile {
		static XMLHANDLE lastFileHandle;
		bool OK;

		XMLHANDLE handle;
		std::string path;
		rapidxml::file<>* file;
		rapidxml::xml_document<>* doc;

		XMLFile(std::string file);
		~XMLFile();
	};
	struct XMLPointer {
		enum XMLMode {
			XMLMODE_DOC,
			XMLMODE_NODE,
			XMLMODE_ATTR
		};

		static XMLPOINTER lastOperationHandle;
		XMLPOINTER handle;
		XMLHANDLE file;
		
		XMLMode currentMode;
		void* element;
		void* nodetemp;

		XMLPointer(XMLHANDLE filehandle);
		~XMLPointer();
		//XMLPointer(XMLPointer* copy);
	};
private:
	std::vector<XMLHandler::XMLFile*> files;
	std::vector<XMLHandler::XMLPointer*> pointers;
	XMLPointer* getByPointer(XMLPOINTER pointer);
	XMLFile* getByHandle(XMLHANDLE handle);
};