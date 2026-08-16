#pragma once

#include <memory>
#include <string>

namespace dlloader
{
	template
	class IDLLoader
	{
	public:
		//virtual ~IDLLoader() = default;
        IDLLoader();
        virtual ~IDLLoader();
		/*
		** Load the library and map it in memory.
		*/
		virtual void DLOpenLib() = 0;
		//virtual auto DLGetInstance(0) = 0;
		/*
		** Unload the library.
		*/
		virtual void DLCloseLib() = 0;
        /*
         * * Return a shared pointer on an instance of class loaded through
         ** a dynamic library.
         */
        virtual std::shared_ptr DLGetInstance() = 0;
	}
}
