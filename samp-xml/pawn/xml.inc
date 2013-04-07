// SA:MP XML Plugin by Stewie` 
// 0.2

native XMLFile:xml_open(input[]);
native xml_close(XMLFile:file);
native xml_save(XMLFile:file);

native XMLPointer:xml_pointer(XMLFile:file);
native XMLPointer:xml_clonepointer(XMLPointer:pointer);
native xml_killpointer(XMLPointer:pointer);

native xml_pointer_getfilehandle(XMLPointer:pointer);
native xml_pointer_getname(XMLPointer:pointer, name[], len = sizeof(name));
native xml_pointer_getvalue(XMLPointer:pointer, value[], len = sizeof(value));
native xml_pointer_getvalue_int(XMLPointer:pointer);
native xml_pointer_getvalue_float(XMLPointer:pointer);

native xml_pointer_root(XMLPointer:pointer);
native xml_pointer_childnode(XMLPointer:pointer, node[] = "");
native xml_pointer_nextnode(XMLPointer:pointer, node[] = "");
native xml_pointer_parentnode(XMLPointer:pointer);
native xml_pointer_childattr(XMLPointer:pointer, attr[] = "");
native xml_pointer_nextattr(XMLPointer:pointer, attr[] = "");

native xml_pointer_delete(XMLPointer:pointer);
native xml_pointer_appendnode(XMLPointer:pointer, name[], value[]="");
native xml_pointer_prependnode(XMLPointer:pointer, name[], value[]="");