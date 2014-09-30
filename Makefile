
PROJ_THIRD_PARTY=third_party/build/android
PROJ_COMMON=common/proj/android
PROJ_RENDER=JHRenderEngine/proj/android
PROJ_GAME=JHGameEngine/proj/android
PROJ_APPUTILS=JHAppEngine/android/utilities
PROJ_APP=JHAppEngine/android/main


.PHONY: all third_party JHEngine common JHRenderEngine JHGameEngine JHAppUtil JHAppEngine clean_Common clean_JHRenderEngine clean_JHGameEngine clean_JHAppEngine clean_JHAppUtil clean_JHEngine clean_third_party clean clean_all

all: third_party JHEngine
	@echo "[----------------- all done -------------------------]"

third_party:
	@echo "[----------------- Building third_party libs -------------------------]"
	pushd . && cd $(PROJ_THIRD_PARTY) && ndk-build all && popd
	@echo "[----------------- third_party done -------------------------]"

JHEngine: common JHRenderEngine JHGameEngine JHAppEngine
	@echo "[----------------- JHEngine done -------------------------]"

common:
	@echo "[----------------- Building Common -------------------------]"
	pushd . && cd $(PROJ_COMMON) && ndk-build && popd
	@echo "[----------------- common done -------------------------]"

JHRenderEngine:
	@echo "[----------------- Building JHRenderEngine -------------------------]"
	pushd . && cd $(PROJ_RENDER) && ndk-build && popd
	@echo "[----------------- JHRenderEngine done -------------------------]"

JHGameEngine:
	@echo "[----------------- Building JHGameEngine -------------------------]"
	pushd . && cd $(PROJ_GAME) && rm -Rf bin/* libs/*/* && pwd && ant all && popd
	mv $(PROJ_GAME)/bin/classes.jar $(PROJ_GAME)/bin/jhgameengine.jar
	@echo "[----------------- JHGameEngine done -------------------------]"

JHAppUtil:
	@echo "[----------------- Building JHAppUtil -------------------------]"
	pushd . && cd $(PROJ_APPUTILS) && rm -Rf bin/* libs/*/* && pwd && ant all && popd
	mv $(PROJ_APPUTILS)/bin/classes.jar $(PROJ_APPUTILS)/bin/jhappengineutils.jar
	@echo "[----------------- JHAppUtil done -------------------------]"

JHAppEngine: JHAppUtil
	@echo "[----------------- Building JHAppEngine -------------------------]"
	rm -Rf $(PROJ_APP)/libs/*/* $(PROJ_APP)/bin/*
	cp $(PROJ_APPUTILS)/bin/jhappengineutils.jar $(PROJ_APP)/libs/jhappengineutils.jar
	pushd . && cd $(PROJ_APP) && ant all && popd
	mv $(PROJ_APP)/bin/classes.jar $(PROJ_APP)/bin/jhappengine.jar
	@echo "[----------------- JHAppEngine done -------------------------]"

clean_third_party:
	pushd . && cd $(PROJ_THIRD_PARTY) && ndk-build clean && popd

clean_Common:
	pushd . && cd $(PROJ_COMMON) && rm -Rf libs/*/*.so bin/*.jar && ndk-build clean && popd

clean_JHRenderEngine:
	pushd . && cd $(PROJ_RENDER) && rm -Rf libs/*/*.so bin/*.jar && ndk-build clean && popd

clean_JHGameEngine:
	pushd . && cd $(PROJ_GAME) && rm -Rf libs/*/*.so bin/*.jar jni/SwigGen/* src/com/JHEngine/JHGameEngine/SwigGen/* && ant clean && popd

clean_JHAppUtil:
	pushd . && cd $(PROJ_APPUTILS) && rm -Rf libs/*/*.so bin/*.jar && ant clean && popd

clean_JHAppEngine:
	pushd . && cd $(PROJ_APP) && rm -Rf libs/*/*.so bin/*.jar && ant clean && popd

clean_JHEngine: clean_JHAppEngine clean_JHAppUtil clean_JHGameEngine clean_JHRenderEngine clean_Common

clean: clean_JHEngine

clean_all: clean_JHEngine clean_third_party
