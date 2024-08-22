# DLL loader

This header is windows specific, because it uses the win32 dll functions. The linux equivalent is .so, but i don't really have any good way to code for it, so it won't exist for a while. All contents are in the DLLloader namespace. LoadedFuncSet is typedef'd as am `std::vector < std::pair<LPCWSTR, bool>>`.

#### LoadedDLL `class`

This class can load a DLL.

##### Private vars (listed for maintenance uses)

- dll              `HINSTANCE`
- functionsloaded  `LoadedFuncSet`
- unparsedmetadata `LPCWSTR` `unused for now`
- loadedfile       `bool`
- metadataloaded   `bool`

##### Methods exposed

- `LoadedDLL(LPCWSTR name,bool* error)` The error field is set to true if dll loading fails
- `template<typename T> T collect_typed_funcptr(LPCSTR name, bool* err)` This function loads and returns a function pointer. If it fails to load the function pointer, it returns NULL. The private variable `functionsloaded` is set as required.
-  `get_loaded_functions()` Functions as a getter for the private variable `functionsloaded`