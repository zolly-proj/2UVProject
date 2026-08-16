#include <iostream>
#include <dlfcn.h>
#include "clIDLLoader.h"

namespace dlloader
{
	template
	class DLLoader : public IDLLoader
	{
	private:
		void			*_handle;
		std::string		_pathToLib;
		std::string		_allocClassSymbol;
		std::string		_deleteClassSymbol;

	public:
		DLLoader(std::string const &pathToLib,
			std::string const &allocClassSymbol = "allocator",
			std::string const &deleteClassSymbol = "deleter") :
			  _handle(nullptr), _pathToLib(pathToLib),
			  _allocClassSymbol(allocClassSymbol),
        _deleteClassSymbol(deleteClassSymbol)
		{
		}

		~DLLoader() = default;

		void DLOpenLib()
		{
			if (!(_handle = dlopen(_pathToLib.c_str(), RTLD_NOW | RTLD_LAZY))) {
				std::cerr << dlerror() << std::endl;
			}
		}

		void DLCloseLib() override
		{
			if (dlclose(_handle) != 0) {
				std::cerr << dlerror() << std::endl;
			}
		}

		std::shared_ptr DLGetInstance() override
		{
            using allocClass = T *(*)();
            using deleteClass = void (*)(T *);

            auto allocFunc = reinterpret_cast(
                dlsym(_handle, _allocClassSymbol.c_str()));
            auto deleteFunc = reinterpret_cast(
                dlsym(_handle, _deleteClassSymbol.c_str()));

            if (!allocFunc || !deleteFunc) {
                DLCloseLib();
                std::cerr << dlerror() << std::endl;
            }
            return std::shared_ptr(allocFunc(),[deleteFunc](T *p){ deleteFunc(p); });
        }
	};
}
