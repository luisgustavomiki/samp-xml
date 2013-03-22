#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_utils.hpp"

#include "plugin.h"

typedef int XMLHANDLE;
typedef int XMLPOINTER;

class XMLHandler {
public:
	XMLHandler(void);
	~XMLHandler(void);
	XMLHANDLE loadFile(std::string path);
	XMLPOINTER createPointer(XMLHANDLE file);

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
		static XMLPOINTER lastOperationHandle;
		XMLPOINTER handle;
		XMLHANDLE file;

		enum XMLMode {
			XMLMODE_NODE,
			XMLMODE_LOOP_NODE,
			XMLMODE_LOOP_ATTR
		};

		XMLPointer(XMLHANDLE filehandle);

	private:
		XMLMode currentMode;
		rapidxml::xml_node<>* currentNode;
		rapidxml::xml_attribute<>* currentAttribute;
	};
private:
	std::vector<XMLHandler::XMLFile*> files;
	std::vector<XMLHandler::XMLPointer*> pointers;
	XMLFile* getByHandle(XMLHANDLE handle);
};

