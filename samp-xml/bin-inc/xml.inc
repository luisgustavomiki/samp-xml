// SA:MP XML Plugin by Stewie` 
// 0.1

native XMLFile:xml_open(input[]);
native xml_close(XMLFile:file);

native XMLPointer:xml_pointer(XMLFile:file);
native xml_killpointer(XMLPointer:pointer);

native xml_pointer_getname(XMLPointer:pointer, name[], len = sizeof(name));
native xml_pointer_getvalue(XMLPointer:pointer, value[], len = sizeof(value));

native xml_pointer_root(XMLPointer:pointer);
native xml_pointer_childnode(XMLPointer:pointer, node[] = "");
native xml_pointer_nextnode(XMLPointer:pointer, node[] = "");
native xml_pointer_parentnode(XMLPointer:pointer);

native xml_pointer_attr(XMLPointer:pointer, attr[] = "");