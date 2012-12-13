#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string>
#include <string.h>

typedef int (*ENTRYPOINTPTR)(const char *rootdir, int argc, const char **argv);

int main(int argc, const char **argv) {
	std::string strDir(".");
	if (argc > 0) {
		const char *szMe = argv[0];
		const char *szEnd = strrchr(szMe, '/');
		if (szEnd) {
			strDir = std::string(szMe, szEnd);
		}
	}
	std::string strPathCommon = strDir + "/common.so";
	std::string strPathCore = strDir + "/core.so";
    void *common = dlopen(strPathCommon.c_str(), RTLD_NOW|RTLD_GLOBAL);
	if (!common) {
		fprintf(stderr, "Unable to open common library:\n\t%s\n", dlerror());
		return -1;
	}
	void *core = dlopen(strPathCore.c_str(), RTLD_NOW);
	if (!core) {
		fprintf(stderr, "Unable to open core library:\n\t%s\n", dlerror());
		dlclose(common);
		return -1;
	}
	ENTRYPOINTPTR entry = (ENTRYPOINTPTR)dlsym(core, "coreEntryPoint");
	if (!entry) {
		fprintf(stderr, "Entrypoint not found in core library.\n");
		dlclose(core);
		dlclose(common);
		return -1;
	}
	int retval = entry(strDir.c_str(), argc, argv);
	dlclose(core);
	dlclose(common);
	return retval;
}