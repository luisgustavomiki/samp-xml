#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_utils.hpp"

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

	XMLPOINTER createPointer(XMLHANDLE file);
	void destroyPointer(XMLPOINTER pointer);
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
		//XMLPointer(XMLPointer* copy);
	};
private:
	std::vector<XMLHandler::XMLFile*> files;
	std::vector<XMLHandler::XMLPointer*> pointers;
	XMLPointer* getByPointer(XMLPOINTER pointer);
	XMLFile* getByHandle(XMLHANDLE handle);
};