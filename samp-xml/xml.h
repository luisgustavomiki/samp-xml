#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_utils.hpp"

typedef int XMLHANDLE;
typedef int XMLPOINTER;

class XMLHandler {
public:
	XMLHandler(void);
	~XMLHandler(void);
	XMLHANDLE loadFile(std::string path);
	XMLPOINTER createPointer(XMLHANDLE file);

	/*
	 *	Data functions
	 */
	bool jumpToStart(XMLPOINTER _pointer);
	bool jumpFromStart(XMLPOINTER _pointer, std::string node);
	bool jumpToChild(XMLPOINTER _pointer, std::string node);
	bool jumpToNextSibling(XMLPOINTER _pointer);

	struct XMLFile {
		static XMLHANDLE lastFileHandle;
		XMLHANDLE handle;
		std::string path;
		rapidxml::file<>* file;
		rapidxml::xml_document<>* doc;

		XMLFile(std::string file);
		~XMLFile();
	};
	struct XMLPointer {
		enum XMLMode {
			XMLNODE_DOC,
			XMLMODE_NODE,
			XMLMODE_LOOP_NODE,
			XMLMODE_LOOP_ATTR
		};

		static XMLPOINTER lastOperationHandle;
		XMLPOINTER handle;
		XMLHANDLE file;
		
		XMLMode currentMode;
		void* element;

		XMLPointer(XMLHANDLE filehandle);
		XMLPointer(XMLPointer* copy);
	};
private:
	std::vector<XMLHandler::XMLFile*> files;
	std::vector<XMLHandler::XMLPointer*> pointers;
	XMLPointer* getByPointer(XMLPOINTER pointer);
	XMLFile* getByHandle(XMLHANDLE handle);
};

